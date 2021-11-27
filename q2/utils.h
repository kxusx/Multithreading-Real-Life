#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

typedef long long int ll;

ll X; // spectating time
ll num_groups;
ll G;

pthread_t personThreadArr[1000];   // array of threads for each course
pthread_t teamThreadArr[1000];     // array of threads for each student

pthread_mutex_t zone_lock[3];   
pthread_mutex_t mutex_lock[3];
pthread_cond_t c[3];

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
    char support;
    ll time_reached;
    ll zone;    // which zone he will go
    ll patience;
    ll enragedScore;
};

struct Zone{
    ll capacity;
    ll current;
};

struct Goal{
    char team[100];
    ll time;
    float probability;
};

struct thread_details
{
    int id;
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
