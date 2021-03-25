#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<fstream>
#include<thread>
using namespace cv;
using namespace std;
using namespace std::chrono;

vector<Point2f> userparameter;							// To store the points clicked by user
void mousefunction(int event,int x,int y,int flags,void* parameters)		// To record every left click
{
	if(event == EVENT_LBUTTONDOWN)	
		userparameter.push_back(Point2f(x,y));
}

void consecutive(string video,VideoCapture cap[],int index,int total,float queue_density[],float dynamic_density[],Mat back_final,Mat matrix)
{
	cap[index].open(video);
	if (cap[index].isOpened() == false)  
	{
		cout << "Video file not found, you can download it from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/trafficvideo.mp4 or simply name path variable in code" << endl;
		return ;
	}
	Rect crop_region(472,52,328,778);

	bool done = true;
	int framenum = 0;					
		
	Scalar pixels;						//sum of pixels in subtracted image for queue_density
	Scalar dynamic_pixels;					// sum of pixels in subtracted image for dynamic_density
	Mat previous_frame = back_final;			//stores img of previous frame.
		
	while(done)
	{
		Mat frame,frame_homo,frame_final;
	    done = cap[index].read(frame);
	    if(!done) break;					//video is finished.
	    
	    if(framenum%total==index-1)
	    {
	    	warpPerspective(frame,frame_homo,matrix,frame.size());
	    	frame_final = frame_homo(crop_region);			//frame after wrapping and cropping
	    	previous_frame = frame_final;
	    }
	    else if(framenum%total==index)
	    {	
	    	warpPerspective(frame,frame_homo,matrix,frame.size());
	    	frame_final = frame_homo(crop_region);			//frame after wrapping and cropping
	    	    	
	    	Mat img = abs(frame_final - back_final) > 50;		//Subtract background and consider part with diff grater than 50
	    	Mat dynamic_img = abs(frame_final - previous_frame)>50;//Subtract previous frame and consider part with diff greaer than 50
	    	//previous_frame = frame_final;					//Set current frame to be previous for next frame
	    	
	    	pixels = sum(img);
	    	dynamic_pixels = sum(dynamic_img);
	    	
	    	queue_density[framenum] = ((pixels[0]+pixels[1]+pixels[2]));		//We assumed queue density will be proportional to number of poxels that are different in the 2 images
	    	dynamic_density[framenum] = (dynamic_pixels[0]+dynamic_pixels[1]+dynamic_pixels[2]);//And dynamic density will be proportional to the pixels that are changed in the 2 consecutive frames
	    	
			//if(framenum == 5175) imwrite("empty.jpg",frame); 			 For capturing empty frame  		    		    	
	    	//imshow("video_queue", img);
	    	//imshow("video_dynamic", dynamic_img);
	    }

		if (waitKey(10) == 27 || framenum==325)		//for testing purposes break at 100 seconds
		{
			cout << "Esc key is pressed by user. Stopping the video" << endl;
		   	break;
		}
		
		framenum = framenum+1;
	}
}



int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		cout<<"Expected 2 variables 1st one path of empty image and second the path of video. Empty image submitted was taken from 5:45 from given video";
		return -1;
	}
	string empty(argv[1]);
	string video(argv[2]);
	Mat background;		
	background = imread(empty);
	if(!background.data)
	{
		cout<<"Image not found, you can download from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/empty.jpg or simply name path variable in code \n";
		return -1;
	}
	
	imshow("Original Frame",background);
	int parameter = 50;							// max number of mouse calls
  	setMouseCallback("Original Frame",mousefunction,&parameter);
  	waitKey(0);								// press any key to proceed
  	destroyAllWindows();							// destroyes all opened windows    
  	
  	if(userparameter.size()<4)						// ensures 4 points are considered
  	{
  		cout<<"Lesser points selected than expected.. Terminating";
  		return -1;
  	}
  	if(userparameter.size()>4)					       
  	{
  		cout<<"More points selected than expected.. Taking first four points into account";
  	}
  	userparameter.resize(4);
  	
	vector<Point2f> finalparameter;					//vector for destination coordinates
	finalparameter.push_back(Point2f(472,52));
	finalparameter.push_back(Point2f(472,830));
	finalparameter.push_back(Point2f(800,830));
	finalparameter.push_back(Point2f(800,52));
	
	Mat back_homo,back_final;					// intermediate homographic image,final cropped image
	Mat matrix = getPerspectiveTransform(userparameter,finalparameter);
	warpPerspective(background,back_homo,matrix,background.size()); 
	Rect crop_region(472,52,328,778);					
	back_final = back_homo(crop_region);
	destroyAllWindows();		    
	    
	int total = 4;
	//std::thread mythreads[total];
	VideoCapture cap[total];
	float queue_density[6000];
	float dynamic_density[6000];

	auto start = high_resolution_clock::now();
	std::thread mythread1(consecutive,video,cap,0,total,queue_density,dynamic_density,back_final,matrix);
	std::thread mythread2(consecutive,video,cap,1,total,queue_density,dynamic_density,back_final,matrix);
	std::thread mythread3(consecutive,video,cap,2,total,queue_density,dynamic_density,back_final,matrix);
	std::thread mythread4(consecutive,video,cap,3,total,queue_density,dynamic_density,back_final,matrix);

	if(mythread1.joinable()) mythread1.join();
	if(mythread2.joinable()) mythread2.join();
	if(mythread3.joinable()) mythread3.join();
	if(mythread4.joinable()) mythread4.join();
	

	cout<<"Sec,Queue,Dynamic"<<endl;
	int framenum = 0;
	while(framenum<325)
	{
		cout<<float(framenum/15)<<fixed<<','<<queue_density[framenum]/(1.25e6)<<','<<dynamic_density[framenum]/(2.5e5)<<endl;
		framenum++;
	}
	
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "\nTime taken by function: "
         << duration.count()/(1e6) << " seconds" << endl;
	//myfile.close();	
	return 0;
}
