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
#include <chrono>
using namespace std;
using namespace std::chrono;


#define BUFFER_SIZE 50            // maxisize of input file name
#define PORT 12000                // port address to bind
#define MAX_CON 5                 // maximum number of waiting connections in queue
#define PACKET_SIZE 50            // size of packets

#define KRED  "\x1B[31m"
#define KNRM  "\x1B[0m"
#define PROMPT printf("--------------\n");

map<string, int>fdMap, last_used;

struct user_info{
    string name, ip_address = "127.0.0.1";
    int port;
} portMap[5];
void init_map(){

    portMap[0].name = "Jan";
    portMap[1].name = "Michael";
    portMap[2].name = "Jim";
    portMap[3].name = "Toby";
    portMap[4].name = "Dwight";
    
    portMap[0].port = 12000;
    portMap[1].port = 12001;
    portMap[2].port = 12002;
    portMap[3].port = 12003;
    portMap[4].port = 12004;

    fdMap["Jan"] = -1;
    fdMap["Michael"] = -1;
    fdMap["Jim"] = -1;
    fdMap["Toby"] = -1;
    fdMap["Dwight"] = -1;
    
}

void errorExit(char *msg){            // prints error message to stderr and exits
    printf("%s%s%s\n", KRED, msg, KNRM);
    exit(1);
}

void error(char *msg){            // prints error message to stderr
    printf("%s%s%s\n", KRED, msg, KNRM);
    PROMPT;
}

float get_time(time_point<system_clock> start){
    duration<double> elapsed_seconds = system_clock::now() - start;
    return (float)elapsed_seconds.count();
}

void sig_handler(int);
string findbyport(int);
int findbyname(string);
pair<string,string> split(char *, int);