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

#ifndef MYFREETICKETS
TODO(USING THE SOLUTION freetickets.c NOT MY CODE)
#else

/*
 * freetickets
 *      tears down the database freeing all allocated memory
 *      count the number of tickets freed in the database and
 *      print it out before returning
 */
      // unsigned long cnt = 0UL
       //printf("Total tickets freed: %lu\n", cnt);
 
void
freetickets(void)
{
    unsigned long cnt = 0UL;
    int empty = 1; // set to zero if the database is not empty
    struct vehicle *cptr; // car ptr
    struct ticket *tptr; //ticket ptr 
    struct vehicle *vptrHold; //vehivle ptr temp
    struct ticket *tptrHold; //ticket ptr temp 
    /*
     * walk down each chain
     */
    for(uint32_t index = 0; index < tabsz; index++) {

        cptr = *(htable+index);
        //while there is a car
        while (cptr != NULL) {
            empty = 0;

            //check tickets
            tptr = (cptr->head);
            while(tptr != NULL) {
                //temporary store the pointer so that we can check
                //if there are more tickets at that vehicle
                tptrHold = tptr->next;

                //free the ticket and set to NULL
                
                free(tptr);
                tptr = NULL;
                cnt++;

                //iterate
                tptr = tptrHold; 
            }  
            //temporarily store the pointer so that we can check 
            //if there are more vehicles in the list
            vptrHold = cptr->next;

            //free the vehicle and set to NULL
            free(cptr->state);
            free(cptr->plate);
            free(cptr);
            cptr = NULL;

            //iterate
            cptr = vptrHold;
            
        }
       //clear list 
        *(htable+index) = NULL;
    }
    /*
     * remove all the tickets for all vehicles
     * on this chain
     */

    /*
     * free the vehicle
     */

    /*
     * chain is removed, update the htable entry
     */

    if (empty)
        printf("Empty Database\n");
    printf("Total tickets freed: %lu\n", cnt);
    return;
}
#endif
