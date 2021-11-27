#include "person.h"
#include "utils.h"

void* personStimulater(void* arg){
    int id = ((struct thread_details *)arg)->id;
    printf("id reached : %d name : %s\n",id,persons[id].name);
    sleep(persons[id].time_reached);
    stadium_reached(id);
    while(1){
        gettingSeat(id);
        break;
    }
    return NULL;
}

void stadium_reached(int id){
    printf(RED"%s has reached the stadium\n"RESET,persons[id].name);
}

void gettingSeat(int id){
    ll wantedZone = persons[id].zone;
    printf("id:%d zone:%lld current:%lld capacity:%lld\n",id,wantedZone,zones[wantedZone].current,zones[wantedZone].capacity);

    if(zones[wantedZone].current < zones[wantedZone].capacity){
        pthread_mutex_lock(&zone_lock[wantedZone]);
        if(zones[wantedZone].current <= zones[wantedZone].capacity){
            zones[wantedZone].current++;
            printf(GREEN"%s has got a seat in zone %c\n"RESET,persons[id].name,persons[id].support);
        }
        pthread_mutex_lock(&zone_lock[wantedZone]);
     }
    else{                                                                      // if zone is full
        pthread_mutex_lock(&mutex_lock[wantedZone]);
        while(zones[wantedZone].current == zones[wantedZone].capacity){
            pthread_cond_wait(&c[wantedZone], &mutex_lock[wantedZone]);
        }
        zones[wantedZone].current++;
        printf("%s has got a seat in zone %c\n",persons[id].name,persons[id].support);
        pthread_mutex_lock(&zone_lock[wantedZone]);
    }
}