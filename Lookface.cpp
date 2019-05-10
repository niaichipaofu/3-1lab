#include"Lookface.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include"Control_cloud_platform.h"
#include <unistd.h>
#include <iostream>
#include<sys/msg.h>


using namespace std;
using namespace cv;

double degree_l=90;
double degree_v=10;
double Offset_x ;
double Offset_y;
double delta_degree_l;
double delta_degree_v;

#define level 600
#define vertical 700

struct Msgbuf
{
    int mtype;
    char mtext[10];
}recv_buf;

void Getfacecenter(Mat frame);
double fabs(double x);
short Compare_st(char *str1,char *str2) ;


CascadeClassifier face_cascade;
//CascadeClassifier eyes_cascade;
short autolookface(int argc, char**argv,int *fd)
{
	 Control_cloud_platform_single("/dev/ttyUSB0",3,90 ,100);
	CommandLineParser parser(argc, argv,
		"{help h||}"
		"{face_cascade|haarcascade_frontalface_alt.xml|Path to face cascade.}"
		"{camera|0|Camera device number.}");

	String face_cascade_name = parser.get<String>("face_cascade");
    int camera_device = parser.get<int>("camera");

	if (!face_cascade.load(face_cascade_name))
	{
		cout << "Error loading face cascade\n";
		return 1;
	};
	
	
	VideoCapture capture;
	Mat frame;
 
  char s[2];	
	while (1)
	{
       
        int len;
         if((len=msgrcv(*fd,&recv_buf,10,0,IPC_NOWAIT))<0)
		    {
		      ;
		    }

		if(Compare_st(recv_buf.mtext,"openauto"))
		{
			capture.open(camera_device);
			if (!capture.isOpened())
			{
				cout << "Error opening video capture\n";
				return 1;
			}
				capture>>frame;
				//-- 3. Apply the classifier to the frame
			Getfacecenter(frame);
		     /* if( waitKey(10) == 27 )
				{
					break; // escape
				}*/
			capture.release();
                 
     
		}

		 
	}
	return 0;
}
void Getfacecenter(Mat frame)
{
	Mat frame_gray;
	double Interface_x=frame.cols/2,Interface_y=frame.rows/2;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);
	//-- Detect faces
	std::vector<Rect> faces;
	face_cascade.detectMultiScale(frame_gray, faces);
	double face_center_x=0.0,face_center_y=0.0;
	double face_size=0.0,face_size_def=0;
	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		face_center_x+=(faces[i].x + faces[i].width / 2)/faces.size();
		face_center_y+=(faces[i].y + faces[i].width / 2)/faces.size();	
	    //cout<<faces[i]<<endl;
		//ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);
	}
if(faces.size()!=0)
{
    Offset_x =-(face_center_x-Interface_x);
	//if(fabs(Offset_x)-100>0)
//{
	Offset_y =-(face_center_y-Interface_y);
	delta_degree_l  = 0.078125*Offset_x;
	delta_degree_v  = 0.078125*Offset_y;
	degree_l=degree_l+delta_degree_l;
    degree_v = degree_v+delta_degree_v;
    Control_cloud_platform_single("/dev/ttyUSB0",3,degree_l ,100);
   // Control_cloud_platform_single("/dev/ttyUSB0",1,degree_v ,100);

//	cout<<degree_l<<" "<<delta_degree_v<<endl;
//}
}
     //cout<<face_center_x<<" "<<face_center_y<<endl;
//imwrite("Q_rs",frame);
//	imshow("Capture - Face detection", frame);
}



short Compare_st(char *str1,char *str2)         //比较字符串是否相等
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
