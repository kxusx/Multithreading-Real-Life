#include "course.h"
#include <string.h>
#include "utils.h"

void *courseStimulater(void *arg)
{
    ll id = ((struct thread_details *)arg)->id;
    turns[id] = 0;
    sleep(1);
    while (1)
    {
        //printf("TURN :%d id : %lld\n", turns[id], id);

        course_TA_Allocation(id); // allocating TA for current tut batch (used lock and conditional lock here coz we r accessing common resource)
        if (CourseList[id].TA_Allocated == -1)
        {
            //printf("bye id : %lld\n", id);
            coursesOver++;
            break;
        }
        course_size_Allocation(id); //  allocating size of current tut batch

        sleep(3); // waiting for students to sign up

        course_start(id); // starting tut

        sleep(1); // tut duration

        tut_over(id); // to end tut
        //printf("Students over : %lld\n", studentsOver);
        if (course_Removed(id))
        {
            pthread_mutex_lock(&mutex_lock2[id]);
            coursesOver++;
            printf(RED"Course %s doesn’t have any TA’s eligible and is removed from course offerings\n"RESET, CourseList[id].courseName);
            CourseList[id].isStarting=0;
            pthread_cond_broadcast(&c2[id]);
            pthread_mutex_unlock(&mutex_lock2[id]);
            break; // to check if the TAs are over for this course
        }
        if (studentsOver == num_student)
        {
            //printf("breaking over students are over\n");
            break;
        }

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
        char str[100], random[100];
        strcpy(random, "th");
        sprintf(str, "%lld", pos);
        strcat(str, random);
        strcpy(position, str);
    }
}

void course_size_Allocation(ll id)
{
    //printf("ENTERED course_size_Allocation for id : %lld\n",id);
    pthread_mutex_lock(&mutex_lock2[id]);
    CourseList[id].isOn = 1;
    CourseList[id].isStarting = 1;
    
    CourseList[id].D = (rand() % CourseList[id].course_max_slots) + 1;
    printf(BLUE"Course %s has been allocated %lld seats\n"RESET, CourseList[id].courseName, CourseList[id].D);
    pthread_cond_broadcast(&c2[id]);
    pthread_mutex_unlock(&mutex_lock2[id]);
    //printf("EXITED course_size_Allocation for id : %lld\n",id);
}

void course_TA_Allocation(ll id)
{
    
    //printf("ENTERED course_TA_Allocation for id : %lld\n",id);
    for (int f = 0; f < CourseList[id].p; f++)
    {
        ll labID = CourseList[id].listp[f];
        for (ll j = 0; j < LabList[labID].noOfTAs; j++)
        {
            pthread_mutex_lock(&lab_lock[id]);
            if (CourseList[id].TA_Allocated == -1)
            { //printf("H1 ");
                if (LabList[labID].courseDone[j] < LabList[labID].max)
                { //printf("H2 ");
                    //printf("LabList[%lld].busy[%lld]:%lld",labID,j,LabList[labID].busy[j]);
                    if (LabList[labID].busy[j] == 0)
                    {
                        //printf("H3 \n");
                        LabList[labID].courseDone[j]++;
                        LabList[labID].busy[j] = 1;

                        CourseList[id].labID_Allocated = labID;
                        CourseList[id].TA_Allocated = j;
                        //printf("J : %lld",j);
                        position_giver(LabList[labID].courseDone[j]);
                        printf(YELLOW"TA %lld from lab %s has been allocated to course %s for his %s TA ship\n"RESET, j, LabList[labID].labName, CourseList[id].courseName, position);
                    }
                }
            }

            pthread_mutex_unlock(&lab_lock[id]);
        }
    }
   // printf("EXITED course_TA_Allocation for id : %lld\n",id);
    
}

void course_start(ll id)
{   
    CourseList[id].isStarting = 0;
    int flag=0;
    //printf("ENTERED course_start for id : %lld\n",id);
    //printf("hhh id : %lld over:%lld cinterest : %lld\n", id, studentsOver, courseInterest[id] );
    //while (CourseList[id].currentStudentSize == 0)
    // {
    //     sleep(1);
        //printf("hhh id : %lld over:%lld cinterest : %lld\n", id, studentsOver, courseInterest[id] );
        if (studentsOver >= num_student || course_Removed(id) || courseInterest[id]==0)
        {
            flag=1;
            //break;
        }
    //}
    
    CourseList[id].isOn = 1;
    printf(GREEN"Tutorial has started for Course %s with %lld seats filled out of %lld\n"RESET, CourseList[id].courseName, CourseList[id].currentStudentSize, CourseList[id].D);
    //printf("EXITED course_start for id : %lld\n",id);
}

void tut_over(ll id)
{
    //printf("ENTERED tut_over for id : %lld\n",id);
    pthread_mutex_lock(&mutex_lock[id]);
    char labName[100];
    strcpy(labName, LabList[CourseList[id].labID_Allocated].labName);
    printf(MAGENTA"TA %lld from lab %s has completed the tutorial for course %s\n"RESET, CourseList[id].TA_Allocated, labName, CourseList[id].courseName);
    LabList[CourseList[id].labID_Allocated].busy[CourseList[id].TA_Allocated] = 0;
    CourseList[id].isOn = 0;
    CourseList[id].TA_Allocated = -1;
    CourseList[id].currentStudentSize = 0;
    pthread_cond_broadcast(&c[id]);
    pthread_mutex_unlock(&mutex_lock[id]);
    //printf("EXITED tut_over for id : %lld\n",id);
}

int course_Removed(ll id)
{

    int TAsOver = 1;
    for (int l = 0; l < CourseList[id].p; l++)
    {
        int labID = CourseList[id].listp[l];
        for (int f = 0; f < LabList[labID].noOfTAs; f++)
        {
            if (LabList[labID].courseDone[f] < LabList[labID].max)
            {
                TAsOver = 0;
            }
        }
    }

    if (TAsOver) // if its 1, course removed
    {
        CourseList[id].isExisting = 0;
        CourseList[id].isOn = 0;
       
        return 1;
    }
    return 0;
}