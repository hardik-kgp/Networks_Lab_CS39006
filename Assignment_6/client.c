// Name: Hardik Aggarwal
// Roll No: 18CS10021

// Name : Sriyash Poddar
// Roll No: 18CS30040

/*
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

#define MAX_SIZE 75                   // maximum size of a word
#define PORT 8080                     // port address to bind
#define OUTPUT_FILE "output_file.txt" // output file name

void error(char *msg){ // prints error message to stderr
    perror(msg);
    exit(1);
}

int get_msg(int sockid, char *buffer){ // send the message to the server arguments : socket id , message to send
    int len = recv(sockid, buffer, MAX_SIZE, 0);
    return len;
}

void send_msg(int sockid, char *buffer){ // receive the message from the server arguments : socket id , message array
    size_t filelen = strlen(buffer) + 1;
    send(sockid, buffer, filelen, 0);
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
    char *buffer = (char *)malloc(sizeof(char) * MAX_SIZE);
    scanf("%s", buffer);
    send_msg(sockid, buffer);

    // opening file
    int fout = open(OUTPUT_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    int len = 0, num_words = 0, num_bytes = 0;
    int i = 0, flag = 0;
    bzero(buffer, MAX_SIZE);
    
    // receving files in chunks
    while ((len = get_msg(sockid, buffer))>0){
        // writing to file
        write(fout, buffer, strlen(buffer));
        printf("%s", buffer);

        // processing input
        for (int i = 0; i < len && buffer[i] != '\0'; i++){
            num_bytes += 1;
            if (buffer[i] == ',' || buffer[i] == ';' || 
            buffer[i] == ':' || buffer[i] == '.' || 
            buffer[i] == ' ' || buffer[i] == '\t'){
                flag=1;
            }
            else if(flag){
                num_words += 1;
                flag = 0;
            }
        }
    }

    if(len <=0 && num_bytes == 0){
        error("ERR 01: File Not Found");
    }
    else{
        printf("\nThe file transfer is successful. Words: %d, Bytes: %d\n", num_words, num_bytes );
    }
    close(sockid); close(fout);
    exit(1);
}