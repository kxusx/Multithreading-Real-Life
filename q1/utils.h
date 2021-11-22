#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef long long int ll;
time_t startTime;


pthread_t courseThreadArr[10000];   // array of threads for each course
pthread_t studThreadArr[10000];     // array of threads for each student
pthread_mutex_t course_TA_Allocation_lock;           // mutex lock
pthread_mutex_t student_CourseApplication_lock;           // mutex lock
pthread_mutex_t mutex_lock;
pthread_cond_t c;

int turns[100];
struct Student
{
    float calibre;
    ll courseIDs[3];
    float time;
    ll isLearning;                  // if students is allocated for tutorial already or not
    ll prefPos;                     // preferencePosition
    ll existing;                    // to check if student is in simulation or not
};

struct Course
{
    char courseName[50];
    float interestQ;
    ll course_max_slots;
    ll D;                  // no of slots announced for current tutorial
    ll p;                  // number of labs from which the course accepts TAs
    ll listp[100];         // list of lab IDs
    ll currentStudentSize; // no of students allocated for current tutorial of course
    char courseTA[100];    // TA name for current tutorial of course
    ll isOn;               // if Course tutorial is On/Off
    ll isExisting;         // if Course is in simulation or not
    ll labID_Allocated;
    ll TA_Allocated;        // TA id allocated for curr Tut
};

struct Lab
{
    char labName[50];
    ll noOfTAs;
    ll courseDone[100];         // courseDone[i] tells no of courses TA[i] has done
    ll busy[100];               // busy[i] tells if TA[i] is allocated for a tutorial already or not
    ll max;                     // maximum no of tutorials TA of this lab can do
};

ll num_student, num_labs, num_courses, i, j;
char position[50];
struct Course CourseList[100];
struct Lab LabList[100];
struct Student StudentList[100];

struct thread_details
{
    int id;
};
