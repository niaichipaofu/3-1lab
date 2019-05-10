 CXX=g++
Objects=sever_function.o sever.o Get_bmpdata.o Control_cloud_platform.o Lookface.o
exe:$(Objects)
	g++  $(Objects) -o exe -l realsense2 -lwiringPi `pkg-config --libs --cflags opencv4` -std=c++11
sever_function.o:sever_function.cpp 
	g++ -c sever_function.cpp -o sever_function.o 
sever.o:sever.cpp 
	g++ -c sever.cpp -o sever.o  -std=c++11 
Get_bmpdata.o: Get_bmpdata.cpp  
	g++ -c Get_bmpdata.cpp  -o Get_bmpdata.o -l realsense2   -std=c++11 
Control_cloud_platform.o: Control_cloud_platform.cpp
	g++ -c Control_cloud_platform.cpp -o Control_cloud_platform.o -lwiringPi
Lookface.o:Lookface.cpp
	g++ -c Lookface.cpp -o Lookface.o `pkg-config --libs --cflags opencv4` -std=c++11
clear :
	rm $(Objects) exe
