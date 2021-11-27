#include "utils.h"
#include "person.h"

void input()
{
    scanf("%lld %lld %lld", &zones[0].capacity, &zones[1].capacity, &zones[2].capacity);
    zones[0].current = 0,zones[1].current = 0,zones[2].current = 0;
    scanf("%lld", &X);
    scanf("%lld", &num_groups);
    int c=0;
    for (int i = 0; i < num_groups; i++)
    {
        scanf("%lld", &groups[i].size);
        for (int j = 0; j < groups[i].size; j++)
        {
            scanf("%s %c %lld %lld %lld", persons[c].name, &persons[c].support, &persons[c].time_reached, &persons[c].patience, &persons[c].enragedScore);
            persons[c].groupNo = i;
            if(persons[c].support=='H'){
                persons[c].zone=0;
            }else if(persons[c].support=='A'){
                persons[c].zone=1;
            }else if(persons[c].support=='N'){
                persons[c].zone=2;
            }

            c++;
        }  
    }
    scanf("%lld", &G);
    for (int i = 0; i < G; i++)
    {
        scanf("%s %lld %f", goals[i].team, &goals[i].time, &goals[i].probability);
    }
}

int main()
{
    input();

    int c =0;
    for (int l = 0; l < num_groups; l++)
    {
        for (int j = 0; j < groups[l].size; j++)
        {
            pthread_t curr_tid;
            struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
            thread_input->id = c;
            pthread_create(&curr_tid, NULL, personStimulater, (void *)(thread_input));
            personThreadArr[l] = curr_tid;
            c++;
        }
    }

// ------------------------------------------------------------ -------------------- -------------------- --------------------

    for (int l = 0; l < num_groups; l++)
    {
        for (int j = 0; j < groups[l].size; j++)
        {
        pthread_join(personThreadArr[l], NULL);
        }
    }

}
