// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040

/*
    Client side code that uses UDP protocol. 
    Sends the file name to read and then word by word writes into the output file
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
#define INPUT_FILE "file.txt"    // input file name
#define OUTPUT_FILE "output_file.txt"   // output file name

void error(char *msg){    // prints error message to stderr
    perror(msg);
    exit(1);
}

void get(int sockid, char *recv_msg, struct sockaddr_in* server){        // send the message to the server arguments : socket id , message to send and address structure object
    int l = sizeof(*server);
    int len = recvfrom(sockid, recv_msg, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*)server, &l);
    recv_msg[len] = '\0';
}

void send_msg(int sockid, char *sen_msg, struct sockaddr_in* server){    // receive the message from the server arguments : socket id , message array and address structure object
    
    int msg_len = strlen(sen_msg);
    sendto(sockid, sen_msg, msg_len, MSG_CONFIRM, (const struct sockaddr *)(server), sizeof(*server));
}

int main(){

    // two arrays to receive and send message
    char sen_msg[BUFFER_SIZE], recv_msg[BUFFER_SIZE];
     // open a socket of address domain AF_INET of type SOCK_DGRAM used for UDP.
    // The third argument chooses the appropriate protocol for the given domain.
    int sockid = socket(AF_INET, SOCK_DGRAM, 0);
    // if the return value is -1 throw error;
    if(sockid < 0){
        error("Cannot open socket");
    }

    // create client of type struct sockaddr_in to store
    // the internet address of the server
    struct sockaddr_in server;

    server.sin_family = AF_INET; // adress family = IPv4
    server.sin_addr.s_addr = INADDR_ANY;    // ip address of the machine 
    server.sin_port = htons(PORT); // port number in the network order
    memset(server.sin_zero, 0, sizeof(server.sin_zero)); // assign zeros

    
    
    // copy the input file name to the send message 
    sprintf(sen_msg, INPUT_FILE);
    
    // send the file name to the server
    send_msg(sockid, sen_msg, &server);


    // receive the first name 
    get(sockid, recv_msg, &server);
    
    
    // throw error if server couldn't open the requested file
    if(strcmp(recv_msg, "FILE_NOT_FOUND") == 0){
        printf("File not found");
        exit(1);
    }

    // throw error if the file contents are not in correct format
    else if(strcmp(recv_msg, "WRONG_FILE_FORMAT") == 0){
        printf("Wrong File Format");
        exit(1);
    }
    
    // create a file to write the received contents
    FILE* f= fopen(OUTPUT_FILE, "w");
     
    // keeps count of the number of words 
    int i = 1;
    while(1) {

        sprintf(sen_msg, "WORD%d", i);         // copy the request to the send message
        send_msg(sockid, sen_msg, &server);    // send the message to the server
        get(sockid, recv_msg, &server);   // receive the next word from the server
	
	if(strcmp(recv_msg, "END") == 0) {   // if the file contents are finished break out
            break;
        }

        fprintf(f, "%s\n", recv_msg);   // print the word to the file 
        
        
        i++;
    }


    return 0;
}
