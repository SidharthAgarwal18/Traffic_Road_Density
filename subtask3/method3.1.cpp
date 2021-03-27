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
float queue_density[6000][17];
float dynamic_density[6000][17];
int esc = 0;

class forparallel
{
public:
	string video;
	int index;
	Mat back_final;
	Mat matrix;
};

void* consecutive(void* arg)
{
	forparallel n = *((forparallel*)arg);
	int index = n.index;
	String video = n.video;
	Mat back_final = n.back_final;
	Mat matrix = n.matrix;
	//sleep(index);
	VideoCapture cap(video);
	if (cap.isOpened() == false)  
	{
		cout << "Video file not found, you can download it from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/trafficvideo.mp4 or simply name path variable in code" << endl;
		return NULL;
	}
	bool done = true;
	int framenum = 0;					
		
	Scalar pixels;						//sum of pixels in subtracted image for queue_density
	Scalar dynamic_pixels;					// sum of pixels in subtracted image for dynamic_density
	Mat previous_frame = back_final;			//stores img of previous frame.
		
	while(done)
	{
		Mat frame,frame_homo,frame_final;
		done = cap.read(frame);
		if(!done) break;					//video is finished.
	
		warpPerspective(frame,frame_homo,matrix,frame.size());
		Rect crop_region(472,52+(index*(778/total)),328,778/total);
		frame_final = frame_homo(crop_region);

		Mat img = abs(frame_final - back_final) > 50;		//Subtract background and consider part with diff grater than 50
		Mat dynamic_img = abs(frame_final - previous_frame)>50;//Subtract previous frame and consider part with diff greaer than 50
		previous_frame = frame_final;					//Set current frame to be previous for next frame
		
		pixels = sum(img);
		dynamic_pixels = sum(dynamic_img);
		
		queue_density[framenum][index] = ((pixels[0]+pixels[1]+pixels[2]));		//We assumed queue density will be proportional to number of poxels that are different in the 2 images
		dynamic_density[framenum][index] = (dynamic_pixels[0]+dynamic_pixels[1]+dynamic_pixels[2]);//And dynamic density will be proportional to the pixels that are changed in the 2 consecutive frames
		
		//if(framenum == 5175) imwrite("empty.jpg",frame); 			 For capturing empty frame  					
		//imshow("video_queue", img);
		//imshow("video_dynamic", dynamic_img);

		if (waitKey(10) == 27 || esc == 1)		//for testing purposes break at 100 seconds
		{
			cout << "Esc key is pressed by user. Stopping the video"<<framenum << endl;
		   	esc = 1;
		   	return NULL;
		}
		
		framenum = framenum+1;
	}
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
	if(total > 16)
	{
		cout << "Currently support max of 16 threads.. Number of threads that can be processed can be increased(significantly) by using locking mechanisms, but it may slow down by bit";
	}
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
  	
	Mat matrix[total];
	Mat back_final[total];					// intermediate homographic image,final cropped image

	auto start = high_resolution_clock::now();

	pthread_t ptid[total];
	forparallel n[total];

	Point2f left = userparameter[0];
	Point2f right = userparameter[3];
	Point2f left_new,right_new;

	int ratio = total - 1;
	for(int i=0;i<total;i++)
	{
		Mat back_homo;
		vector<Point2f> new_user;
		vector<Point2f> finalparameter;

		n[i].index = i;
		n[i].video = video;

		left_new = (left*ratio + userparameter[1])/(ratio+1);
		right_new = (right*ratio + userparameter[2])/(ratio+1);

		new_user.push_back(left);
		new_user.push_back(left_new);
		new_user.push_back(right_new);
		new_user.push_back(right);

		finalparameter.push_back(Point2f(472,52+(i*(778/total))));
		finalparameter.push_back(Point2f(472,52+((i+1)*(778/total))));
		finalparameter.push_back(Point2f(800,52+((i+1)*(778/total))));
		finalparameter.push_back(Point2f(800,52+(i*(778/total))));

		left = left_new;
		right = right_new;
		ratio--;

		matrix[i] = getPerspectiveTransform(new_user,finalparameter);
		warpPerspective(background,back_homo,matrix[i],background.size()); 
		Rect crop_region(472,52+(i*(778/total)),328,778/total);
		back_final[i] = back_homo(crop_region);

		n[i].back_final = back_final[i];
		n[i].matrix = matrix[i];
		pthread_create(&(ptid[i]), NULL, &consecutive, &(n[i]));
	}
	
	for(int i=0;i<total;i++)
	{
		pthread_join((ptid[i]),NULL);
	}
	
	freopen("out3.txt","w",stdout);
	cout<<"Sec,Queue,Dynamic"<<endl;
	int framenum = 0;
	int queue,dynamic,i;

	while(framenum<5737)
	{
		for(i=0;i<total;i++) 
		{
			queue = queue + queue_density[framenum][i];
			dynamic = dynamic + dynamic_density[framenum][i];
		}
		cout<<((float)framenum)/15<<fixed<<','<<queue/(1.25e6)<<','<<dynamic/(2.5e5)<<endl;
		queue = 0;
		dynamic = 0;
		framenum++;
	}
	
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout << "Time taken by function in seconds:\n "
 << duration.count()/(1e6)<< endl;
	//myfile.close();	
	return 0;
}
