#ifndef MACRO_H_INCLUDED
#define MACRO_H_INCLUDED
#include<string.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<malloc.h>
#include<dirent.h>
#include<sys/wait.h>





#define SERVER_PORT 9514
#define File_max 1048576
#define Instruction_size 10
#define safeFree(p) saferFree((void**)&(p))
char Control[]="Control";
char Get[]="Get";
char Obtain[]="Obtain";
char ok[]="Successful";
char nook[]="Faild";
char EXIT[]="exit";


struct Process_information
{
    int serverfd=-1;
    int clientfd=-1;
    struct sockaddr_in client_information;
};

#endif // MACRO_H_INCLUDED
