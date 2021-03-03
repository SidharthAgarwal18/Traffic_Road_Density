#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<fstream>
using namespace cv;
using namespace std;

vector<Point2f> userparameter;							// To store the points clicked by user
void mousefunction(int event,int x,int y,int flags,void* parameters)		// To record every left click
{
	if(event == EVENT_LBUTTONDOWN)	
		userparameter.push_back(Point2f(x,y));
}

int main(int argc, char* argv[])
{
	Mat background;		
	background = imread("./empty.jpg");
	if(!background.data)
	{
		cout<<"Image not found, you can download from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/empty.jpg or https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/traffic.jpg or simply name path variable in code \n";
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
	    
	VideoCapture cap("./trafficvideo.mp4");
	
	bool done = true;
	int framenum = 0;
	int queue_density = 0;
	int dynamic_density = 0;
	int avg_queue = 0;
	int avg_dynamic = 0;
	
	Scalar pixels;
	Scalar dynamic_pixels;
	Mat previous_frame = back_final;
	
	ofstream myfile("./graph.csv");
	myfile<<"Sec"<<','<<"Queue"<<','<<"Dyanmic"<<endl;
	
	while(done)
	{
		Mat frame,frame_homo,frame_final;
	    	done = cap.read(frame);
	    	if(!done) break;
	    	
	    	warpPerspective(frame,frame_homo,matrix,frame.size());
	    	frame_final = frame_homo(crop_region);
	    	    	
	    	Mat img = abs(frame_final - back_final) > 50;
	    	Mat dynamic_img = abs(frame_final - previous_frame)>50;
	    	previous_frame = frame_final;
	    	
	    	pixels = sum(img);
	    	dynamic_pixels = sum(dynamic_img);
	    	
	    	queue_density = ((pixels[0]+pixels[1]+pixels[2])/30000);
	    	dynamic_density = (dynamic_pixels[0]+dynamic_pixels[1]+dynamic_pixels[2])/6000;
	    	
	    	avg_queue = avg_queue + queue_density;
	    	avg_dynamic = avg_dynamic + dynamic_density;
	    	
	    		
	    	if(framenum%15==0)
	    	{
	    		myfile<<framenum/15<<','<<avg_queue/15<<','<<avg_dynamic/15<<endl;
	    		avg_queue = 0;
	    		avg_dynamic = 0;
	    	}	
	    	    		    		    	
	    	imshow("vid", img);
	    	imshow("vid_dynamic", dynamic_img);
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stopping the video" << endl;
		   	break;
		}
		
		framenum = framenum+1;
	}
	myfile.close();	
	return 0;
}
