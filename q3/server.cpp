#include "serverUtils.h"
#include "clientUtils.h"

//------------------------------------------------------------------------------------------------------------------------

pair<string, int> read_string_from_socket(const int &fd, int bytes)
{
    std::string output;
    output.resize(bytes);
 
    int bytes_received = read(fd, &output[0], bytes - 1);
    //debug(bytes_received);
    if (bytes_received <= 0)
    {
        cerr << "Failed to read data from socket. \n";
    }

    output[bytes_received] = 0;
    output.resize(bytes_received);
    return {output, bytes_received};
}

//------------------------------------------------------------------------------------------------------------------------

int send_string_on_socket(int fd, const string &s)
{
    // debug(s.length());
    //printf("sending\n");
    //cout<<s;
    int bytes_sent = write(fd, s.c_str(), s.length());
    if (bytes_sent < 0)
    {
        cerr << "Failed to SEND DATA via socket.\n";
    }
    return bytes_sent;
}

void adder(string cmd, int client_socket_fd,pthread_t pthreadID)
{
    vector<string> token_commands;
    stringstream check1(cmd);
    string intermediate, ack;
    string st = boost::lexical_cast<string>(pthreadID);

    while (getline(check1, intermediate, ' '))
    {
        token_commands.push_back(intermediate);
    }

    ll index = stoi(token_commands[1]), flag = 0, index2;
    //cout << token_commands[0] << "\n";

    if (token_commands[0] == "insert")
    {
        //printf("in insert:\n");
        if (keys[index] == "0")
        {
            //printf("Insertion Successful\n");
            pthread_mutex_lock(&mutex_lock[index]);
            keys[index] = token_commands[2];
            pthread_mutex_unlock(&mutex_lock[index]);
            ack = st + ":Insertion Successful";
            cout<<ack<<"\n";
            send_string_on_socket(client_socket_fd, ack);
            flag = 1;
        } else 
        {
            //printf("Key already exists\n");
            ack = st + ":Key already exists";
            cout<<ack<<"\n";
            send_string_on_socket(client_socket_fd, ack);
        }
    }

    if (token_commands[0] == "delete")
    {
        if (keys[index] != "0")
        {
            pthread_mutex_lock(&mutex_lock[index]);
            keys[index] = "0";
            pthread_mutex_unlock(&mutex_lock[index]);
            ack = st + ":Deletion Successful";
            cout<<ack<<"\n";
            send_string_on_socket(client_socket_fd, ack);
        }
        else
        {
            keys[index] = "0";
            ack = st + ":No such key exists";
            cout<<ack<<"\n";
            send_string_on_socket(client_socket_fd, ack);
        }
    }

    if (token_commands[0] == "update")
    {
        if (keys[index] != "0")
        {
            pthread_mutex_lock(&mutex_lock[index]);
            keys[index] = token_commands[2];
            string p = st + ":"+keys[index];
            cout<<p<<"\n";
            pthread_mutex_unlock(&mutex_lock[index]);
            send_string_on_socket(client_socket_fd, p);
        }
        else
        {
            //printf("Key doesn't exist\n");
            ack = st + ":Key doesn't exist";
            cout<<ack<<"\n";
            send_string_on_socket(client_socket_fd, ack);
        }
    }

    if (token_commands[0] == "concat")
    {
        index2 = stoi(token_commands[2]);
        if (keys[index] != "0" && keys[index2] != "0")
        {
            string s1 = keys[index];
            string s2 = keys[index2];
            string s1s2 = s1 + s2;
            string s2s1 = s2 + s1;
            pthread_mutex_lock(&mutex_lock[index]);
            keys[index] = s1s2;
            pthread_mutex_unlock(&mutex_lock[index]);

            pthread_mutex_lock(&mutex_lock[index2]);
            keys[index2] = s2s1;
            pthread_mutex_unlock(&mutex_lock[index2]);
            s2s1 = st+":"+s2s1;
            cout<<s2s1<<"\n";
            send_string_on_socket(client_socket_fd, s2s1);
        }
        else
        {
            //printf("Concat failed as at least one of the keys does not exist\n");
            ack = st + ":Concat failed as at least one of the keys does not exist";
            cout<<ack<<"\n";
            send_string_on_socket(client_socket_fd, ack);
        }
    }

    if (token_commands[0] == "fetch")
    {
        if (keys[index] != "0")
        {
            pthread_mutex_lock(&mutex_lock[index]);
            string p = st + ":"+keys[index];
            cout<<p<<"\n";
            send_string_on_socket(client_socket_fd, p);
            pthread_mutex_unlock(&mutex_lock[index]);
        }
        else
        {
            //printf("Key doesn't exist\n");
            ack = st+ ":Key doesn't exist";
            cout<<ack<<"\n";
            send_string_on_socket(client_socket_fd, ack);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void handle_connection(int client_socket_fd,pthread_t pthreadID)
{
    int received_num, sent_num;
    int ret_val = 1;
    //printf("handle:%d\n", client_socket_fd);
    string cmd;
    tie(cmd, received_num) = read_string_from_socket(client_socket_fd, buff_sz);
    ret_val = received_num;

    if (ret_val <= 0)
    {
        // perror("Error read()");
        printf("Server could not read msg sent from client\n");
        //goto close_client_socket_ceremony;
    }

    adder(cmd, client_socket_fd,pthreadID);

close_client_socket_ceremony:
    close(client_socket_fd);
    //printf(RED "Disconnected from client" RESET "\n");
    return;
}

//------------------------------------------------------------------------------------------------------------------------

void *serverThread_routine(void *arg)
{
    while (1)
    {
        int *P_clientSocketfd;

        pthread_mutex_lock(&clientList_Lock);
        while (clientListQueue.empty())
        {
            pthread_cond_wait(&c, &clientList_Lock);
        }
        pthread_mutex_unlock(&clientList_Lock);

        if (!clientListQueue.empty())
        {
            P_clientSocketfd = clientListQueue.front();
            clientListQueue.pop();
            int clientSocketfd = *P_clientSocketfd;
            pthread_t pthreadID= pthread_self();
            handle_connection(clientSocketfd,pthreadID);
        }
    }
    return NULL;
}

//------------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    counter = 0;
    for (int i = 0; i < length; i++)
    {
        keys[i] = "0";
    }

    if (argc < 2)
    {
        cout << "Wrong Number of Arguments Given\n";
        exit(0);
    }

    ll noOfWorkerThreads = stoi(argv[1]);
    pthread_t serverThread[noOfWorkerThreads];

    for (int i = 0; i < noOfWorkerThreads; i++)
    {
        pthread_create(&serverThread[i], NULL, serverThread_routine, NULL);
    }

    //----------------------------------------------------------------------

    int wel_socket_fd, client_socket_fd, port_number;
    socklen_t clilen;

    struct sockaddr_in serv_addr_obj, client_addr_obj;

    //----------------------------------------------------------------------

    //creates a new socket
    wel_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (wel_socket_fd < 0)
    {
        perror("ERROR creating welcoming socket");
        exit(-1);
    }

    //----------------------------------------------------------------------

    bzero((char *)&serv_addr_obj, sizeof(serv_addr_obj)); // sets everything to 0

    port_number = SERVER_PORT;
    serv_addr_obj.sin_family = AF_INET;
    // On the server side I understand that INADDR_ANY will bind the port to all available interfaces,
    serv_addr_obj.sin_addr.s_addr = INADDR_ANY;
    serv_addr_obj.sin_port = htons(port_number); //process specifies port

    /* bind socket to this port number on this machine */
    if (::bind(wel_socket_fd, (struct sockaddr *)&serv_addr_obj, sizeof(serv_addr_obj)) < 0)
    {
        perror("Error on bind on welcome socket: ");
        exit(-1);
    }

    //----------------------------------------------------------------------

    /* listen for incoming connection requests */
    listen(wel_socket_fd, noOfWorkerThreads);
    cout << "Server has started listening on the LISTEN PORT" << endl;
    clilen = sizeof(client_addr_obj);

    while (1)
    {
        /* accept a new request, create a client_socket_fd */
        //printf("Waiting for a new client to request for a connection\n");
        client_socket_fd = accept(wel_socket_fd, (struct sockaddr *)&client_addr_obj, &clilen);
        if (client_socket_fd < 0)
        {
            perror("ERROR while accept() system call occurred in SERVER");
            exit(-1);
        }

        //printf(GREEN "New client connected from port number %d and IP %s \n" RESET, ntohs(client_addr_obj.sin_port), inet_ntoa(client_addr_obj.sin_addr));

        int *P_clientSocketfd = (int *)malloc(sizeof(int));
        *P_clientSocketfd = client_socket_fd;

        pthread_mutex_lock(&clientList_Lock);
        clientListQueue.push(P_clientSocketfd);
        pthread_cond_signal(&c);
        pthread_mutex_unlock(&clientList_Lock);
        counter++;
        //printf("counter:%d m:%lld\n", counter,m);
    }

    //----------------------------------------------------------------------
    //cout << "bye\n";
    for (int i = 0; i < noOfWorkerThreads; i++)
    {
        pthread_join(serverThread[i], NULL);
    }

    close(wel_socket_fd);
    return 0;
}