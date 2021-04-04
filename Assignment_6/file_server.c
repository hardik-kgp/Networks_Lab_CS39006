// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040



/*
    Server side code that uses TCP protocol. 
    Opens the requested file name and sends the contents of the file in max_size PACKET_SIZE chunks 
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

#define BUFFER_SIZE 50        // maxisize of input file name
#define PORT 8080                // port address to bind
#define MAX_CON 5           // maximum number of waiting connections in queue
#define PACKET_SIZE 50     // size of packets

void error(char *msg){       // prints error message to stderr
    perror(msg);
    exit(1);
}

int main(){
     // two arrays to receive and send message
    char sen_msg[BUFFER_SIZE], recv_msg[BUFFER_SIZE];
    // open a socket of address domain AF_INET of type SOCK_STREAM used for TCP.
    // The third argument chooses the appropriate protocol for the given domain.
    int sockid = socket(AF_INET, SOCK_STREAM, 0);
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
        
        int nbytes  = recv(new_id, recv_msg, BUFFER_SIZE, 0);  // receive the file name 
        
        if(nbytes <= 0){
            error("File name invalid");
        }                               
        // message received is stored in recv_msg
        recv_msg[nbytes] = '\0';

        printf("File Recieved: %s\n", recv_msg);

        // open the file 
        int file = open(recv_msg, O_RDONLY,0);

        if(file < 0){  // throw error is file cannot be opened 
            close(new_id);
            close(sockid);
            error("File name invalid");
        }

        bzero(sen_msg, sizeof(sen_msg));  // reset the message to send
        // keep reading and sending the contents of the file        
        int sent_flag = 0;
        while(read(file, sen_msg, PACKET_SIZE) > 0){
            send(new_id, sen_msg, PACKET_SIZE, 0); 	// send the buffer with max_size of PACKET_SIZE
            bzero(sen_msg, sizeof(sen_msg));  // reset the message to send
            sent_flag = 1;
        }

        if(sent_flag == 0){     // file empty
            sen_msg[0] = '\0';
            send(new_id, sen_msg, PACKET_SIZE, 0);
        }
        // close the connection id and the file of the current client 
        close(new_id);  // close the current client
        close(file);  // close the file 
        
    }

    close(sockid);  // close the socket 
    return 0;

	
	
    



}