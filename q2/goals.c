#include "goals.h"
#include "utils.h"
 


void position_giver(ll id)
{
    pthread_mutex_lock(&position_lock[goals[id].zone]);
    zones[goals[id].zone].pos = zones[goals[id].zone].pos+1;
    ll pos = zones[goals[id].zone].pos;
    if (pos == 1)
    {
        strcpy(goals[id].position, "1st");
    }
    if (pos == 2)
    {
        strcpy(goals[id].position, "2nd"); 
    }
    if (pos == 3)
    {
        strcpy(goals[id].position, "3rd");   
    }
    if (pos >= 4)
    {   
        char str[100], random[100];
        strcpy(random, "th");
        sprintf(str, "%lld", pos);
        strcat(str, random);
        strcpy(goals[id].position, str);
    }
    pthread_mutex_unlock(&position_lock[goals[id].zone]);
}

void *goalStimulater(void *arg)
{
    int id = ((struct thread_details *)arg)->id;
    float r = (rand() % 100);
    
    pthread_mutex_lock(&time_lock[goals[id].time]);
    while (timer != goals[id].time)
    {
        pthread_cond_wait(&timeC[goals[id].time], &time_lock[goals[id].time]);
    }
    position_giver(id);
    if (r < goals[id].probability)
    {
        printf(MAGENTA "t=%lld : Team %s has scored their %s goal\n" RESET, timer, goals[id].team, goals[id].position);
        zones[goals[id].zone].score++;
    }
    else
    {
        printf(MAGENTA "t=%lld : Team %s missed the chance to score their %s goal\n" RESET, timer, goals[id].team, goals[id].position);
    }
    pthread_mutex_unlock(&time_lock[goals[id].time]);

    return NULL;
}