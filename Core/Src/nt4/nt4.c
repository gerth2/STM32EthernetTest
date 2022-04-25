

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

static nt4_subscription_t subscriptions[MAX_SUBSCRIPTIONS];


void nt4_onTopicAnnounce(){

}

void nt4_onTopicUnAnnounce(){
    
}

void nt4_onNewTopicData(){

}

void nt4_onConnect(){

}

void nt4_onDisconnect(){

}

void nt4_init(){

}

void nt4_update(){

}

void nt4_subscribePeriodic(char * pattern){

}

void nt4_subscribeImmedeate(char * pattern){

}

void nt4_subscribeLogging(char * pattern){

}

void nt4_getValue(char * pattern){

}

void nt4_clearAllSubscriptions(){

}


void nt4_setProperties(nt4_topic_t * topic, bool isPersistant){

}

void nt4_publishTopic(nt4_topic_t * newTopic){

}

void nt4_unPublishTopic(nt4_topic_t * oldTopic){

}

float nt4_getCurServerTime(){
return 0.0; //TODO
}

void nt4_addSampleFloat(nt4_topic_t * topic, float time_us, float value){

}

void nt4_addSampleBool(nt4_topic_t * topic, float time_us, bool value){

}
