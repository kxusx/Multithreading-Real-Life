#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <semaphore.h>
#include "semaphoreP.h"

typedef long long int ll;

ll X; // spectating time
ll num_groups;
ll G; 
ll timer;

pthread_t personThreadArr[1000];   // array of threads for each course
pthread_t goalThreadArr[1000];     // array of threads for each student
pthread_t sleeper;

Semaphore* zoneAs;
Semaphore* zoneHs;
Semaphore* zoneNs;

char zones1[3];

pthread_mutex_t position_lock[2];
pthread_mutex_t zone_lock[2];
pthread_mutex_t zone1_lock[2];
pthread_mutex_t time_lock[1000];
pthread_mutex_t person_lock[100];
pthread_cond_t timeC[1000];

struct Group{
    ll size;   // no of ppl in grp
    char names[100][100];
    char support[100];
    ll time_reached[100];
    ll patience[100];
    ll enragedScore[100];

};

struct Person{
    ll groupNo;
    char name[100];
    char c_support; // (H,A,N)
    ll time_reached;
    ll patience;
    ll enragedScore;
    
    ll stadiumReached;

    ll support;    // which team he supports(0,1,2)
    ll currentZoneSeated;
    char c_currentZoneSeated;
    ll alreadyExisted;
     
    ll gotSeated;
    ll seatedTime;
    ll timeWatched;
};

struct Zone{
    ll capacity;
    ll current;
    ll score;
    ll pos;
};

struct Goal{
    char team[20];
    ll time;
    float probability;
    ll pos;
    char position[20];
    ll zone;
};

struct thread_details
{
    int id;
    ll wantedZone;   // which zone he is trying to be in
};

struct Group groups[1000];
struct Goal goals[100];
struct Person persons[1000];
struct Zone zones[3];


#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"


#define RESET   "\x1b[0m"
