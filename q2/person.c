#include "person.h"
#include "utils.h"

void *personStimulater(void *arg)
{
    int id = ((struct thread_details *)arg)->id;
    int wantedZone = ((struct thread_details *)arg)->wantedZone;
    //printf("t=%lld : id reached : %d name : %s wantedZone:%d\n", timer, id, persons[id].name,wantedZone);

    stadium_reached(id, wantedZone);
    gettingSeat(id, wantedZone);
    if(persons[id].gotSeated==2){
        //printf("exiting\n");
        return NULL;
    }
    exiting(id, wantedZone);
    //printf("EXITING FOR ID:%d WANTED:%d\n",id,wantedZone);
    return NULL;
}
 
void stadium_reached(int id, int wantedZone)
{
    //printf("id:%d wantedZone:%d support:%c\n", id,wantedZone, persons[id].c_support);

    pthread_mutex_lock(&person_lock[id]);
    pthread_mutex_lock(&time_lock[persons[id].time_reached]); 
    //printf("id:%d wantedZone:%d sr:%lld\n",id,wantedZone,persons[id].stadiumReached);
    if (persons[id].stadiumReached == 0)
    {
        while (timer != persons[id].time_reached)
        {
            pthread_cond_wait(&timeC[persons[id].time_reached], &time_lock[persons[id].time_reached]);
        }
        persons[id].stadiumReached = 1;
        printf(CYAN "t=%lld : %s has reached the stadium\n" RESET, timer, persons[id].name);
    }

    pthread_mutex_unlock(&time_lock[persons[id].time_reached]);
    pthread_mutex_unlock(&person_lock[id]);
}

void gettingSeat(int id, int wantedZone)
{

    if (wantedZone == 0)
    {
        //printf("0 id : %d wanted zone :%d \n",id,wantedZone);
        pthread_mutex_lock(&zone1_lock[wantedZone]);
        semaphore_wait(zoneHs);
        if (zones[wantedZone].current < zones[wantedZone].capacity)
        {
            //printf("id:%d wantedZone:%d current:%lld capacity:%lld support:%c\n", id, wantedZone, zones[wantedZone].current, zones[wantedZone].capacity, persons[id].c_support);
            if (persons[id].gotSeated == 0)
            {
                if (timer >= persons[id].patience + persons[id].time_reached)
                {
                    persons[id].gotSeated = 2;
                    printf(YELLOW "t=%lld : %s couldn’t get a seat\n" RESET, timer, persons[id].name);
                }
                else
                {
                    persons[id].gotSeated = 1;
                    persons[id].currentZoneSeated = wantedZone;
                    zones[wantedZone].current = zones[wantedZone].current + 1;
                    persons[id].seatedTime = timer;
                    printf(GREEN "t=%lld : %s has got a seat in zone %c\n" RESET, timer, persons[id].name, zones1[wantedZone]);
                }
            }
            else
            {
                semaphore_signal(zoneHs);
            }
        }
        pthread_mutex_unlock(&zone1_lock[wantedZone]);
    }

    if (wantedZone == 1)
    {
        //printf("1 id : %d wanted zone :%d \n",id,wantedZone);
        pthread_mutex_lock(&zone1_lock[wantedZone]);
        semaphore_wait(zoneAs);
        if (zones[wantedZone].current < zones[wantedZone].capacity)
        {
            //printf("id:%d wantedZone:%d current:%lld capacity:%lld support:%c\n", id, wantedZone, zones[wantedZone].current, zones[wantedZone].capacity, persons[id].c_support);
            if (persons[id].gotSeated == 0)
            {
                if (timer >= persons[id].patience + persons[id].time_reached)
                {
                    persons[id].gotSeated = 2;
                    printf(YELLOW "t=%lld : %s couldn’t get a seat\n" RESET, timer, persons[id].name);
                }
                else
                {
                    persons[id].gotSeated = 1;
                    persons[id].currentZoneSeated = wantedZone;
                    zones[wantedZone].current = zones[wantedZone].current + 1;
                    persons[id].seatedTime = timer;
                    printf(GREEN "t=%lld : %s has got a seat in zone %c\n" RESET, timer, persons[id].name, zones1[wantedZone]);
                }
            }
            else
            {
                semaphore_signal(zoneAs);
            }
        }
        pthread_mutex_unlock(&zone1_lock[wantedZone]);
    }

    if (wantedZone == 2)
    {
        //printf("2 id : %d wanted zone :%d \n",id,wantedZone);
        pthread_mutex_lock(&zone1_lock[wantedZone]);
        semaphore_wait(zoneNs);
        if (zones[wantedZone].current < zones[wantedZone].capacity)
        {
            //printf("id:%d wantedZone:%d current:%lld capacity:%lld support:%c\n", id, wantedZone, zones[wantedZone].current, zones[wantedZone].capacity, persons[id].c_support);
            if (persons[id].gotSeated == 0)
            {
                if (timer >= persons[id].patience + persons[id].time_reached)
                {
                    persons[id].gotSeated = 2;
                    printf(YELLOW "t=%lld : %s couldn’t get a seat\n" RESET, timer, persons[id].name);
                }
                else
                {
                    persons[id].gotSeated = 1;
                    persons[id].currentZoneSeated = wantedZone;
                    zones[wantedZone].current = zones[wantedZone].current + 1;
                    persons[id].seatedTime = timer;
                    printf(GREEN "t=%lld : %s has got a seat in zone %c\n" RESET, timer, persons[id].name, zones1[wantedZone]);
                }
            }
            else
            {
                semaphore_signal(zoneNs);
            }
        }
        pthread_mutex_unlock(&zone1_lock[wantedZone]);
    }
}

void exiting(int id, int wantedZone)
{
    ll support = persons[id].support;
    ll zoneSeated = persons[id].currentZoneSeated;
    if (persons[id].support == 0 && persons[id].currentZoneSeated == wantedZone)
    {
        while (1)
        {
            if (timer >= persons[id].seatedTime + X)
            {
                persons[id].timeWatched = timer - persons[id].seatedTime;
                printf(RED "t=%lld : %s watched the match for %lld seconds and is leaving\n" RESET, timer, persons[id].name, persons[id].timeWatched);
                zones[support].current--;       
                break;
            }

            if (zones[1].score >= persons[id].enragedScore)
            {
                printf(RED "t=%lld : %s is leaving due to the bad defensive performance of his team\n" RESET, timer, persons[id].name);
                zones[support].current--; 
                break;
            }
        }
        semaphore_signal(zoneHs);
    }
    if (persons[id].support == 1 && persons[id].currentZoneSeated == wantedZone)
    {
        while (1)
        {
            if (timer >= persons[id].seatedTime + X)
            {
                persons[id].timeWatched = timer - persons[id].seatedTime;
                printf(RED "t=%lld : %s watched the match for %lld seconds and is leaving\n" RESET, timer, persons[id].name, persons[id].timeWatched);
                zones[support].current--;           
                break;
            }

            if (zones[0].score >= persons[id].enragedScore)
            {
                printf(RED "t=%lld : %s is leaving due to the bad defensive performance of his team\n" RESET, timer, persons[id].name);
                zones[support].current--;
                break;
            }
        }
        semaphore_signal(zoneAs);
    }
    if (persons[id].support == 2 && persons[id].currentZoneSeated == wantedZone)
    {
        while (1)
        {
            if (timer >= persons[id].seatedTime + X)
            {
                persons[id].timeWatched = timer - persons[id].seatedTime;
                printf(RED "t=%lld : %s watched the match for %lld seconds and is leaving\n" RESET, timer, persons[id].name, persons[id].timeWatched);
                zones[support].current--;
                break;
            }
        }
        semaphore_signal(zoneNs);
    }
}

