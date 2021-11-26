#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef long long int ll;
ll i,j;
ll Hzone_capacity,Azone_capacity, Nzone_capacity;
ll X; // spectating time
ll num_groups;
ll G;

struct Group{
    ll size;   // no of ppl in grp
    char names[100][100];
    char support[100];
    ll time_reached[100];
    ll patience[100];
    ll enragedScore[100];
};

struct Goal{
    char team[100];
    char time[100];
    char probability[100];
};

struct Group groups[1000];
struct Goal goals[100];

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"

#define RESET   "\x1b[0m"
