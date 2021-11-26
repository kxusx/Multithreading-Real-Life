
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <pthread.h>
#include <iostream> 
#include <semaphore.h>
#include <assert.h>
#include <queue>
#include <vector>
#include <tuple> 
using namespace std;
typedef long long ll;

struct commandList_Item{
    ll send_time;
    string command;
};

vector <commandList_Item> commandList;

struct thread_details
{
    int id;
};

pthread_mutex_t clientLock = PTHREAD_MUTEX_INITIALIZER;


const ll MOD = 1000000007;
#define SERVER_PORT 8007

#define part cout << "-----------------------------------" << endl;
#define pb push_back
#define debug(x) cout << #x << " : " << x << endl

const ll buff_sz = 1048576;
ll m; //totalnumberofuserrequeststhroughoutthesimulation

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"