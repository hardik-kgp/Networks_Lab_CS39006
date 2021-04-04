// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040

/*
    Client side code that uses TCP protocol. 
    Receives the file name from the user and sends it to server. Then receives the file size in chunks of PACKET_SIZE
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PACKET_SIZE 50                // maximum size of a word
#define PORT 8080                     // port address to bind
#define OUTPUT_FILE "output_file.txt" // output file name
#define B 20


void error(char *msg){ // prints error message to stderr
    perror(msg);
    exit(0);
}

int get_msg(int sockid, char *buffer, int get_size){ // send the message to the server arguments : socket id , message to send

    bzero(buffer, sizeof(buffer));  // reset the buffer to receive a new message
    
    int len;
    if(get_size == -1)
        len = recv(sockid, buffer, PACKET_SIZE, 0);
    else
        len = recv(sockid, buffer, get_size, MSG_WAITALL);
    return len;
}

void send_msg(int sockid, char *buffer){ // receive the message from the server arguments : socket id , message array
    size_t filelen = strlen(buffer) + 1;
    send(sockid, buffer, filelen, 0);
}

int isdelim(char c){
    return (c == ',' || c == ';' || c == ':' || c == '.' || c == ' ' || c == '\t' || c == '\n');
}



void main(){
    int sockid;
    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));

    sockid = socket(AF_INET, SOCK_STREAM, 0); //opening socket
    if (sockid < 0)
        error("Cannot open socket");

    // connecting to server
    server_addr.sin_family = AF_INET; // adress family = IPv4
    server_addr.sin_port = htons(PORT); // port number in the network order
    server_addr.sin_addr.s_addr = INADDR_ANY; // ip address of the machine 

    if (connect(sockid, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        close(sockid);
        error("Cannot connect to server");
    }
    
    // reading file name and sending it to the server
    printf("Enter file name: \n");
    char *buffer = (char *)malloc(sizeof(char) * PACKET_SIZE);
    scanf("%s", buffer);
    send_msg(sockid, buffer);

    int len = get_msg(sockid, buffer, -1);
    if(buffer[0] == 'E'){
        close(sockid); 
        error("FILE NOT FOUND");
    }
    
    len = get_msg(sockid, buffer, -1);

    int file_size = atoi(buffer);
    int remaining_size = file_size;
    
    // opening file
    int fout = open(OUTPUT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    bzero(buffer, PACKET_SIZE);
    int last_block_size = 0, num_blocks  = 0;
    // receving files in chunks
    while (remaining_size > 0){

        last_block_size = remaining_size < B? remaining_size: B;
        num_blocks++;
        remaining_size -= last_block_size;

        len = get_msg(sockid, buffer, last_block_size);
        // writing to file
        write(fout, buffer, strlen(buffer));
    }

    printf("Total number of blocks received = %d, Last Block size = %d", num_blocks, last_block_size);
    // close the connection and output file 
    close(sockid); 
    close(fout);
    exit(0);
}
