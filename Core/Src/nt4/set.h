#ifndef _SET_H
#define _SET_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct set_member {

    void * content;
    int32_t id; 
    struct set_member * larger;
    struct set_member * smaller;

} set_member_t;

typedef struct set {

    set_member_t * root;
    uint32_t size;

} set_t;

void set_init(set_t * set);
uint8_t set_add(set_t * set, void * newContent, int32_t id);
uint8_t set_remove(set_t * set, int32_t id);
void * set_get(set_t * set, int32_t id);




#endif
