#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

vector<Point2f> userparameter;
void mousefunction(int event,int x,int y,int flags,void* parameters)
{
	if(event == EVENT_LBUTTONDOWN)
		userparameter.push_back(Point2f(x,y));
}

int main( int argc,char** argv)
{
	if(argc==1){cout<<"No image selected";return -1;}
	if(argc>2){cout<<"More than one image selected";return -1;}
	string path(argv[1]);
	userparameter.resize(0);
	if(path=="exit") return -1;
	Mat input_img;
	input_img = imread(path);
	if(!input_img.data)
	{
		cout<<"Image not found, you can download from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/empty.jpg or https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/traffic.jpg or simply name path variable in code \n";
		return -1;
	}
	namedWindow("Original Frame", WINDOW_AUTOSIZE);
	imshow("Original Frame", input_img);
	int parameter = 5;
  	setMouseCallback("Original Frame",mousefunction,&parameter);
  	waitKey(0);
  	destroyAllWindows();
  	
  	string name = "./" + path.substr(0,path.length() - 4);	// for naming the image stored.
  	
  	userparameter.resize(4);
	vector<Point2f> finalparameter;			//vector for destination coordinates
	finalparameter.push_back(Point2f(472,52));
	finalparameter.push_back(Point2f(472,830));
	finalparameter.push_back(Point2f(800,830));
	finalparameter.push_back(Point2f(800,52));
	
	Mat homo_img;
	Mat matrix = getPerspectiveTransform(userparameter,finalparameter);
	warpPerspective(input_img,homo_img,matrix,input_img.size()); 	
	imshow("Warpped Source Image",homo_img);
	waitKey(0);
	
	
	bool homo_success = imwrite(name+"_wrapped.jpg",homo_img);
	if(homo_success==false)
	{
		cout<<"Failed to save the image\n";
		return -1;
	}	
	
	destroyAllWindows();
	
	Mat final_img;
	Rect crop_region(472,52,328,778);
	final_img = homo_img(crop_region);
	imshow("Final Image",final_img);
	
	bool final_success = imwrite(name+"_final.jpg",final_img);
	if(final_success==false)
	{
		cout<<"Failed to save the cropped image\n";
		return -1;
	}
	
	waitKey(0);
	destroyAllWindows();	
	
	return 0;
}
