// g++ -std=c++11 client.cpp
#include "clientUtils.h"

pair<string, int> read_string_from_socket(int fd, int bytes)
{
    std::string output;
    output.resize(bytes);
 
    int bytes_received = read(fd, &output[0], bytes - 1);
    // debug(bytes_received);
    if (bytes_received <= 0)
    {
        cerr << "Failed to read data from socket. Seems server has closed socket\n";
        // return "
        exit(-1);
    }

    // debug(output);
    output[bytes_received] = 0;
    output.resize(bytes_received);

    return {output, bytes_received};
}

int send_string_on_socket(int fd, const string &s)
{  
    int bytes_sent = write(fd, s.c_str(), s.length());
    if (bytes_sent < 0)
    {
        cerr << "Failed to SEND DATA on socket.\n";
        // return "
        exit(-1);
    }
    //printf("Info sent\n");

    return bytes_sent;
}

int get_socket_fd()
{
    struct sockaddr_in server_obj;

    // socket() creates an endpoint for communication and returns a file descriptor that refers to that endpoint.
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("Error in socket creation for CLIENT");
        exit(-1);
    }
    

    int port_num = SERVER_PORT;

    memset(&server_obj, 0, sizeof(server_obj)); // Zero out structure
    server_obj.sin_family = AF_INET;
    server_obj.sin_port = htons(port_num); //convert to big-endian order

    /* connect to server */
    if (connect(socket_fd, (struct sockaddr *)&server_obj, sizeof(server_obj)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(-1);
    }
    //printf("Socket connected\n");
    return socket_fd;
}

// void begin_process()
// {
//     struct sockaddr_in server_obj;
//     int socket_fd = get_socket_fd(&server_obj);
//     cout << "Connection to server successful" << endl;
//     while (true)
//     {
//         string to_send;
//         cout << "Enter msg: ";
//         getline(cin, to_send);
//         send_string_on_socket(socket_fd, to_send);
//         int num_bytes_read;
//         string output_msg;
//         tie(output_msg, num_bytes_read) = read_string_from_socket(socket_fd, buff_sz);
//         cout << "Received: " << output_msg << endl;
//         cout << "====" << endl;
//     }
// }

void *client_Thread_Routine(void *arg)
{
    int id = ((struct thread_details *)arg)->id;
    sleep(commandList[id].send_time);

    int client_socket_fd = get_socket_fd();

    pthread_mutex_lock(&clientLock);
    send_string_on_socket(client_socket_fd, commandList[id].command);
    pthread_mutex_unlock(&clientLock);

    int num_bytes_read;
    string output_msg;

    pthread_mutex_lock(&clientLock);
    tie(output_msg, num_bytes_read) = read_string_from_socket(client_socket_fd, buff_sz);
    pthread_mutex_unlock(&clientLock);

    cout << id<<":" << output_msg << endl;
    return NULL;
}

void input()
{
    string mString, input, string_send_time = "";
    getline(cin, mString);
    m = stoi(mString);

    ll ll_send_time, i, j;
    for (i = 0; i < m; i++)
    {
        getline(cin, input);
        //cout << input << "\n";
        for (j = 0; input[j] != ' '; j++)
        {
            ;
        }
        string string_send_time = input.substr(0, j);
        string command = input.substr(j + 1, input.length());
        ll_send_time = stoi(string_send_time);
        commandList_Item item;
        item.send_time = ll_send_time;
        item.command = command;
        commandList.push_back(item);
    }
}

int main(int argc, char *argv[])
{
    input();
    printf("\n");

    // creating client threads
    pthread_t ClientThread[100];
    for (int i = 0; i < m; i++)
    {
        pthread_t curr_tid;
        struct thread_details *thread_input = (struct thread_details *)(malloc(sizeof(struct thread_details)));
        thread_input->id = i;
        pthread_create(&curr_tid, NULL, client_Thread_Routine, (void *)(thread_input));
        ClientThread[i] = curr_tid;
    }

    // joining client threads
    for (int l = 0; l < m; l++)
    {
        pthread_join(ClientThread[l], NULL);
    }

    return 0;
}
