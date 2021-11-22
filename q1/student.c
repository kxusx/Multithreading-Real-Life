#include "student.h"
#include "utils.h"

void *StudentStimulater(void *arg)
{
    int id = ((struct thread_details *)arg)->id;
    int time = StudentList[id].time;
    sleep(time);
    student_PreferenceFilling(id); // filling preferences at alloted time
    while (1)
    {
        student_CourseApplication(id); // applying for each course per preference   (locks used here)
        student_CourseSelection(id);   // selecting course based on prob
        if(student_Removed(id)) break; 
    }
    return NULL;
}

int student_Removed(ll id){
    if(StudentList[id].existing==0){
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
    ll courseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos], prob;
    ll nextCourseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos + 1];
    ll pos = StudentList[id].prefPos;

    if (CourseList[courseWantedID].isExisting == 1) // to check if course is existing
    {
        if (CourseList[courseWantedID].isOn == 0) // to confirm the tutorial hasnt started
        {
            pthread_mutex_lock(&student_CourseApplication_lock);
            if (CourseList[courseWantedID].currentStudentSize++ < CourseList[courseWantedID].D) // to see if capacity is not filled yet
            {
                CourseList[courseWantedID].currentStudentSize++;
                StudentList[id].isLearning = 1;
                printf("Student %lld has been allocated a seat in course %s\n", id, CourseList[courseWantedID].courseName);
            }
            pthread_mutex_unlock(&student_CourseApplication_lock);
        }
        else // if tut is ongoing at the moment when student applies
        {   
            pthread_mutex_lock(&mutex_lock);
            printf("GOTTTT signal for course :\n");
            printf("StudID : %lld prefPos : %lld\n",id,StudentList[id].prefPos);
            pthread_cond_wait(&c, &mutex_lock);
            if (CourseList[courseWantedID].isExisting == 1) // to check if course is existing
            {
                if (CourseList[courseWantedID].isOn == 0) // to confirm the tutorial hasnt started
                {
                    pthread_mutex_lock(&student_CourseApplication_lock);
                    if (CourseList[courseWantedID].currentStudentSize++ < CourseList[courseWantedID].D) // to see if capacity is not filled yet
                    {
                        CourseList[courseWantedID].currentStudentSize++;
                        StudentList[id].isLearning = 1;
                        printf("Student %lld has been allocated a seat in course %s\n", id, CourseList[courseWantedID].courseName);
                    }
                    pthread_mutex_unlock(&student_CourseApplication_lock);
                }
            }
            pthread_mutex_unlock(&mutex_lock);
        }
    }
    else // if course is not existing
    {
        if (StudentList[id].prefPos == 2) // to check if preferences are left
        {
            StudentList[id].existing = 0; // since no preferences left, student is out of simulation
            printf("Student %lld couldn’t get any of his preferred courses\n",id);
        }
        else
        {

            
            printf("Student %lld has changed current preference from %s (priority %lld) to %s (priority %lld)\n", id, CourseList[courseWantedID].courseName, pos, CourseList[nextCourseWantedID].courseName, pos + 1);
            StudentList[id].prefPos++;
        }
    }
}

void student_CourseSelection(ll id) // after Tut is over, the student selects course based on probability
{

    ll courseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos], prob;
    ll nextCourseWantedID = StudentList[id].courseIDs[StudentList[id].prefPos + 1];
    ll pos = StudentList[id].prefPos;
    printf("StudID : %lld prefPos : %lld CourseWanted Id : %lld next : %lld \n",id,pos,courseWantedID,nextCourseWantedID);
    prob = StudentList[id].calibre * CourseList[courseWantedID].interestQ;
    if (prob > 0.5) // to check if interest is enuf
    {
        StudentList[id].existing = 0; // student is out of simulation since he has selected his prefered course
        printf("Student %lld has selected course %s permanently\n", id, CourseList[courseWantedID].courseName);
    }
    else // if not selected, preferences updated
    {
        if (StudentList[id].prefPos == 2) // to check if preferences are left
        {
            StudentList[id].existing = 0; // since no preferences left, student is out of simulation
            printf("Student %lld couldn’t get any of his preferred courses\n",id);
        }
        else
        {
            
            printf("Student %lld has changed current preference from %s (priority %lld) to %s (priority %lld)\n", id, CourseList[courseWantedID].courseName, pos, CourseList[nextCourseWantedID].courseName, pos + 1);
            StudentList[id].prefPos++;
        }
    }
}
