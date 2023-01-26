#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parking.h"
#include "hashdb.h"

#ifndef MYSUMLOOKUP
TODO(USING THE SOLUTION sumlookup.c NOT MY CODE)
#else

/*
 * summlookup
 *     find the vehicle with a specified summons number
 * Input:
 *      summ: string of summons number
 * returns:   a pointer to the vehicle
 */
struct vehicle *
sumlookup(char *summ)
{
    unsigned long long summid;
    struct vehicle *cptr; //car ptr
    struct ticket *tptr; //ticket ptr

    if (strtosumid(summ, &summid) != 0) {
        fprintf(stderr, "%s:sumlookup bad summons number:%s\n", argv0, summ);
        return NULL;
    }
    //go to each vehicle 
    for(uint32_t index = 0; index < tabsz; index++ ){
        cptr = *(htable+index);
        
        //loop through each vehicle
        while(cptr != NULL) {
            //check each ticket

            tptr = (cptr->head);
            while (tptr != NULL) {
                //verify same summon number
                
                if (tptr->summons == summid){
                   return cptr; 
                }
                else if (tptr->summons != summid && tptr->next != NULL){
                    tptr = tptr->next;
                }
                else {
                    tptr = tptr->next;
                }
            }
            cptr = cptr->next;
        }
        
    }
    return NULL;
}
#endif
