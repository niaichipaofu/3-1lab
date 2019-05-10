#include"Control_cloud_platform.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
 #include <cstdio>
#include<malloc.h>

#define level 600
#define vertical 700

char UsB[] = "/dev/ttyUSB0";
char Instructions[20];




int get_instruction_to_control_flog(char *instruction,int *number_l,int *number_v)
{
 int flog_l=0;
 int flog_v=0;

	for(int i=0;instruction[i]!='\0';i++)
	{
		if((instruction[i]>='a'&&instruction[i]<='z')||(instruction[i]>='A'&&instruction[i]<='Z'))
		{
			if(((instruction[i]=='l')||(instruction[i]=='L'))&&flog_l==0)
			{
				if(flog_v==0)
				flog_l+=1;
				else
					flog_l+=10;
 
			}
			if(((instruction[i]=='v')||(instruction[i]=='V'))&&flog_v==0)
			{
				flog_v+=2;
			}
			
		}
		else if(instruction[i]>47&&instruction[i]<58)
		{
			if(flog_l>0&&flog_v==0)
			{
				*number_l=*number_l*10+instruction[i]-'0';
							
			}
			else if(flog_v>0)
			{
				*number_v=*number_v*10+instruction[i]-'0';

			}

		}

       
	}
    return flog_l+flog_v;
}

int get_instruction_to_control__(char *instruction)
{
    int number_l=0,number_v=0;
	int flog=get_instruction_to_control_flog(instruction,&number_l,&number_v);
     if(flog==1)
     {
     if(Control_cloud_platform_single(UsB,3,number_l,100))
        return 1;
     }
     else if(flog==2)
     {
         if(Control_cloud_platform_single(UsB,1,number_v,100))
         return 1;
     }
     else if(flog==3)
     {
         if(Control_cloud_platform_double(UsB,number_l,number_v,100))
         return 1;
     }
     else
     {
         return 1;
     }

     return 0;
     


}









int angle___(float angle ,int initial)
{

   return angle*10+initial;

}

int Control_cloud_platform_double(char * Device_number,int  angle_level, int angle_vertical,  int time)
{
    int fd;
    int initial;
 

    if(angle_level>=180)	
    {
        angle_level=angle___(180,level);
    }
    else  if(angle_level<=0)
    {	
        angle_level=angle___(0,level);
    }
    else	if(angle_level<180&&angle_level>0)
    {
        angle_level = angle___(angle_level,level);
    }
    if(angle_vertical>=180)	
    {
        angle_vertical=angle___(180,vertical);
    }
    else  if(angle_vertical<=0)
    {	
        angle_vertical=angle___(0,vertical);
    }
    else	if(angle_vertical<180&&angle_vertical>0)
    {
        angle_vertical = angle___(angle_vertical,vertical);
    }
    
   // char *Instructions=(char *)malloc(sizeof(char)*20);
    sprintf(Instructions, "#1P%d#3P%dT%d\r\n",angle_vertical,angle_level,time);
if ((fd = serialOpen(Device_number, 9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1 ;
    } 
        serialPuts(fd, Instructions);
        delay(1000);
        serialFlush(fd);
        serialClose(fd) ;
      
    
        return 0;
}

int Control_cloud_platform_single(char * Device_number,int direction, int angle,  int time)
{
    int fd;
    int initial;
    if(direction==1)initial=vertical;

    if(direction==3)initial=level;

    if(angle>=180)	angle=angle___(180,initial);
    else  if(angle<=0)	angle=angle___(0,initial);
    else		angle = angle___(angle,initial);

    
    sprintf(Instructions, "#%dP%dT%d\r\n",direction,angle,time);

    if ((fd = serialOpen(Device_number, 9600)) < 0)
    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
        return 1 ;
    } 
        serialPuts(fd, Instructions);
        delay(1000);
        serialFlush(fd);
        serialClose(fd) ;
       
        return 0;
}
/*
int main()
{

char str[20];
while(1)
{
scanf("%s",str);
 get_instruction_to_control__(str);
}

}*/
