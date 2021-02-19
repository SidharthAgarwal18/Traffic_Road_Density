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
	if(argc==1){cout<<"No option selected";return -1;}
	if(argc>2){cout<<"More than one option selected";return -1;}
	string path(argv[1]);
	userparameter.resize(0);
	if(path=="exit") return -1;
	Mat image;
	image = imread(path);
	if(!image.data)
	{
		cout<<"Image not found, you can download from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/empty.jpg or https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/traffic.jpg or simply name path variable in code \n";
		return -1;
	}
	namedWindow("Original Frame", WINDOW_AUTOSIZE);
	imshow("Original Frame", image);
	int parameter = 5;
  	setMouseCallback("Original Frame",mousefunction,&parameter);
  	waitKey(0);
  	for(int i=0;i<5;i++)
  	{
  		cout<<userparameter[i];
  	}
    return 0;

}
