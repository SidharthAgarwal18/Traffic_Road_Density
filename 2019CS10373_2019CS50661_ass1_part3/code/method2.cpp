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
using namespace std::chrono;

int main(int argc, char* argv[])
{
	if(argc != 5)
	{
		cout<<"Expected 4 variables 1st one path of empty image and second the path of video and 3rd 4th for pixel per inch in vertical and horizontal dirns. Empty image submitted was taken from 5:45 from given video";
		return -1;
	}
	string empty(argv[1]);
	string video(argv[2]);
	float x_len = atoi(argv[3]);
	float y_len = atoi(argv[4]);

	Mat background_first,background;		
	background_first = imread(empty);

	if(!background_first.data)
	{
		cout<<"Image not found, you can download from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/empty.jpg or simply name path variable in code \n";
		return -1;
	}
	
	resize(background_first,background,Size(x_len,y_len));
	float ratio1 = x_len/1920;
	float ratio2 = y_len/1080;

	vector<Point2f> userparameter;							// To store the points clicked by user
	userparameter.push_back(Point2f(1000*ratio1,218*ratio2));
	userparameter.push_back(Point2f(461*ratio1,897*ratio2));
	userparameter.push_back(Point2f(1521*ratio1,924*ratio2));
	userparameter.push_back(Point2f(1278*ratio1,205*ratio2));
  	
	vector<Point2f> finalparameter;					//vector for destination coordinates
	finalparameter.push_back(Point2f(472*ratio1,52*ratio2));
	finalparameter.push_back(Point2f(472*ratio1,830*ratio2));
	finalparameter.push_back(Point2f(800*ratio1,830*ratio2));
	finalparameter.push_back(Point2f(800*ratio1,52*ratio2));
	
	    
	    
	VideoCapture cap(video);

	if (cap.isOpened() == false)  
	 {
	  cout << "Video file not found, you can download it from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/trafficvideo.mp4 or simply name path variable in code" << endl;
	  return -1;
	 }

	
	bool done = true;
	float framenum = 0;					// 15framenums = 1second
	float queue_density = 0;
	float dynamic_density = 0;
	float previous_dynamic = 0;
	//int avg_queue = 0;
	//int avg_dynamic = 0;
	
	Scalar pixels;						//sum of pixels in subtracted image for queue_density
	Scalar dynamic_pixels;					// sum of pixels in subtracted image for dynamic_density
	
	
	freopen("out2.txt", "w", stdout);		//To save csv in out.txt
	cout<<"Sec,Queue,Dynamic"<<endl;
	auto start = high_resolution_clock::now();

	Mat back_homo,back_final;					// intermediate homographic image,final cropped image
	Mat matrix = getPerspectiveTransform(userparameter,finalparameter);
	warpPerspective(background,back_homo,matrix,background.size()); 
	Rect crop_region(472*ratio1,52*ratio2,328*ratio1,778*ratio2);	
	back_final = back_homo(crop_region);
	Mat previous_frame = back_final;			//stores img of previous frame.
		
	while(done)
	{
		Mat frame_first,frame,frame_homo,frame_final;
	    done = cap.read(frame_first);
	    if(!done) break;					//video is finished.
	    
	    resize(frame_first,frame,Size(x_len,y_len));
	    //imshow("frame",frame);
	    warpPerspective(frame,frame_homo,matrix,frame.size());
	    frame_final = frame_homo(crop_region);			//frame after wrapping and cropping
	        	
	    Mat img = abs(frame_final - back_final) > 50;		//Subtract background and consider part with diff grater than 50
	    Mat dynamic_img = abs(frame_final - previous_frame)>50;//Subtract previous frame and consider part with diff greaer than 50
	    previous_frame = frame_final;					//Set current frame to be previous for next frame
	    
	    pixels = sum(img);
	    dynamic_pixels = sum(dynamic_img);
	    	
	    queue_density = ((pixels[0]+pixels[1]+pixels[2]));		//We assumed queue density will be proportional to number of poxels that are different in the 2 images
	    dynamic_density = (dynamic_pixels[0]+dynamic_pixels[1]+dynamic_pixels[2]);//And dynamic density will be proportional to the pixels that are changed in the 2 consecutive frames
	    //dynamic_density = 0.2*dynamic_density + 0.8*previous_dynamic;
		previous_dynamic = dynamic_density;

		cout<<framenum/15<<fixed<<','<<queue_density/(1.25e6)<<','<<dynamic_density/(2.5e5)<<endl;	
	   	//if(framenum == 5175) imwrite("empty.jpg",frame); 			 For capturing empty frame  		    		    	
	   	//imshow("video_queue", img);
	   	//imshow("video_dynamic", dynamic_img);
		if (waitKey(10) == 27)		//for testing purposes break at 100 seconds
		{
			cout << "Esc key is pressed by user. Stopping the video" << endl;
		   	break;
		}
		framenum++;
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken by function in seconds: \n"
         << duration.count()/(1e6) <<endl;
	//myfile.close();	
	return 0;
}