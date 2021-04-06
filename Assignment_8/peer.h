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

map<string, int> portMap, fdMap, last_used;

void init_map(){
    portMap["Jan"] = 12000;
    portMap["Michael"] = 12001;
    portMap["Jim"] = 12002;
    portMap["Toby"] = 12003;
    portMap["Dwight"] = 12004;

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
pair<string,string> split(char *, int);