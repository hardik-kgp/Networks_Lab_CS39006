// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040


#include <bits/stdc++.h>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <assert.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
using namespace std;
#define BUFFER_SIZE 50        // maxisize of input file name
#define PORT 12000                // port address to bind
#define MAX_CON 5           // maximum number of waiting connections in queue
#define PACKET_SIZE 50     // size of packets

void error(char *msg){       // prints error message to stderr
    perror(msg);
    exit(1);
}


map<string, int> names_to_port, names_to_fd, last_used;


void sig_handler(int signal){
    for(auto &item:names_to_fd){
        close(item.second);
    }
    exit(0);
}
typedef struct user_info{
    string name;
    string ip;
    int port;
} user_info;

string findbyport(int port){
    for(auto item:names_to_port){
        if(item.second == port) return item.first;
    }
    return "";
}

pair<string,string> split(char *sen_msg, int len){
    string name = "", msg = "";
    int change = 0;
    for(int i = 0;i<len;i++){
        if(sen_msg[i] == '/'){
            change = 1;
        }
        else if(change == 0){
            name += sen_msg[i];
        }
        else msg += sen_msg[i];
    }
    return {name, msg};
}


int main(){

    int opt = 1;
    signal(SIGINT,sig_handler);
    // two arrays to receive and send message
    char sen_msg[BUFFER_SIZE], recv_msg[BUFFER_SIZE];
    names_to_port["Jan"] = 12000;
    names_to_port["Michael"] = 12001;
    names_to_port["Jim"] = 12002;
    names_to_port["Toby"] = 12003;
    names_to_port["Dwight"] = 12004;

    names_to_fd["Jan"] = -1;
    names_to_fd["Michael"] = -1;
    names_to_fd["Jim"] = -1;
    names_to_fd["Toby"] = -1;
    names_to_fd["Dwight"] = -1;

    
    int port = -1;
    do{
        cout << "Enter your username(Jan, Michael, Jim, Toby, Dwight): " ;
        string name;
        cin >> name;
        for(pair<string, int> item: names_to_port){
            if(item.first == name){
                port = item.second;
            }
        }
        if(port == -1){
            cout << "Please Enter a valid username" << endl;
        }
    }while(port == -1);
         
    

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) 
        error("Cannot Open Socket");

    if( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&opt, 
          sizeof(opt)) < 0 )  {  
        error("setsockopt");  
    }  
    struct sockaddr_in server;

    clock_t begin =  clock();
    
    server.sin_family = AF_INET;   // adress family = IPv4
    server.sin_addr.s_addr = inet_addr("127.0.0.1");   // local host as ip_address
    server.sin_port = htons(port);    // port number in the network order
    
    memset(server.sin_zero, 0, sizeof(server.sin_zero));  // assign zeros

    int bind_status = bind(server_fd, (const struct sockaddr *)&server, sizeof(server));  // bind the socket to address
    if(bind_status < 0){    // throw error if cannot bind
        error("Cannot bind the socket");
    }
    
    if(listen(server_fd, MAX_CON) < 0){
        error("Listening failed!");
    }

    

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

        if(FD_ISSET(STDIN_FILENO, &readfds)){

            bzero(sen_msg,BUFFER_SIZE);
            int len = read(STDIN_FILENO,sen_msg,BUFFER_SIZE);
            string peer_name, msg;
            tie(peer_name, msg) = split(sen_msg, len);
            if(!names_to_fd.count(peer_name)){
                error("incorrect message username!");
            }

            if(names_to_fd[peer_name] == -1){
                int sockid;
                struct sockaddr_in client;
                bzero((char *)&client, sizeof(client));
         
                sockid = socket(AF_INET, SOCK_STREAM, 0); //opening socket
                if (sockid < 0)
                    error("Cannot open socket");

                int opt1 = 1;
                if( setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, (char *)&opt1, sizeof(opt1)) < 0 )  {  
                    error("setsockopt");  
                }  
                int bind_status = bind(sockid, (const struct sockaddr *)&server, sizeof(server));
                if(bind_status  < 0){
                    error("Error in binding to Port");
                }
                // connecting to server
                client.sin_family = AF_INET; // adress family = IPv4
                client.sin_port = htons(names_to_port[peer_name]); // port number in the network order
                
                client.sin_addr.s_addr = inet_addr("127.0.0.1"); // ip address of the machine 
                

                if (connect(sockid, (struct sockaddr *)&client, sizeof(client)) < 0){
                    close(sockid);
                    error("Cannot connect to server");
                }

                names_to_fd[peer_name]=sockid;
                fds.push_back(sockid);
            }
            
            len = write(names_to_fd[peer_name], msg.c_str(), strlen(msg.c_str()));
            if (len < 0) 
                error("ERROR sending message to peer");
            
            last_used[peer_name]=(float)(clock()-begin)/CLOCKS_PER_SEC;
        }

        if(FD_ISSET(server_fd, &readfds)){

            struct sockaddr_in clientaddr; /* client addr */
            socklen_t clientlen = sizeof(clientaddr);
            int childfd = accept(server_fd, (struct sockaddr *) &clientaddr, &clientlen);
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(clientaddr.sin_addr), ip, INET_ADDRSTRLEN);
            int client_port = ntohs(clientaddr.sin_port);
            if(client_port < 12000 || client_port > 12004){
                error("Invalid Port!");
            }
            names_to_fd[findbyport(client_port)] = childfd;
            last_used[findbyport(client_port)] = (float)(clock()-begin)/CLOCKS_PER_SEC;

            fds.push_back(childfd);
        }

        

        for(auto item:names_to_fd){
            float current_time = (float)(clock()-begin)/CLOCKS_PER_SEC;
            if(current_time - last_used[item.first] > 120.0){   // case for timeout
                item.second = -1;
            }
            if(item.second == -1) continue;

            if(FD_ISSET(item.second, &readfds)){
                bzero(recv_msg, BUFFER_SIZE);
                int len = read(item.second, recv_msg, BUFFER_SIZE);
                last_used[item.first] = (float)(clock()-begin)/CLOCKS_PER_SEC;
                if (len < 0) 
                    error("ERROR reading from peer");
                if(len == 0)
                {
                    close(item.second);
                    item.second = -1;
                }
                else{
                    cout << item.first << " : " << recv_msg << endl;
                }
            }
        }
    }
  
    return 0;
}