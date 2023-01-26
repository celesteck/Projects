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

#ifndef MYDELTICKET
TODO(USING THE SOLUTION delticket.c NOT MY CODE)
#else

/*
 * delticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a long long int using
 *      using strtosumid. This long long int is how the summons number
 *      is stored in the database in struct ticket.
 *      After being converted you can use it find a summons id match
 *      in the linked list of tickets attached to the strcut vehicle.      
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string (summid in the example)
 *      example:
 *        unsigned long long summid
 *        if (strtosumid(summ, &summid) != 0)
 *           return -1;
 *        now search the ticket chain to find that summons
 *        struct ticket *tptr;
 *        if (tptr->summons == summid)
 *          found it
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to delete all space that malloc()'d 
 *
 * Inputs:
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *
 * returns 0 if ok -1 if not found or error
 */

int
delticket(char *plate, char *state, char *summ)
{
    unsigned long long summid;

    /*
     * convert the summons string to a number
     */
    if (strtosumid(summ, &summid) != 0)
        return -1;

    /*
     * first find the vehicle 
     */  
     
     uint32_t hashval;  
     struct vehicle *vptr; //vehicle pointer
     struct ticket *tptr; //ticket pointer
     hashval = hash(plate) % tabsz;
     vptr = *(htable + hashval);
     struct ticket *tptrHold = NULL; //holds the previous ticket
     struct vehicle *vptrHold = NULL;; //holds the previous vehicle

    //vehicle is not found
     
     //goes thru the vehicles
     while (vptr != NULL) {
        
        //make sure its the right vehicle 
        if (strcmp(plate, vptr->plate) == 0 && strcmp(state, vptr->state) == 0){
            //go to ticket loop
            break;
       }
        
        //iterate
        vptrHold = vptr;
        vptr = vptr->next; 
     }
     //vehicle not found
     if (vptr == NULL) {
        return -1;
     }
     tptr = vptr->head;
     //goes through the tickets
     while (tptr != NULL) {
        
        //make sure its the right ticket
        if (summid == tptr->summons) {
            break;
        }

        //iterate
        tptrHold = tptr;
        tptr = tptr->next;
     }

     //ticket not found
     if(tptr == NULL){
        return -1;
     }

    //since we have found the right vehicle and ticket, delete the ticket

    //if it is the first ticket
    if (tptrHold == NULL) {
        
        int code = tptr->code;
        //change the first ticket to the next
        vptr->head = tptr->next;
        free(tptr);
        tptr = NULL;

        //decrement ticket count and total fine
        vptr->cnt_ticket -=1;
        vptr->tot_fine -= fineTab[code].fine;

    }
   
    //if its in the middle or last
    else {

        //change the ticket to the next
        tptrHold->next = tptr->next;

        int code = tptr->code;

        free(tptr);
        tptr = NULL;

        //decrement ticket count and total fine
        vptr->cnt_ticket -=1;
        vptr->tot_fine -= fineTab[code].fine;
        
    }

    //now, if the vehicle has no tickets and no fine, remove it
    if (vptr->cnt_ticket == 0 && vptr->tot_fine == 0) {

        //if its the first vehical 
        if (vptrHold == NULL) {
            
            //change the first vehicle to the next
            *(htable + hashval) = vptr->next;
            free(vptr->state);
            free(vptr->plate);
            free(vptr);
            vptr = NULL;

            return 0;

        }
        // if its the middle or the last
        else {

        vptrHold->next = vptr->next;

        free(vptr->state);
        free(vptr->plate);
        free(vptr);
        vptr = NULL;
        return 0;
        }

    } 
    return 0;
}
#endif
