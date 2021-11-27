// g++ -std=c++11 server.cpp -lpthread
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <pthread.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream> 
#include <list>
#include <queue>
#include <assert.h>
#include <tuple>
#include <string.h>
#include <iostream>
#include <vector>
#include <string> 
#include <boost/lexical_cast.hpp>
#include <sstream>
using namespace std;

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

typedef long long ll;

#define pb push_back
#define debug(x) cout << #x << " : " << x << endl
#define part cout << "-----------------------------------" << endl;

#define MAX_CLIENTS 4
#define SERVER_PORT 8008

const int initial_msg_len = 256;

int counter;
 
ll length =1000;
string keys[1000];


queue<int*> clientListQueue;

pthread_mutex_t mutex_lock[1000];
pthread_mutex_t clientList_Lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
    