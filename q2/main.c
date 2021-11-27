#include "utils.h"
#include "person.h"
#include "goals.h"

void input()
{
    scanf("%lld %lld %lld\n", &zones[0].capacity, &zones[1].capacity, &zones[2].capacity);
    zones[0].current = 0, zones[1].current = 0, zones[2].current = 0;
    zones[0].score = 0, zones[1].score = 0, zones[2].score = 0;
    zones[0].pos = 0, zones[1].pos = 0, zones[2].pos = 0;

    scanf("%lld\n", &X);
    scanf("%lld\n", &num_groups);

    int c = 0;
    for (int i = 0; i < num_groups; i++)
    {
        scanf("%lld", &groups[i].size);
        for (int j = 0; j < groups[i].size; j++)
        {
            scanf("%s %c %lld %lld %lld\n", persons[c].name, &persons[c].c_support, &persons[c].time_reached, &persons[c].patience, &persons[c].enragedScore);
            persons[c].groupNo = i;
            persons[c].gotSeated = 0;
            persons[c].stadiumReached = 0;
            if (persons[c].c_support == 'H')
            {
                persons[c].support = 0;
            }
            else if (persons[c].c_support == 'A')
            {
                persons[c].support = 1;
            }
            else if (persons[c].c_support == 'N')
            {
                persons[c].support = 2;
            }
            c++;
        }
    }

    scanf("%lld\n", &G);
    for (int i = 0; i < G; i++)
    {
        scanf("%s %lld %f", goals[i].team, &goals[i].time, &goals[i].probability);
        goals[i].probability = goals[i].probability * 100;
        if (strcmp(goals[i].team, "H") == 0)
        {
            goals[i].zone = 0;
        }
        else if (strcmp(goals[i].team, "A") == 0)
        {
            goals[i].zone = 1;
        }
    }

    zones1[0]='H';
    zones1[1]='A';
    zones1[2]='N';
}

void *sleepingFunc(void *arg)
{
    timer = 0;
    while (1) 
    {
        pthread_mutex_lock(&time_lock[timer + 1]);
        sleep(1);
        timer++;
        pthread_cond_broadcast(&timeC[timer]);
        pthread_mutex_lock(&time_lock[timer + 1]);
    }
}

int main()
{
    srand(time(0));
    input();

    // creating thread for timer
    pthread_create(&sleeper, NULL, sleepingFunc, NULL);

    // for H
    zoneAs = make_semaphore(zones[1].capacity);
    zoneHs = make_semaphore(zones[0].capacity);
    zoneNs = make_semaphore(zones[2].capacity);


    // sem_t zone_empty[3];
    // sem_t zone_full[3];
    // sem_t zone_mutex[3];
//     sem_init(&zone_empty[0], 0, zones[0].capacity); // CAPACITY are empty
//    // sem_init(&zone_full[0], 0, 0);                  // 0 are full
//     sem_init(&zone_mutex[0], 0, 1);                 // mutex

//     // for A
//     sem_init(&zone_empty[1], 0, zones[1].capacity); // CAPACITY are empty
//     //sem_init(&zone_full[1], 0, 0);                  // 0 are full
//     sem_init(&zone_mutex[1], 0, 1);                 // mutex

//     // for N
//     sem_init(&zone_empty[2], 0, zones[2].capacity); // CAPACITY are empty
//     //sem_init(&zone_full[2], 0, 0);                  // 0 are full
//     sem_init(&zone_mutex[2], 0, 1);                 // mutex

    for(int i = 0;i<100;i++){
        pthread_mutex_init(&person_lock[i],NULL);
    }
    for(int i = 0;i<2;i++){
        pthread_mutex_init(&zone_lock[i],NULL);
    }
    for(int i = 0;i<2;i++){
        pthread_mutex_init(&zone1_lock[i],NULL);
    }

    
    //creating thread for spectators
    int c = 0,id=0;
    for (int l = 0; l < num_groups; l++)
    {
        for (int j = 0; j < groups[l].size; j++)
        {
            if (persons[id].support == 0)
            {   
                pthread_t curr_tid1;
                struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
                thread_input->id = id;
                thread_input->wantedZone = 0;
                pthread_create(&curr_tid1, NULL, personStimulater, (void *)(thread_input));
                personThreadArr[c] = curr_tid1;
                c++;

               
                pthread_t curr_tid2;
                struct thread_details *thread_input2 = (struct thread_details *)(malloc(sizeof(struct thread_details)));
                thread_input2->id = id;
                thread_input2->wantedZone = 2;
                pthread_create(&curr_tid2, NULL, personStimulater, (void *)(thread_input2));
                personThreadArr[c] = curr_tid2;
                c++;
            }

            if (persons[id].support == 1)
            {
                
                pthread_t curr_tid1;
                struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
                thread_input->id = id;
                thread_input->wantedZone = 1;
                pthread_create(&curr_tid1, NULL, personStimulater, (void *)(thread_input));
                personThreadArr[c] = curr_tid1;
                c++;
            }

            if (persons[id].support == 2)
            {
                
                //printf("id:%d 0 support:%c\n", id,persons[id].c_support);
                pthread_t curr_tid1;
                struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
                thread_input->id = id;
                thread_input->wantedZone = 0;
                pthread_create(&curr_tid1, NULL, personStimulater, (void *)(thread_input));
                personThreadArr[c] = curr_tid1;
                c++;

                //printf("id:%d 1 support:%c\n", id,persons[id].c_support);
                pthread_t curr_tid2;
                struct thread_details *thread_input2 = (struct thread_details *)(malloc(sizeof(struct thread_details)));
                thread_input2->id = id;
                thread_input2->wantedZone = 1;
                pthread_create(&curr_tid2, NULL, personStimulater, (void *)(thread_input2));
                personThreadArr[c] = curr_tid2;
                c++;

                //printf("id:%d 2 support:%c\n", id,persons[id].c_support);
                pthread_t curr_tid3;
                struct thread_details *thread_input3 = (struct thread_details *)(malloc(sizeof(struct thread_details)));
                thread_input3->id = id;
                thread_input3->wantedZone = 2;
                pthread_create(&curr_tid3, NULL, personStimulater, (void *)(thread_input3));
                personThreadArr[c] = curr_tid3;
                c++;
            }
            id++;
        }
    }

    // creating threads for goals
    for (int l = 0; l < G; l++)
    {
        pthread_t curr_tid;
        struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
        thread_input->id = l;
        pthread_create(&curr_tid, NULL, goalStimulater, (void *)(thread_input));
        goalThreadArr[l] = curr_tid;
    }

    // ------------------------------------------------------------ -------------------- -------------------- --------------------

    for (int l = 0; l < c; l++)
    {
        pthread_join(personThreadArr[l], NULL);
    }

    for (int l = 0; l < G; l++)
    {
        pthread_join(goalThreadArr[l], NULL);
    }
}
