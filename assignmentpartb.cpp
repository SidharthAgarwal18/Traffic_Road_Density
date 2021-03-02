#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
    Mat first, second,fg,sg;
    first = imread("./empty.jpg");
    VideoCapture cap("./trafficvideo.mp4");
    //namedWindow(w, WINDOW_NORMAL);
    while(true)
    {
    	Mat frame;
    	bool done = cap.read(frame);
    	if(!done) break;
    	Mat img = abs(frame - first) > 50;
    	cout<<"a";
    	imshow("vid", img);
    	if (waitKey(10) == 27)
  {
   cout << "Esc key is pressed by user. Stoppig the video" << endl;
   break;
  }

    }
    waitKey(0);
    return 0;
}