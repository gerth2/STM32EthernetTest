
#include "set.h"

void makeNewItem(set_member_t ** insertPoint,  void * newContent, int32_t id){
    *insertPoint = (set_member_t *) malloc(sizeof(set_member_t));
    (*insertPoint)->id = id;
    (*insertPoint)->content = newContent;
}

void removeItem(set_member_t ** removeMember){
    free(*removeMember);
}


// Adds new item to the set.
// Returns 0 on success, -1 for id already in set;
uint8_t set_add(set_t * set, void * newContent, int32_t id){

    set_member_t ** insertPoint = &(set->root);

    while(true){
        if((*insertPoint) == NULL){
            makeNewItem(insertPoint, newContent, id);
            set->size++;
            return 0;
        } else {

            if(id < (*insertPoint)->id){
                insertPoint = &((*insertPoint)->smaller);
            } else if (id > (*insertPoint)->id) {
                insertPoint = &((*insertPoint)->larger);
            } else {
                return -1; //ID already exists
            }
        }
    }

}

void set_init(set_t * set){
    set->root = NULL;
    set->size = 0;
}

uint8_t set_remove(set_t * set, int32_t id){

    set_member_t ** curMember = &(set->root);

    while(true){
        if(*curMember == NULL){
            return -1; // ID not found
        } else {
            if(id < (*curMember)->id){
                curMember = &((*curMember)->smaller);
            } else if (id > (*curMember)->id) {
                curMember = &((*curMember)->larger);
            } else {
                //ID Found, remove it
                removeItem(curMember);
                set->size--;
                return 0;
            }
        }
    }
    

}

void * set_get(set_t * set, int32_t id){
        set_member_t ** curMember = &(set->root);

    while(true){
        if(*curMember == NULL){
            return NULL; // ID not found
        } else {
            if(id < (*curMember)->id){
                curMember = &((*curMember)->smaller);
            } else if (id > (*curMember)->id) {
                curMember = &((*curMember)->larger);
            } else {
                //ID Found, return its content
                return ((*curMember)->content);
            }
        }
    }
}
