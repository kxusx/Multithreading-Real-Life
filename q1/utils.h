#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef long long int ll;
time_t startTime;
ll studentsOver;
ll coursesOver;
ll timer;

ll courseInterest[100];


pthread_t courseThreadArr[10000];   // array of threads for each course
pthread_t studThreadArr[10000];     // array of threads for each student

pthread_mutex_t course_TA_Allocation_lock[100];           // mutex lock
pthread_mutex_t lab_lock[100];  
pthread_mutex_t student_CourseApplication_lock[100];           // mutex lock

pthread_t sleeper;

pthread_mutex_t time_lock[1000];
pthread_cond_t timeC[1000];

pthread_mutex_t mutex_lock[100];
pthread_cond_t c[100];

pthread_mutex_t mutex_lock2[100];
pthread_cond_t c2[100];

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
    ll p;                  // number of labs from which the course accepts TAs
    ll listp[100];         // list of lab IDs

    ll D;                  // no of slots announced for current tutorial
    ll currentStudentSize; // no of students allocated for current tutorial of course

    ll isOn;               // if Course tutorial is On/Off
    ll isStarting;
    ll isExisting;         // if Course is in simulation or not

    char courseTA[100];    // TA name for current tutorial of course
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

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"

#define RESET   "\x1b[0m"
