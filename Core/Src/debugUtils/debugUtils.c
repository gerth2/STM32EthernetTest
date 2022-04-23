
#include "debugUtils.h"

// Adapted from https://stackoverflow.com/questions/16647278/minimal-implementation-of-sprintf-or-printf
// Because apparently the stdiolib included with freertos/stm32 cube isn't thread safe
// and I can't seem to stomp on all the thread issues. So we made our own.

UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart) {
  gHuart = huart;
}


static void ftoa_fixed(char *buffer, double value);
static void ftoa_sci(char *buffer, double value);

static void safePutC(char **buf, char in){
	**buf = in;
	*buf += 1;
}

static void safePutS(char **buf, char * str_in){
	memcpy(*buf, str_in, strlen(str_in));
	*buf += strlen(str_in);
}

int my_vfprintf(char *buf, char const *fmt, va_list arg) {

    int int_temp;
    char char_temp;
    char *string_temp;
    double double_temp;
    char * bufStart = buf;

    char ch;

    char buffer[256];

    while ( ch = *fmt++) {
        if ( '%' == ch ) {
            switch (ch = *fmt++) {
                /* %% - print out a single %    */
                case '%':
                	safePutC(&buf, '%');
                    break;

                /* %c: print out a character    */
                case 'c':
                    char_temp = va_arg(arg, int);
                    safePutC(&buf, char_temp);
                    break;

                /* %s: print out a string       */
                case 's':
                    string_temp = va_arg(arg, char *);
                    safePutS(&buf, string_temp);
                    break;

                /* %d: print out an int         */
                case 'd':
                    int_temp = va_arg(arg, int);
                    itoa(int_temp, buffer, 10);
                    safePutS(&buf, buffer);
                    break;

				/* %u: print out an unsigned int         */
				case 'u':
					int_temp = va_arg(arg, int);
					utoa(int_temp, buffer, 10);
					safePutS(&buf, buffer);
					break;

                /* %x: print out an int in hex  */
                case 'x':
                case 'X':
                    int_temp = va_arg(arg, int);
                    utoa(int_temp, buffer, 16);
                    safePutS(&buf, "0x");
                    safePutS(&buf, buffer);
                    break;

                case 'f':
                	double_temp = va_arg(arg, double);
                    ftoa_fixed(buffer, double_temp);
                    safePutS(&buf, buffer);
                    break;

                case 'e':
                    double_temp = va_arg(arg, double);
                    ftoa_sci(buffer, double_temp);
                    safePutS(&buf, buffer);
                    break;


            }
        }  else {
        	//No formatting, just copy
        	safePutC(&buf, ch);
        }
    }

    //unconditionally inject null terminator
    *buf = 0x00;

    return buf - bufStart;
}

static int normalize(double *val) {
    int exponent = 0;
    double value = *val;

    while (value >= 1.0) {
        value /= 10.0;
        ++exponent;
    }

    while (value < 0.1) {
        value *= 10.0;
        --exponent;
    }
    *val = value;
    return exponent;
}

static void ftoa_fixed(char *buffer, double value) {
    /* carry out a fixed conversion of a double value to a string, with a precision of 5 decimal digits.
     * Values with absolute values less than 0.000001 are rounded to 0.0
     * Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
     * The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
     * e+308. The C standard requires an implementation to allow a single conversion to produce up to 512
     * characters, so that's what we really expect as the buffer size.
     */

    int exponent = 0;
    int places = 0;
    static const int width = 8;

    if (value == 0.0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0.0) {
        *buffer++ = '-';
        value = -value;
    }

    exponent = normalize(&value);

    while (exponent > 0) {
        int digit = value * 10;
        *buffer++ = digit + '0';
        value = value * 10 - digit;
        ++places;
        --exponent;
    }

    if (places == 0)
        *buffer++ = '0';

    *buffer++ = '.';

    while (exponent < 0 && places < width) {
        *buffer++ = '0';
        --exponent;
        ++places;
    }

    while (places < width) {
        int digit = value * 10.0;
        *buffer++ = digit + '0';
        value = value * 10.0 - digit;
        ++places;
    }
    *buffer = '\0';
}

void ftoa_sci(char *buffer, double value) {
    int exponent = 0;
    static const int width = 5;

    if (value == 0.0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return;
    }

    if (value < 0.0) {
        *buffer++ = '-';
        value = -value;
    }

    exponent = normalize(&value);

    int digit = value * 10.0;
    *buffer++ = digit + '0';
    value = value * 10.0 - digit;
    --exponent;

    *buffer++ = '.';

    for (int i = 0; i < width; i++) {
        int digit = value * 10.0;
        *buffer++ = digit + '0';
        value = value * 10.0 - digit;
    }

    *buffer++ = 'e';
    itoa(exponent, buffer, 10);
}

int threadSafeSPrintf(char * buf, const char *fmt, ...) {
    va_list arg;
    int length;

    va_start(arg, fmt);
    length = my_vfprintf(buf, fmt, arg);
    va_end(arg);

    return length;
}


int threadSafePrintf(const char *fmt, ...) {

    //return; //uncomment this to stop support for all print statments
    va_list arg;
    int length;

    char buf[256];

    va_start(arg, fmt);
    length = my_vfprintf(buf, fmt, arg);
    va_end(arg);

    taskENTER_CRITICAL();
    HAL_UART_Transmit(gHuart, (uint8_t *) &buf, strlen(buf), HAL_MAX_DELAY);
    taskEXIT_CRITICAL();

    return length;
}
