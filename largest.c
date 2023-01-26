#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "parking.h"

#ifndef MYLARGEST
TODO(USING THE SOLUTION largest.c NOT MY CODE)
#else

/*
 * largest
 *     find the vehicle with the largest number of
 *     tickets and largest fine in database
 *     and print it
 */
void
largest(void)
{
    struct vehicle *count = NULL; // vehicle with largest number of tickets
    struct vehicle *fine = NULL;  // vehicle with largest total fine

    unsigned int largestFine = 0;
    unsigned int greatestNumOfTix = 0;
    struct vehicle *cptr; //car ptr
   
    //go to each vehicle
    for(uint32_t index = 0; index <tabsz; index++){
        cptr = *(htable+index);
        
        //loop through each vehicle
        while (cptr != NULL) {

            //if tot_fine is same or larger
            //update variable ptr to that vehicle
            if (cptr->tot_fine >= largestFine) {
                largestFine = cptr->tot_fine;
                fine = cptr;
            }
            if (cptr->cnt_ticket >= greatestNumOfTix) {
                greatestNumOfTix = cptr->cnt_ticket;
                count = cptr;
            }
        cptr = cptr->next;
        }
    }


    if ((count == NULL) || (fine == NULL)) {
        printf("Empty database\n");
        return;
    }
    printf("Most tickets Plate: %s, State: %s tickets: %u, total fine: $%u\n",
            count->plate, count->state, count->cnt_ticket, count->tot_fine);
    printf("Largest fine Plate: %s, State: %s tickets: %u, total fine: $%u\n",
            fine->plate, fine->state, fine->cnt_ticket, fine->tot_fine);
    return;
}
#endif
