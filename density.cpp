#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
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
	int previous_pixels = 0;
	int dynamic_density = 0;
	Scalar pixels;
	
	cout<<"Framename\tQueue_density\tDyanmic_density";
	while(done)
	{
		Mat frame,frame_homo,frame_final;
	    	done = cap.read(frame);
	    	if(!done) break;
	    	
	    	warpPerspective(frame,frame_homo,matrix,frame.size());
	    	frame_final = frame_homo(crop_region);
	    	    	
	    	Mat img = abs(frame_final - back_final) > 50;
	    	pixels = sum(img);
	    	queue_density = (pixels[0]+pixels[1]+pixels[2])/30000;
	    	dynamic_density = abs((queue_density - previous_pixels)*100);
	    	previous_pixels = queue_density;
	    		    		    	
	    	imshow("vid", img);
		if (waitKey(10) == 27)
		{
			cout << "Esc key is pressed by user. Stopping the video" << endl;
		   	break;
		}
		cout<<framenum/15<<"\t"<<queue_density<<"\t"<<dynamic_density<<endl;
		framenum = framenum+1;
	}
	
	return 0;
}
