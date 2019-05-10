#ifndef CONTROL_CLOUD_PLATFORM_H_INCLUDE
#define  CONTROL_CLOUD_PLATFORM_H_INCLUDE

int angle___(float angle ,int initial);
int Control_cloud_platform_single(char * Device_number,int direction, int angle,  int time);
int Control_cloud_platform_double(char * Device_number,int  angle_level, int angle_vertical,  int time);
int get_instruction_to_control_flog(char *instruction,int *number_l,int *number_v);
int get_instruction_to_control__(char *instruction);



#endif // !CONTROL_CLOUD_PLATFORM_H_INCLUDE
