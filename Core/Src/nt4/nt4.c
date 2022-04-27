

#include "nt4.h"

uint8_t nt4_lookupTypestr(char * str) {

    if (strcmp(str, "double") == 0){return 1;
    } else if (strcmp(str, "int") == 0){return 2;
    } else if (strcmp(str, "float") == 0){return 3;
    } else if (strcmp(str, "string") == 0){return 4;
    } else if (strcmp(str, "json") == 0){return 4;
    } else if (strcmp(str, "raw") == 0){return 5;
    } else if (strcmp(str, "rpc") == 0){return 5;
    } else if (strcmp(str, "msgpack") == 0){return 5;
    } else if (strcmp(str, "protobuf") == 0){return 5;
    } else if (strcmp(str, "boolean[]") == 0){return 16;
    } else if (strcmp(str, "double[]") == 0){return 17;
    } else if (strcmp(str, "int[]") == 0){return 18;
    } else if (strcmp(str, "float[]") == 0){return 19;
    } else if (strcmp(str, "string[]") == 0){return 20;
    } else {return 0;
    }
}

static set_t subscriptions;
static set_t clientPublishedTopics;
static set_t serverAnnouncedTopics;

static uint32_t serverTimeOffset_us;

////////////////////////////////////////
// Private Helper Functions

void makeJSONForValReq(char * outBuf, char ** patternList, size_t numPatterns){

}

void makeJSONForSubscribe(char * outBuf, nt4_subscription_t * subList, size_t numSubs){

}

void makeJSONForUnSubscribe(char * outBuf, nt4_subscription_t * unSubList, size_t numSubs){

}

void makeJSONForSubOptions(char * outBuf, nt4_subscription_t * sub){

}

void makeJSONForTopicPublish(char * outBuf, nt4_topic_t * topic){

}

void makeJSONForTopicUnPublish(char * outBuf, nt4_topic_t * topic){

}

void makeJSONForTopicPropeties(char * outBuf, nt4_topic_t * topic){

}

void sendServerTimeSyncRequest(void){

}

void handleServerTimeSyncResponse(void){

}

uint32_t localTimeToServerTime(float localTime){
    return (uint32_t)(localTime * 1000000.0) + serverTimeOffset_us;
}

float serverTimeToLocalTime(uint32_t serverTime_us){
    return (float)(serverTime_us - serverTimeOffset_us) / 1000000.0;

}

nt4_topic_t * newTopic(char * name, char * type, int32_t id){
    nt4_topic_t * retVal = malloc(sizeof(nt4_topic_t));
    retVal->name = name;
    retVal->type = type;
    retVal->id = id;
    return retVal;
}

nt4_subscription_t * newSubscription(char ** prefixes, int32_t uid, nt4_subscriptionOptions_t * options){
    nt4_subscription_t * retVal = malloc(sizeof(nt4_subscription_t));
    retVal->prefixes = prefixes;
    retVal->uid = uid;
    retVal->options = options;
    return retVal;
}

nt4_subscriptionOptions_t * newSubscriptionOptions(bool immedeate, bool logging, float periodicRate_s){
    nt4_subscriptionOptions_t * retVal = malloc(sizeof(nt4_subscriptionOptions_t));
    retVal->immedeate = immedeate;
    retVal->logging = logging;
    retVal->periodicRate_s = periodicRate_s;
    return retVal;
}

////////////////////////////////////////
// Public API
void nt4_onTopicAnnounce(char * name, char * type, int32_t id){

}

void nt4_onTopicUnAnnounce(int32_t id){
    
}

void nt4_onNewTopicDataFloat(int32_t id, float val){

}

void nt4_onNewTopicDataBool(int32_t id, bool val){

}

void nt4_onConnect(){
    set_add(&serverAnnouncedTopics, newTopic("time", "int", NT4_TIME_TOPIC_ID), NT4_TIME_TOPIC_ID);

}

void nt4_onDisconnect(){

}


void nt4_subscribePeriodic(char ** patternList, size_t numPatterns){

}

void nt4_subscribeImmedeate(char ** patternList, size_t numPatterns){

}

void nt4_subscribeLogging(char ** patternList, size_t numPatterns){

}

void nt4_getValue(char ** patternList, size_t numPatterns){

}

void nt4_clearAllSubscriptions(){

}


void nt4_setProperties(nt4_topic_t * topic, bool isPersistant){

}

void nt4_publishTopic(nt4_topic_t * newTopic){

}

void nt4_unPublishTopic(nt4_topic_t * oldTopic){

}

void nt4_addSampleFloat(nt4_topic_t * topic, float time_sec, float value){


}

void nt4_addSampleBool(nt4_topic_t * topic, float time_sec, bool value){

}

void nt4_init(){
    set_init(&subscriptions);
    set_init(&clientPublishedTopics);
    set_init(&serverAnnouncedTopics);

}

void nt4_update(){

}