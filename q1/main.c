#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "course.h"
#include "student.h"

void input()
{
    scanf("%lld %lld %lld", &num_student, &num_labs, &num_courses);

    // taking in CourseList
    for (int i = 0; i < num_courses; i++)
    {
        scanf("%s %f %lld %lld", CourseList[i].courseName, &CourseList[i].interestQ, &CourseList[i].course_max_slots, &CourseList[i].p);
        for (j = 0; j < CourseList[i].p; j++)
        {
            scanf("%lld", &CourseList[i].listp[j]);
        }
        CourseList[i].currentStudentSize = 0;
        CourseList[i].isOn = 0;
        CourseList[i].isExisting = 1;
        CourseList[i].TA_Allocated = -1;
        CourseList[i].isStarting=0;
    }

    // taking in StudentList
    for (int i = 0; i < num_student; i++)
    {
        scanf("%f %lld %lld %lld %f", &StudentList[i].calibre, &StudentList[i].courseIDs[0], &StudentList[i].courseIDs[1], &StudentList[i].courseIDs[2], &StudentList[i].time);
        courseInterest[StudentList[i].courseIDs[0]]++;
        StudentList[i].isLearning = 0;
        StudentList[i].prefPos = 0;
        StudentList[i].existing = 1;
    }

    // taking in LabList
    for (int i = 0; i < num_labs; i++)
    {
        scanf("%s %lld %lld", LabList[i].labName, &LabList[i].noOfTAs, &LabList[i].max);
        memset(LabList[i].courseDone, 0, sizeof(LabList[i].courseDone));
        memset(LabList[i].busy, 0, sizeof(LabList[i].busy));
        LabList[i].busy[i] = 0;
    }
}

void print()
{
    for (int i = 0; i < num_student; i++)
    {
        printf("%f %lld %lld %lld %f\n", StudentList[i].calibre, StudentList[i].courseIDs[StudentList[i].prefPos], StudentList[i].courseIDs[StudentList[i].prefPos + 1], StudentList[i].courseIDs[StudentList[i].prefPos + 2], StudentList[i].time);
    }
    for (int i = 0; i < num_labs; i++)
    {
        printf("%s %lld %lld", LabList[i].labName, LabList[i].noOfTAs, LabList[i].max);
    }
    for (int i = 0; i < num_courses; i++)
    {
        printf("\n");
        printf("%s %f %lld %lld ", CourseList[i].courseName, CourseList[i].interestQ, CourseList[i].course_max_slots, CourseList[i].p);
        for (j = 0; j < CourseList[i].p; j++)
        {
            printf("%lld ", CourseList[i].listp[j]);
        }
        printf("\n");
        CourseList[i].currentStudentSize = 0;
        CourseList[i].isOn = 0;
        CourseList[i].isExisting = 1;
    }
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
    // pthread_mutex_init(&course_TA_Allocation_lock, NULL);
    // pthread_mutex_init(&student_CourseApplication_lock, NULL);
    studentsOver=0;
    coursesOver =0;
    memset(courseInterest, 0, sizeof(courseInterest));
    srand(time(0));
    input();
    printf("\n");

    pthread_create(&sleeper, NULL, sleepingFunc, NULL);
    
// -------------------- -------------------- -------------------- -------------------- -------------------- -------------------- --------------------

    // Creating threads
    for (int l = 0;  l < num_student; l++)
    { 
        pthread_t curr_tid;
        struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
        thread_input->id = l;
        pthread_create(&curr_tid, NULL, StudentStimulater, (void *)(thread_input));
        studThreadArr[l] = curr_tid;
    }

    for (int l = 0; l < num_courses; l++)
    {
     pthread_t curr_tid;
    struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
    thread_input->id = l;
    pthread_create(&curr_tid, NULL, courseStimulater, (void *)(thread_input));
    courseThreadArr[l] = curr_tid;
    }

    
// -------------------- -------------------- -------------------- -------------------- -------------------- -------------------- --------------------
    for (int l = 0; l < num_courses; l++)
    {
        pthread_join(courseThreadArr[l], NULL);
    }
    for (int l = 0; l < num_student; l++)
    {
        pthread_join(studThreadArr[l], NULL);
    }
}