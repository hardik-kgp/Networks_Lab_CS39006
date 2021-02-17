// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040



/*
    Server side code that uses UDP protocol. 
    Opens the requested file name and sends the word by word messages to each of the word requests
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <assert.h>

#define BUFFER_SIZE 1024         // maxisize of a word
#define PORT 8080                // port address to bind

void error(char *msg){       // prints error message to stderr
    perror(msg);
    exit(1);
}

void get(int sockid, char *recv_msg, struct sockaddr_in* client){    // send the message to the client arguments : socket id , message to send and address structure object
    int l = sizeof(*client);
    int len = recvfrom(sockid, recv_msg, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)client, &l);
    
    recv_msg[len] = '\0';
}

void send_msg(int sockid, char *sen_msg, struct sockaddr_in* client){  // receive the message from the client arguments : socket id , message array and address structure object
    uint l = sizeof(*client);
    int msg_len = strlen(sen_msg);
    sendto(sockid, sen_msg, msg_len, MSG_CONFIRM, (const struct sockaddr *)(client), l);
}

int main(){

    // two arrays to receive and send message
    char sen_msg[BUFFER_SIZE], recv_msg[BUFFER_SIZE];
    // open a socket of address domain AF_INET of type SOCK_DGRAM used for UDP.
    // The third argument chooses the appropriate protocol for the given domain.
    int sockid = socket(AF_INET, SOCK_DGRAM, 0);
    // if the return value is -1 throw error;
    if(sockid == -1){
        error("Cannot open socket");
    }
    // create two objects server and client of type sockaddr_in to store
    // the internet address of the client and server
    struct sockaddr_in server, client;

    
    server.sin_family = AF_INET;   // adress family = IPv4
    server.sin_addr.s_addr = INADDR_ANY;   // ip address of the machine 
    server.sin_port = htons(PORT);    // port number in the network order
    memset(server.sin_zero, 0, sizeof(server.sin_zero));  // assign zeros
    int bind_status = bind(sockid, (const struct sockaddr *)&server, sizeof(server));  // bind the socket to address
    if(bind_status < 0){    // throw error if cannot bind
        error("Cannot bind the socket");
    }
     
    
    // recieve the file name from the client
    get(sockid, recv_msg, &client);
    FILE *f = fopen(recv_msg, "r");   
    if(f == NULL){    // print file not found if no such file exist
        send_msg(sockid, "FILE_NOT_FOUND",&client);
        printf("File not found");
        exit(1);
    }

    fscanf(f, "%s", sen_msg);              // read the first line 
    if(strcmp(sen_msg, "HELLO") != 0){     // throw wrong file format if the first word is not HELLO
        send_msg(sockid, "WRONG_FILE_FORMAT", &client);
        printf("Wrong File Format");
        exit(1);
    }

    send_msg(sockid, sen_msg, &client);   // send this message 

    int i = 1;   // keeps count of the number of words
   
    while(1) {
        get(sockid, recv_msg, &client);   // receive the word request
        char temp[BUFFER_SIZE];
        sprintf(temp, "WORD%d", i);
        assert(strcmp(recv_msg, temp) == 0);   // check if the word is of the correct format

        fscanf(f,"%s", sen_msg);    // read the word from the file 
        send_msg(sockid, sen_msg, &client);   // send the message to the client

        if(strcmp(sen_msg, "END") == 0) {   // if the END word is found break out.
            break;
        }

        i++;  // increase the word count 
    }

    return 0;

}