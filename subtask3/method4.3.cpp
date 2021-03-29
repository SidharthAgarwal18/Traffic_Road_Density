#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include<fstream>
#include<pthread.h>
#include<unistd.h>
using namespace cv;
using namespace std;
using namespace std::chrono;

int total;
float queue_density[6000];
float dynamic_density[6000];
int esc = 0;
VideoCapture cap;
int framenum = 0;

class forparallel
{
public:
	int index;
	Mat back_final;
	Mat matrix;
};

void* consecutive(void* arg)
{
	forparallel n = *((forparallel*)arg);
	int index = n.index;
	Mat back_final = n.back_final;
	Mat matrix = n.matrix;
	
	Rect crop_region(472,52,328,778);

	bool done = true;				
		
	Scalar pixels;						//sum of pixels in subtracted image for queue_density
	Scalar dynamic_pixels;					// sum of pixels in subtracted image for dynamic_density
	Mat previous_frame = back_final;			//stores img of previous frame.
		

	while(framenum < 5737)
	{
	    if(framenum%total==index)
	    {	
	    	Mat frame,frame_homo,frame_final;
	    	done = cap.read(frame);
	    	framenum++;
	    	warpPerspective(frame,frame_homo,matrix,frame.size());
	    	frame_final = frame_homo(crop_region);			//frame after wrapping and cropping
	    	    	
	    	Mat img = abs(frame_final - back_final) > 50;		//Subtract background and consider part with diff grater than 50
	    	Mat dynamic_img = abs(frame_final - previous_frame)>50;//Subtract previous frame and consider part with diff greaer than 50
	    	previous_frame = frame_final;					//Set current frame to be previous for next frame
	    	
	    	pixels = sum(img);
	    	dynamic_pixels = sum(dynamic_img);
	    	
	    	queue_density[framenum] = ((pixels[0]+pixels[1]+pixels[2]));		//We assumed queue density will be proportional to number of poxels that are different in the 2 images
	    	dynamic_density[framenum] = (dynamic_pixels[0]+dynamic_pixels[1]+dynamic_pixels[2]);//And dynamic density will be proportional to the pixels that are changed in the 2 consecutive frames
	    	
	    	//if(index == total-1) cout << "a";
			//if(framenum == 5175) imwrite("empty.jpg",frame); 			 For capturing empty frame  		    		    	
	    	//imshow("video_queue", img);
	    	//imshow("video_dynamic", dynamic_img);
	    }
	}
	//if(index == total-1) cout << "a";
	return NULL;
}



int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		cout<<"Expected 2 variables 1st one path of empty image and second the path of video, and third for number of threads to split into.. Empty image submitted was taken from 5:45 from given video";
		return -1;
	}
	string empty(argv[1]);
	string video(argv[2]);
	total = atoi(argv[3]);
	Mat background;		
	background = imread(empty);
	if(!background.data)
	{
		cout<<"Image not found, you can download from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/empty.jpg or simply name path variable in code \n";
		return -1;
	}
	
	vector<Point2f> userparameter;							// To store the points clicked by user
	userparameter.push_back(Point2f(1000,218));
	userparameter.push_back(Point2f(461,897));
	userparameter.push_back(Point2f(1521,924));
	userparameter.push_back(Point2f(1278,205));
  	
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

	auto start = high_resolution_clock::now();

	pthread_t ptid[total-1];
	forparallel n[total];

	cap.open(video);
	if (cap.isOpened() == false)  
	{
		cout << "Video file not found, you can download it from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/trafficvideo.mp4 or simply name path variable in code" << endl;
		return -1;
	}
	framenum = 0;
	esc = 0;
	for(int i=0;i<total-1;i++)
	{
		n[i].index = i;
		n[i].back_final = back_final;
		n[i].matrix = matrix;
		pthread_create(&(ptid[i]), NULL, &consecutive, &(n[i]));
	}
	n[total - 1].index = total - 1;
	n[total - 1].back_final = back_final;
	n[total - 1].matrix = matrix;
	consecutive(&(n[total-1]));
	for(int i=0;i<total-1;i++)
	{
		pthread_join((ptid[i]),NULL);
	}
	
	//freopen("out4.txt","w",stdout);

	cout<<"Sec,Queue,Dynamic"<<endl;
	int framenum = 0;
	while(framenum<5737)
	{
		cout<<((float)framenum)/15<<fixed<<','<<queue_density[framenum]/(1.25e6)<<','<<dynamic_density[framenum]/(2.5e5)<<endl;
		framenum++;
	}
	
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken by function in seconds:\n"
         << duration.count()/(1e6)<<endl;
	//myfile.close();	
	return 0;
}