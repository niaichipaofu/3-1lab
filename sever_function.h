#ifndef SEVER_FUNCTION_H_
#define SEVER_FUNCTION_H_




void Sockprocess(int *msqid);  
short  Establish(int *serverfd);
struct   sockaddr_in Addressinsert();
short Information_input(int *serverfd);
short Listening(int *serverfd);
struct Process_information Initialization();
short acceptconnect(Process_information  &process);

void saferFree(void **pp);
short Send_file(Process_information &process,char *file_path__) ;
char * Get_Instruction(Process_information &process);  //获取指令
#endif
