#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

vector<Point2f> userparameter;							// To store the points clicked by user
void mousefunction(int event,int x,int y,int flags,void* parameters)		// To record every left click
{
	if(event == EVENT_LBUTTONDOWN)	
		userparameter.push_back(Point2f(x,y));
}

int main( int argc,char** argv)
{
	if(argc==1){cout<<"No image selected";return -1;}			
	if(argc>2){cout<<"More than one image selected";return -1;}
	string path(argv[1]);							//path variable to hold location of image
	userparameter.resize(0);
	Mat input_img;		
	input_img = imread(path);
	if(!input_img.data)
	{
		cout<<"Image not found, you can download from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/empty.jpg or https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/traffic.jpg or simply name path variable in code \n";
		return -1;
	}
	imshow("Original Frame", input_img);
	int parameter = 50;							// max number of mouse calls
  	setMouseCallback("Original Frame",mousefunction,&parameter);
  	waitKey(0);								// press any key to proceed
  	destroyAllWindows();							// destroyes all opened windows
  	
  	string name = "./" + path.substr(0,path.length() - 4);			// for naming the image stored.

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
	
	Mat homo_img;								// intermediate homographic image
	Mat matrix = getPerspectiveTransform(userparameter,finalparameter);
	warpPerspective(input_img,homo_img,matrix,input_img.size()); 	
	imshow("Warpped Source Image",homo_img);
	waitKey(0);
	
	
	bool homo_success = imwrite(name+"_wrapped.jpg",homo_img);
	if(homo_success==false)						//notifes user if failed save operation
	{
		cout<<"Failed to save the image\n";
	}	
	
	destroyAllWindows();
	
	Mat final_img;								//final cropped image
	Rect crop_region(472,52,328,778);					
	final_img = homo_img(crop_region);
	imshow("Final Image",final_img);
	
	bool final_success = imwrite(name+"_final.jpg",final_img);
	if(final_success==false)
	{
		cout<<"Failed to save the cropped image\n";
	}
	
	waitKey(0);
	destroyAllWindows();	
	
	return 0;
}
