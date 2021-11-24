#include "student.h"
#include "utils.h"
#include "course.h"

void *StudentStimulater(void *arg)
{
    int id = ((struct thread_details *)arg)->id;
    int time = StudentList[id].time;
    sleep(time);
    student_PreferenceFilling(id); // filling preferences at alloted time
    while (1)
    {
        student_CourseApplication(id); // applying for each course per preference   (locks used here)
        sleep(2);
        student_CourseSelection(id); // selecting course based on prob
        if (student_Removed(id)){
            printf("STUD %d 1REMOVED\n",id);
            break;
        }
            
    }
    return NULL;
}

int student_Removed(ll id)
{
    if (StudentList[id].existing == 0)
    {
        return 1;
    }
    return 0;
}

void student_PreferenceFilling(ll id)
{
    StudentList[id].existing = 1; // student has filled preferences, and hence it is in simulation
    printf("Student %lld has filled in preferences for course registration\n", id);
}

void student_CourseApplication(ll id)
{
    ll courseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos];
    ll nextCourseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos + 1];
    ll pos = StudentList[id].prefPos;
    StudentList[id].isLearning = 0;
L2:
    if (CourseList[courseWantedID].isExisting == 1) // to check if course is existing
    {
        if (CourseList[courseWantedID].isOn == 0) // to confirm the tutorial hasnt started
        {
            pthread_mutex_lock(&student_CourseApplication_lock);
            if (CourseList[courseWantedID].currentStudentSize < CourseList[courseWantedID].D) // to see if capacity is not filled yet
            {
                CourseList[courseWantedID].currentStudentSize++;
                StudentList[id].isLearning = 1;
                printf("Student %lld has been allocated a seat in course %s\n", id, CourseList[courseWantedID].courseName);
            }
            pthread_mutex_unlock(&student_CourseApplication_lock);
        }
        else // if tut is ongoing at the moment when student applies
        {
            pthread_mutex_lock(&mutex_lock2[courseWantedID]);

            while (CourseList[courseWantedID].isOn == 1)
                pthread_cond_wait(&c2[courseWantedID], &mutex_lock2[courseWantedID]);

            printf("GOTTTT signal for course StudID : %lld prefPos : %lld\n", id, StudentList[id].prefPos);

            if (course_Removed(courseWantedID))
            {
                printf("Course Removed : %lld for studID %lld\n", courseWantedID, id);
                goto L1;
            }

            if (CourseList[courseWantedID].isExisting == 1) // to check if course is existing
            {
                if (CourseList[courseWantedID].isOn == 0) // to confirm the tutorial hasnt started
                {
                    pthread_mutex_lock(&student_CourseApplication_lock);
                    if (CourseList[courseWantedID].currentStudentSize < CourseList[courseWantedID].D) // to see if capacity is not filled yet
                    {
                        CourseList[courseWantedID].currentStudentSize++;
                        StudentList[id].isLearning = 1;
                        printf("CStudent %lld has been allocated a seat in course %s\n", id, CourseList[courseWantedID].courseName);
                    }
                    pthread_mutex_unlock(&student_CourseApplication_lock);
                }
            }
            pthread_mutex_unlock(&mutex_lock2[courseWantedID]);
        }
    }
    else // if course is not existing
    {
    L1:
        if (StudentList[id].prefPos == 2) // to check if preferences are left
        {
            StudentList[id].isLearning = 1;
            StudentList[id].existing = 0; // since no preferences left, student is out of simulation
            studentsOver++;
            printf("MStudent %lld couldn’t get any of his preferred courses\n", id);
        }
        else
        {
            printf("MStudent %lld has changed current preference from %s (priority %lld) to %s (priority %lld)\n", id, CourseList[courseWantedID].courseName, pos, CourseList[nextCourseWantedID].courseName, pos + 1);
            StudentList[id].prefPos++;
            courseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos];
            nextCourseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos + 1];
            StudentList[id].isLearning = 0;
            goto L2;
        }
    }

    if (StudentList[id].isLearning == 0)
    {
        goto L2;
    }
}

void student_CourseSelection(ll id) // after Tut is over, the student selects course based on probability
{
    ll courseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos];

    pthread_mutex_lock(&mutex_lock[courseWantedID]);
    while (CourseList[courseWantedID].isOn == 1)
        pthread_cond_wait(&c[courseWantedID], &mutex_lock[courseWantedID]);
    pthread_mutex_unlock(&mutex_lock[courseWantedID]);

    float prob;
    ll nextCourseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos + 1];
    ll pos = StudentList[id].prefPos;
    printf("StudID : %lld prefPos : %lld CourseWanted Id : %lld next : %lld \n", id, pos, courseWantedID, nextCourseWantedID);
    prob = StudentList[id].calibre * CourseList[courseWantedID].interestQ;
    printf("prob :%f id : %lld\n", prob, id);
    if (prob > 0.5) // to check if interest is enuf
    {
        studentsOver++;
        StudentList[id].existing = 0; // student is out of simulation since he has selected his prefered course
        printf("Student %lld has selected course %s permanently\n", id, CourseList[courseWantedID].courseName);
    }
    else // if not selected, preferences updated
    {
        if (StudentList[id].existing == 1)
        {
            if (StudentList[id].prefPos == 2) // to check if preferences are left
            {
                StudentList[id].existing = 0; // since no preferences left, student is out of simulation
                studentsOver++;
                printf("Student %lld couldn’t get any of his preferred courses\n", id);
            }
            else
            {

                printf("Student %lld has changed current preference from %s (priority %lld) to %s (priority %lld)\n", id, CourseList[courseWantedID].courseName, pos, CourseList[nextCourseWantedID].courseName, pos + 1);
                StudentList[id].prefPos++;
            }
        }
    }
}
