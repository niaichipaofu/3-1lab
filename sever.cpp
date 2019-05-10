/*
 * @Author: yyk 
 * @Date: 2019-03-19 20:31:01 
 * @Last Modified by:   yyk 
 * @Last Modified time: 2019-03-19 20:31:01 
 */
#include <iostream>
#include "sever_function.h"
#include"Control_cloud_platform.h"
#include"Lookface.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/msg.h>

int main(int argc, char **argv)
{
            int msqid;
            if((msqid=msgget(IPC_PRIVATE,0700))<0)
            {
            printf("msgget Create faild! \n");
            return 1;
             }  
           pid_t id = fork();
                if(id < 0)
                { 
                   perror("fork error");
                   exit(EXIT_FAILURE);
                }
                 else if(id == 0) //子进程
                { 
                  autolookface(argc ,argv,&msqid);
                // wait(&id);
                }
                   else
                {
             //      wait(&id);
                }
Sockprocess(&msqid);
       
    return 0;
}
