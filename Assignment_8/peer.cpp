// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040

#include "peer.h"

int main(){

    int opt = 1;
    signal(SIGINT,sig_handler);
    /* Two arrays to receive and send message */
    char sen_msg[BUFFER_SIZE], recv_msg[BUFFER_SIZE];
    init_map();

    int port = -1;
    while (true)
    {
        printf("Enter your username(Jan, Michael, Jim, Toby, Dwight): ");
        string name; cin >> name;
        if (findbyname(name) == -1)
            error("Please Enter a valid username");
        else{
            port = findbyname(name);
            printf("Hi %s !\n", name.c_str());
            PROMPT;
            break;
        }
    }

    /* Setting up the server */
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) 
        errorExit("Cannot Open Socket");

    if( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, 
          sizeof(opt)) < 0 )  {  
        errorExit("setsockopt");  
    }  
    struct sockaddr_in server;

    auto begin = std::chrono::system_clock::now();
    
    server.sin_family = AF_INET;                            // adress family = IPv4
    server.sin_addr.s_addr = inet_addr("127.0.0.1");        // local host as ip_address
    server.sin_port = htons(port);                          // port number in the network order
    
    memset(server.sin_zero, 0, sizeof(server.sin_zero));    // assign zeros

    int bind_status = bind(server_fd, (const struct sockaddr *)&server, sizeof(server));  // bind the socket to address
    if(bind_status < 0){                                    // throw error if cannot bind
        errorExit("Cannot bind the socket");
    }
    
    if(listen(server_fd, MAX_CON) < 0){
        errorExit("Listening failed!");
    }

    /* Variables for select function */
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    struct timeval timeout;
    timeout.tv_sec=120;
    vector<int> fds;
    fds.push_back(STDIN_FILENO);
    fds.push_back(server_fd);

    while(true){
        
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_ZERO(&exceptfds);
        int max_fd = 0;
        for(int fd: fds){
            max_fd = max(max_fd, fd);
            FD_SET(fd, &readfds);

        }
        
        int total_triggered = select(max_fd + 1, &readfds, &writefds,&exceptfds,&timeout);

        /* To take input from command line*/
        if(FD_ISSET(STDIN_FILENO, &readfds)){

            bzero(sen_msg,BUFFER_SIZE);
            int len = read(STDIN_FILENO,sen_msg,BUFFER_SIZE);
            string peer_name, msg;
            tie(peer_name, msg) = split(sen_msg, len);
            if(!fdMap.count(peer_name)){
                error("Incorrect message username!");
                continue;
            }

            /* Establishing connection to the client */
            if(fdMap[peer_name] == -1){
                int sockid;
                struct sockaddr_in client;
                bzero((char *)&client, sizeof(client));
         
                sockid = socket(AF_INET, SOCK_STREAM, 0); //opening socket
                if (sockid < 0){
                    error("Cannot open socket");
                    continue;
                }
                int opt_ = 1;
                if( setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, (char *)&opt_, sizeof(opt_)) < 0 )  {  
                    error("Setsockopt error");  
                    continue;
                }  
                int bind_status = bind(sockid, (const struct sockaddr *)&server, sizeof(server));
                if(bind_status  < 0){
                    error("Error in binding to Port");
                    continue;
                }

                client.sin_family = AF_INET;                        // adress family = IPv4
                client.sin_port = htons(findbyname(peer_name));        // port number in the network order
                client.sin_addr.s_addr = inet_addr("127.0.0.1");    // ip address of the machine 

                if (connect(sockid, (struct sockaddr *)&client, sizeof(client)) < 0){
                    close(sockid);
                    error("Cannot connect to client");
                    continue;
                }

                fdMap[peer_name]=sockid;
                fds.push_back(sockid);
            }
            
            len = write(fdMap[peer_name], msg.c_str(), strlen(msg.c_str()));
            if (len < 0) error("Cannot send message to peer");
            else PROMPT;

            last_used[peer_name]=get_time(begin);
        }

        /* Getting messages from others*/
        if(FD_ISSET(server_fd, &readfds)){

            struct sockaddr_in clientaddr; /* client addr */
            socklen_t clientlen = sizeof(clientaddr);
            int childfd = accept(server_fd, (struct sockaddr *) &clientaddr, &clientlen);
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientaddr.sin_addr), ip, INET_ADDRSTRLEN);
            int client_port = ntohs(clientaddr.sin_port);
            if(client_port < 12000 || client_port > 12004){
                error("Invalid Port!");
                continue;
            }
            fdMap[findbyport(client_port)] = childfd;
            last_used[findbyport(client_port)] = get_time(begin);

            fds.push_back(childfd);
        }

        /* Printing messages and Disconneting due to inactivity*/
        for(auto &item:fdMap){

            if(item.second == -1) continue;
            float current_time = get_time(begin);
            if(current_time - last_used[item.first] > 120.0){   // case for timeout
                cout << "Connection closed with " << item.first << "! "<< endl;
                close(item.second);
                item.second = -1;
            }
            

            if(FD_ISSET(item.second, &readfds)){
                bzero(recv_msg, BUFFER_SIZE);
                int len = read(item.second, recv_msg, BUFFER_SIZE);
                last_used[item.first] = get_time(begin);
                if (len < 0) 
                    error("Error reading from peer");
                if(len == 0){
                    close(item.second);
                    item.second = -1;
                }
                else{
                    printf("From %s: %s\n", item.first.c_str(), recv_msg);
                    PROMPT;
                }
            }
        }
    }
    return 0;
}

void sig_handler(int signal){
    for(auto &item:fdMap){
        close(item.second);
    }
    exit(0);
}

string findbyport(int port){
    for(int i = 0;i<5;i++){
        if(portMap[i].port == port) return portMap[i].name;
    }
    return "";
}

int findbyname(string name){
    for(int i = 0;i<5;i++){
        if(portMap[i].name == name) return portMap[i].port;
    }
    return -1;
}
pair<string,string> split(char *sen_msg, int len){
    string name = strtok(sen_msg, "/");
    string msg = strtok(NULL, "/");
    return {name, msg};
}