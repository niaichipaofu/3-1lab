#include "sever_function.h"
#include"Get_bmpdata.h"
#include"Macro.h"
#include"Control_cloud_platform.h"
#include"Lookface.h"
#include<sys/msg.h>
#include <cstring>


struct Msgbuf
{
    int mtype;
    char mtext[10];
}send_buf;

short Compare_str(char *str1,char *str2);

void Sockprocess(int *msqid) 
{
	  
	
    Process_information process  = Initialization();    //获取已初始化的套接字
send_buf.mtype=1;
strcpy(send_buf.mtext,"openauto");
 	if(msgsnd(*msqid,&send_buf,10,IPC_NOWAIT)<0)
	 {
		   printf("msgsend faild !\n");
	}
    while(1)
    {
        while(acceptconnect(process))  //等待客户端连接，并阻塞等待
        {
             printf("get a new client,%s:%d\n", inet_ntoa(process.client_information.sin_addr), ntohs(process.client_information.sin_port));
            pid_t id = fork();
                if(id < 0)
                { 
                    close(process.clientfd);
                }
                else if(id == 0) //子进程
                { 
                    close(process.serverfd);
                    if(fork() > 0)
                    { 
                        _exit(0);
                    }
                    else if(id == 0)
                    { 
                            //Get_bmp();                  //保存图片
                    	char *Instru__;
                    		mode: Instru__=Get_Instruction(process);
                                if(Compare_str(Instru__,Control))
                                {				strcpy(send_buf.mtext,"Closeauto");
                                           if(msgsnd(*msqid,&send_buf,10,IPC_NOWAIT)<0)
									            {
									               printf("msgsend faild !\n");
									            }
                                	while(1)
                                	{
                                		Instru__=Get_Instruction(process);
                                		if(Compare_str(Instru__,EXIT))
                                		{
											   strcpy(send_buf.mtext,"openauto");
                                           if(msgsnd(*msqid,&send_buf,10,IPC_NOWAIT)<0)
									            {
									               printf("msgsend faild !\n");
									            }
                                			goto mode;
                                        }
										else if(Compare_str(Instru__,Get))
										{
                                           
										 if(!Send_file(process,(char *)"Q_rs"))   //发送数据至客户端
                                  		  { 
                                    	    printf("successful!\n");
                                   	   	  }

										}
                             			  else
                                           {
										get_instruction_to_control__((char *)Instru__);
											}	                                
										}
                                }
                             else  if(Compare_str(Instru__,Get))
                             {
                             	  
                                 if(!Send_file(process,(char *)"Q_rs"))   //发送数据至客户端
                                    { 
                                        printf("successful!\n");
                                    }
                                   goto mode;
                             }
                             else if(Compare_str(Instru__,EXIT))
                             {
								close(process.clientfd);
                                wait(&id);
                             	_exit(0);
                             }

                    }
                    close(process.clientfd);
                     wait(&id);
                }
                else
                { 
                    close(process.clientfd);
                   wait(&id);
                }
                
                                
         }

    }
          
}
char instruction[20];
//bool CleanProcess()
char * Get_Instruction(Process_information &process)  //获取指令
{
    bzero(instruction,20);
    if(recv(process.clientfd,instruction,20,0)>1)
    	return instruction;
     else
     	return EXIT;
}

void saferFree(void **pp)   //有待深入理解的释放内存的好方法
{
    if(pp!=NULL&&*pp!=NULL)
    {
        free(*pp);
        *pp= NULL;
    }
}

short  Establish(int *serverfd)         //初始化套接字
{
    if((*serverfd=socket(AF_INET,SOCK_STREAM,0))== -1)
        return 1;
    else
        return 0;
}

struct   sockaddr_in Addressinsert()  //配置基本信息
{
    struct   sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    return server_addr;
}

short Information_input(int *serverfd)   //套接字绑定基本信息
{
    struct   sockaddr_in  server_addr =  Addressinsert();
    if(bind(*serverfd,(struct sockaddr*)&server_addr,sizeof(server_addr))== -1)
        return 1;
    else
        return 0;
}

short Listening(int *serverfd)   //监听端口
{
    if(listen(*serverfd,25)==-1)
        return 1;
    else
        return 0;
}

struct Process_information Initialization()   //监听的前序工作
{

    int serverfd=0;
    Process_information process;
     while(Establish(&serverfd))   //初始化套接字
    {
        printf("[Socket establish fail]\n");
        sleep(5);
    }

    while(Information_input(&serverfd))  //套接字信息的录入
    {
        printf("[Bind fail] socket =%d\n",serverfd);
    }

     while(Listening(&serverfd))   //开启监听
    {
        printf("Listening fail\n");
    }
    printf("Listening....... !!!\n");
    process.serverfd = serverfd;
    return process;
}

void acceptdata(Process_information &process, char recever[])   //接收数据，有待改进，未启用
{
     int c=recv(process.clientfd,recever,File_max,0);
            if(c!=-1)
            {

            }
    return;
}
short Senddata(Process_information &process,char Send[])    //发送数据，有待改进，未启用
{

    if(send(process.clientfd,Send,File_max,0)<0)
        return 1;
    else
       return 0;
}

short acceptconnect(Process_information  &process)    //接收客户端连接
{
    socklen_t clientaccept=sizeof(process.clientfd);
     process.clientfd = accept(process.serverfd,( struct sockaddr*)&process.client_information,&clientaccept);
     if(process.clientfd!=-1)
        return 1;
     else
        return 0;
}


short Compare_str(char *str1,char *str2)         //比较字符串是否相等
{
    int compare =0;
    while(!(compare=*str1 - *str2)&&*str2)
    {
        str1++;
        str2++;
    }
    if(!compare)
        return 1;
    return 0;

}


short Send_file(Process_information &process,char *file_path__)   //发送bmp数据至客户端
{
    FILE *fp;
    char *file_data =(char *)malloc(sizeof(char)*File_max);
    int i=0;

    if((fp=fopen(file_path__,"rb"))==NULL)
    {
        printf("[file open faild]\n");
        return 1;
    }
 int l=0;

    while(!feof(fp))
    {

         file_data[i]=fgetc(fp);

        if(i==File_max)
        {
            while((l=send(process.clientfd,file_data,File_max+1,0))<0)
            {
                         printf("Send fail\n");
                         sleep(5);
            }

              i=-1;
        }


         i++;
    }


    if(i>0)
    {

        while((l=send(process.clientfd,file_data,i+1,0))<0)
                    {
                         printf("Send fail\n");
                         sleep(5);
                    }

    }
    //free(file_data);
    safeFree(file_data);
    fclose(fp);
    //close(process.clientfd);
    return 0;

}
