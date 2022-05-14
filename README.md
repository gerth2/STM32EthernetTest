# STM32EthernetTest
Scratch work testing STM32 integration with a freertos TCP/IP stack, mongoose for HTTP/Websockets, and ENC28J60 for MAC

This is horribly organized, specific to a pile of hardware I happen to have on my desk right now, and is only to test various concepts that keep coming into my brain. 

It is fit for no particular purpose.

# TODO List

1) NT4 client implementation in C
2) ~~Settings - web + backend~~
3) ~~Settings - EEPROM support~~
4) ~~Complementary filter on all axes~~
5) ~~Rolling calibration~~
6) Periodic save cal to EEPROM
7) w5500 support
8) Investigate OTA Flash Updates
9) Quaternion support
10) REV Color Sensor Support

# Open Source Software Referenced or Included:

 * [stm32-w5500](https://github.com/afiskon/stm32-w5500)
 * [AT24Cxx](https://github.com/cvmanjoo/AT24Cxx)
 * [Minimal implementation of printf](https://stackoverflow.com/questions/16647278/minimal-implementation-of-sprintf-or-printf)
 * [WPILib Rotation2d](https://first.wpi.edu/wpilib/allwpilib/docs/release/java/edu/wpi/first/math/geometry/Rotation2d.html)
 * [stm32-enc28j60](https://github.com/xaionaro/stm32-enc28j60)
 * [EtherCard-MDNS](https://github.com/valinet/EtherCard-MDNS)
 * [mjson](https://github.com/cesanta/mjson)
 * [mongoose](https://github.com/cesanta/mongoose)
 * [MPU60X0 Drivers from FocusFlight32](https://github.com/FocusFlight32/FF32/tree/406f04db0797ab5606bc435d301c52ba8bc99297/src/sensors)
 * [C Message Pack Serialization cmp](https://github.com/camgunz/cmp)
 * [FreeRTOS](https://www.freertos.org/)

