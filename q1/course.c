#include "course.h"
#include <string.h>
#include "utils.h"

void *courseStimulater(void *arg)
{
    
    ll id = ((struct thread_details *)arg)->id;
    turns[id] = 0;

    while (1)
    {   printf("TURN :%d id : %lld\n",turns[id],id);
        course_TA_Allocation(id);       // allocating TA for current tut batch (used lock and conditional lock here coz we r accessing common resource)
        // course_size_Allocation(id);     //  allocating size of current tut batch
        
        // sleep(2);                       // waiting for students to sign up

        // course_start(id);               // starting tut
        
        // sleep(1);                       // tut duration
        
        // tut_over(id);                   // to end tut

        if(course_Removed(id)) break;   // to check if the TAs are over for this course
        turns[id]++;
    }
    return NULL;
}

void position_giver(ll pos)
{
    if (pos == 1)
    {
        strcpy(position, "1st");
    }
    if (pos == 2)
    {
        strcpy(position, "2nd");
    }
    if (pos == 3)
    {
        strcpy(position, "3rd");
    }
    if (pos >= 4)
    {
        char str[100],random[100];
        strcpy(random,"th");
        sprintf(str, "%lld", pos);
        strcat(str, random);
        strcpy(position, str);
    }
}

void course_size_Allocation(ll id)
{   
    printf("course_size_Allocation id : %lld\n",id);
    CourseList[id].D = rand() % CourseList[id].course_max_slots;
    printf("Course %s has been allocated %lld seats\n", CourseList[id].courseName, CourseList[id].D);
}

void course_TA_Allocation(ll id)
{
    int flag=0;
    printf("course_TA_Allocation ID : %lld\n",id);
    for (i = 0; i < CourseList[id].p; i++)
    {
        ll labID = CourseList[id].listp[i];
        printf("hi1 %lld %lld\n",labID,LabList[labID].noOfTAs);
        for (j = 0; j < LabList[labID].noOfTAs; j++)
        {
            printf("hi2");
            pthread_mutex_lock(&course_TA_Allocation_lock);
            printf("hi3");
            if (LabList[labID].courseDone[j] < LabList[labID].max  && LabList[labID].busy[j]==0)
            {
                LabList[labID].courseDone[j]++;
                LabList[labID].busy[j] = 1;

                CourseList[id].labID_Allocated = labID;
                CourseList[id].TA_Allocated = j;
                flag = 1;
                position_giver(j);
                printf("TA %lld from lab %s has been allocated to course %s for his %s TA ship\n", j, LabList[labID].labName, CourseList[id].courseName, position);
            }
            // if(flag == 1){
            //     break;
            // }
            pthread_mutex_unlock(&course_TA_Allocation_lock);
            
        }
    }
    printf("\n");
}

void course_start(ll id)
{
    pthread_mutex_lock(&mutex_lock);
    CourseList[id].isOn = 1;
    printf("Tutorial has started for Course %s with %lld seats filled out of %lld\n", CourseList[id].courseName, CourseList[id].currentStudentSize, CourseList[id].D);
    pthread_cond_signal(&c);  
    pthread_mutex_unlock(&mutex_lock);
}

void tut_over(ll id){
    CourseList[id].isOn = 0;
    LabList[CourseList[id].labID_Allocated].busy[CourseList[id].TA_Allocated] = 1;

    char labName[100];
    strcpy(labName,LabList[CourseList[id].labID_Allocated].labName);
    printf("TA %lld from lab %s has completed the tutorial for course %s\n",CourseList[id].TA_Allocated,labName,CourseList[id].courseName);
}

int course_Removed(ll id)
{
    int TAsOver = 1;
    for (i = 0; i < CourseList[id].p; i++)
    {
        int labID = CourseList[id].listp[i];
        for (j = 0; j < LabList[labID].noOfTAs; j++)
        {
            if (LabList[labID].courseDone[j] < LabList[labID].max)
            {
                TAsOver = 0;
            }
        }
    }

    if (TAsOver)    // if its 1, course removed
    {
        CourseList[id].isExisting = 0;
        return 1;
    }
    return 0;
}