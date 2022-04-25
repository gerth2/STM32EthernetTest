#ifndef _NT4_H
#define _NT4_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define NT4_TYPESTR_BOOL "boolean" 
#define NT4_TYPESTR_FLOAT_64 "double" 
#define NT4_TYPESTR_INT "int" 
#define NT4_TYPESTR_FLOAT_32 "float" 
#define NT4_TYPESTR_STR "string" 
#define NT4_TYPESTR_JSON "json" 
#define NT4_TYPESTR_BIN_RAW "raw" 
#define NT4_TYPESTR_BIN_RPC "rpc" 
#define NT4_TYPESTR_BIN_MSGPACK "msgpack" 
#define NT4_TYPESTR_BIN_PROTOBUF "protobuf" 
#define NT4_TYPESTR_BOOL_ARR "boolean[]" 
#define NT4_TYPESTR_FLOAT_64_ARR "double[]" 
#define NT4_TYPESTR_INT_ARR "int[]" 
#define NT4_TYPESTR_FLOAT_32_ARR "float[]" 
#define NT4_TYPESTR_STR_ARR "string[]" 

#define MAX_SUBSCRIPTIONS 10


typedef struct nt4_subscriptionOptions {
    bool immedeate;
    bool logging;
    float periodicRate_s;
} nt4_subscriptionOptions_t;

typedef struct nt4_subscription {
    char ** prefixes;
    uint32_t uid;
    nt4_subscriptionOptions_t options;
} nt4_subscription_t;

typedef struct nt4_topicProperties {
    bool isPersistent;
} nt4_topicProperties_t;

typedef struct nt4_topic {
    char * name;
    char * type;
    uint32_t id;

} nt4_topic_t;

#endif
