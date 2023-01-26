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

#ifndef MYINSTICKET
TODO(USING THE SOLUTION insticket.c NOT MY CODE)
#else

/*
 * insertticket
 *      add a ticket (summons) to the database
 *
 *      YOU CANNOT USE CALLOC(), only malloc() is allowed.
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add 
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 *  fineTab pointer fine table. maps code number to text description & fine cost 
 *          you use this table to get the amount of the fine when calculating
 *          the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *  summ    summons id string to be added
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string         
 *          Conversion function is supplied. use example:
 *               unsigned long long summid;
 *               if (strtosumid(summ, &summid, argv) != 0)
 *                  error handling
 *               new_ticket->summons = summid;
 *  plate   plate id string to be added
 *  state   state id string to be added
 *  date    date of summons string
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string
 *          The encoding into a number uses Linux time format.
 *          Conversion function is supplied. use example:
 *              time_t dateval;
 *              if (strtoDate(date, &dateval, argv) != 0)
 *                  error handling
 *              new_ticket->date = dateval;
 *  code    summons code integer value used an an index into the fines table
 *
 * returns 0 if ok -1 for all errors
 */

int
insertticket(char *summ, char *plate, char *state, char *date, int code)
{
    unsigned long long summid;
    time_t dateval;

    if (strtosumid(summ, &summid) != 0) {
        return -1;
    }
    if (strtoDate(date, &dateval) != 0) {
        return -1;
    }

    /*when both state and plate match*/
    //if the vehicle exists
    if (vehiclelookup(plate, state) != NULL) {

        struct vehicle *chain = vehiclelookup(plate, state); //finds vehicle
        struct ticket *tix = chain->head; //look at ticket in each vehicle
        struct ticket *new_tix = malloc(sizeof(*new_tix));
        struct ticket *prev_tix = NULL;

        //check if its null
        if (new_tix == NULL) {
            fprintf(stderr, "%s: unable to allocate ticket for summons %s\n",argv0, summ);
            free(new_tix);
            return -1;
        }
        //adding values to the ticket
        new_tix->summons = summid;
        new_tix->date = dateval;
        new_tix->code = code;
        new_tix->next = NULL;

        while (tix != NULL) {
            /*check for duplicates*/
            if (tix->summons == summid) {
                fprintf(stderr, "%s: duplicate summons %llu\n", argv0, summid);
                free(new_tix);
                return -1;
            }
            prev_tix = tix;
            tix = tix->next;
        }

        prev_tix->next = new_tix;

        //update tot_fine in vehicle struct
        int Fine = fineTab[code].fine;
        chain->tot_fine+= Fine; 
        chain->cnt_ticket+=1; //update member cnt_ticket in vehicle struct      
    }

    else {

        uint32_t hashval;
        struct vehicle *chain;
        hashval = hash(plate) % tabsz;
        chain = *(htable + hashval);

        /*when vehicle was not found,add to the beginning of the list*/
        struct vehicle *new_vehicle = malloc(sizeof(*new_vehicle));

        //check if null
        if (new_vehicle == NULL){
            fprintf(stderr, "%s: unable to allocate vehicle for summons %s\n",
            argv0, summ);
            free(new_vehicle);
            return -1;
        }

        new_vehicle->state =strdup(state); //allocate memory

        //check if null
        if (new_vehicle->state == NULL) {
            fprintf(stderr, "%s: unable to allocate state for summons %s\n", argv0, summ);
            free(new_vehicle);
            return -1;
        }

        new_vehicle->plate = strdup(plate); //allocate memory

        //check if null
        if (new_vehicle->plate == NULL) {
            fprintf(stderr, "%s: unable to allocate state for summons %s\n", argv0, summ);
            free(new_vehicle);
            return -1;
        }

        struct ticket *new_ticket = malloc(sizeof(*new_ticket));
        //check for null
        if (new_ticket == NULL ) {
            fprintf(stderr, "%s: unable to allocate ticket for summons %s\n", argv0, summ);
            free(new_ticket);
            return -1;
        }
        //add values
        new_ticket->summons = summid;
        new_ticket->date = dateval;
        new_ticket->code = code;
        new_ticket->next = NULL;

        //add values to the vehicle
        new_vehicle->tot_fine = fineTab[code].fine;
        new_vehicle->cnt_ticket = 1;
        
        
        *(htable + hashval) = new_vehicle; //put in front of list
        new_vehicle->next = chain; //add everything back to position

        new_vehicle->head = new_ticket;

    }

     /*
     * either add ticket to chain or add vehicle and then the ticket
     */
    
      return 0;
}
#endif
