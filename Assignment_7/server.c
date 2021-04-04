// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040


/*
    Server side code that uses TCP protocol. 
    Opens the requested file name and sends the contents of the file in chunks of PACKET_SIZE 
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
#include <fcntl.h>

#define PACKET_SIZE 50                // maximum size of a word
#define PORT 8080                     // port address to bind
#define MAX_CON 5                     // maximum number of waiting connections in queue
#define B 20


void error(char *msg){       // prints error message to stderr
    perror(msg);
    exit(1);
}

int main(){
    // two arrays to receive and send message
    char *sen_msg = (char *)malloc(sizeof(char)*PACKET_SIZE);
    char *recv_msg = (char *)malloc(sizeof(char)*PACKET_SIZE);
    
    /* Open a socket of address domain AF_INET of type SOCK_STREAM used for TCP.
     The third argument chooses the appropriate protocol for the given domain. */
    int sockid = socket(AF_INET, SOCK_STREAM, 0);

    // if the return value is -1 throw error;
    if(sockid == -1){
        error("Cannot open socket");
    }
    
    /* Create two objects server and client of type sockaddr_in to store
     the internet address of the client and server */
    struct sockaddr_in server, client;

    
    server.sin_family = AF_INET;                            // adress family = IPv4
    server.sin_addr.s_addr = INADDR_ANY;                    // ip address of the machine 
    server.sin_port = htons(PORT);                          // port number in the network order
    memset(server.sin_zero, 0, sizeof(server.sin_zero));    // assign zeros

    int bind_status = bind(sockid, (const struct sockaddr *)&server, sizeof(server));  // bind the socket to address
    if(bind_status < 0){    // throw error if cannot bind
        error("Cannot bind the socket");
    }
    
    // keep listening to 
    if(listen(sockid, MAX_CON) < 0){
        error("Listening failed!");
    }

    int addr_size = sizeof(client);
    while(1)  // keep listening to the connection requests
    {
        int new_id = accept(sockid, (struct sockaddr *)&client, (socklen_t *) (&addr_size));  // connection id of the new client 
        if(new_id < 0){   // throw error is client id returned is invalid 
            error("accepting failed!");
        }
        
        int nbytes  = recv(new_id, recv_msg, PACKET_SIZE, 0);  // receive the file name 
        
        if(nbytes <= 0){
            error("File name invalid");
        }
                               
        // message received is stored in recv_msg
        recv_msg[nbytes] = '\0';

        // open the file 
        int file = open(recv_msg, O_RDONLY,0);

        if(file < 0){  // throw error is file cannot be opened 
            sprintf(sen_msg, "E");
            send(new_id, sen_msg, PACKET_SIZE, 0); 	// send the buffer with max_size of PACKET_SIZE
            close(new_id);
            error("File not found");
        }
        else{
            sprintf(sen_msg, "L");
            send(new_id, sen_msg, PACKET_SIZE, 0); 	// send the buffer with max_size of PACKET_SIZE
            bzero(sen_msg, sizeof(sen_msg));        // reset the message to send

            off_t FSIZE;                            // sending size of file
            FSIZE = lseek(file, 0L, SEEK_END);
            sprintf(sen_msg, "%ld", FSIZE);
            send(new_id, sen_msg, PACKET_SIZE, 0); 	// send the buffer with max_size of PACKET_SIZE
            close(file);                            // close the file   
        }

        file = open(recv_msg, O_RDONLY,0);
        bzero(sen_msg, sizeof(sen_msg));  // reset the message to send

        // keep reading and sending the contents of the file in blocks of size B       
        while(read(file, sen_msg, B) > 0){
            send(new_id, sen_msg, B, 0); 	  // send the buffer with block size
            bzero(sen_msg, sizeof(sen_msg));  // reset the message to send
        }

        // close the connection id and the file of the current client 
        close(new_id);  // close the current client
        close(file);  // close the file   
    }

    close(sockid);  // close the socket 
    return 0;
}