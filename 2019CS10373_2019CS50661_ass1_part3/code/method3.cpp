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
int a[17] = {0};
Mat frame_i;
bool done=true;
int fnum;
vector<Point2f> userparameter;							// To store the points clicked by user

class forparallel
{
public:
	int index;
	Mat background;
};

void* consecutive(void* arg)
{
	forparallel n = *((forparallel*)arg);
	int index = n.index;
	Mat back_final;
	Mat background_i = n.background;
	Mat matrix;
	Mat back_homo;
	Point2f lu = userparameter[0];
	Point2f ld = userparameter[1];
	Point2f rd = userparameter[2];
	Point2f ru = userparameter[3];
	Point2f left_new,right_new;
	vector<Point2f> new_user;
	vector<Point2f> finalparameter;
	new_user.push_back((ld*index+(total-index)*lu)/total);			//area of intrest of every thread user
	new_user.push_back((ld*(index+1)+(total-index-1)*lu)/total);
	new_user.push_back((rd*(index+1)+(total-index-1)*ru)/total);
	new_user.push_back((rd*(index)+(total-index)*ru)/total);
	finalparameter.push_back(Point2f(472,52+(index*(778/total))));
	finalparameter.push_back(Point2f(472,52+((index+1)*(778/total))));
	finalparameter.push_back(Point2f(800,52+((index+1)*(778/total))));
	finalparameter.push_back(Point2f(800,52+(index*(778/total))));

	float zero_x,zero_y,zero_width,zero_height;						// shifting origin
	zero_x = min(new_user[0].x,new_user[1].x);
	zero_y = min(new_user[0].y,new_user[3].y);

	zero_x = min(zero_x,finalparameter[0].x);
	zero_y = min(zero_y,finalparameter[0].y);

	new_user[0] = new_user[0] - Point2f(zero_x,zero_y);
	new_user[1] = new_user[1] - Point2f(zero_x,zero_y);
	new_user[2] = new_user[2] - Point2f(zero_x,zero_y);
	new_user[3] = new_user[3] - Point2f(zero_x,zero_y);
	finalparameter[0] = finalparameter[0] - Point2f(zero_x,zero_y);
	finalparameter[1] = finalparameter[1] - Point2f(zero_x,zero_y);
	finalparameter[2] = finalparameter[2] - Point2f(zero_x,zero_y);
	finalparameter[3] = finalparameter[3] - Point2f(zero_x,zero_y);

	zero_width = max(finalparameter[3].x,max(new_user[3].x,new_user[2].x));
	zero_height = max(finalparameter[2].y,max(new_user[2].y,new_user[1].y));

	Rect crop_initial(zero_x,zero_y,zero_width,zero_height);
	Mat background = background_i(crop_initial);
	//if(index == total -1) imshow("a",background);
	//cout << background.size();

	matrix = getPerspectiveTransform(new_user,finalparameter);
	warpPerspective(background,back_homo,matrix,background.size()); 
	Rect crop_region(finalparameter[0].x,finalparameter[0].y,finalparameter[3].x - finalparameter[0].x,finalparameter[1].y - finalparameter[0].y);
	back_final = back_homo(crop_region);

	//sleep(index);
	int framenum = 0;					
		
	Scalar pixels;						//sum of pixels in subtracted image for queue_density
	Scalar dynamic_pixels;					// sum of pixels in subtracted image for dynamic_density
	Mat previous_frame = back_final;
	//if(index == total -1) imshow("a",back_final);			//stores img of previous frame.
	if(waitKey(10)==27)
	{
		return NULL;
	}
	while(done)
	{
		if(a[index]==0){
			
		Mat frame,frame_homo,frame_final;
		frame = frame_i(crop_initial);

		warpPerspective(frame,frame_homo,matrix,frame.size());
		
		frame_final = frame_homo(crop_region);
		//if(index == total-1) imshow("a",frame_final);
		Mat img = abs(frame_final - back_final) > 50;		//Subtract background and consider part with diff grater than 50
		Mat dynamic_img = abs(frame_final - previous_frame)>50;//Subtract previous frame and consider part with diff greaer than 50
		previous_frame = frame_final;					//Set current frame to be previous for next frame
		
		pixels = sum(img);
		dynamic_pixels = sum(dynamic_img);
		
		queue_density[fnum][index] = ((pixels[0]+pixels[1]+pixels[2]));		//We assumed queue density will be proportional to number of poxels that are different in the 2 images
		dynamic_density[fnum][index] = (dynamic_pixels[0]+dynamic_pixels[1]+dynamic_pixels[2]);//And dynamic density will be proportional to the pixels that are changed in the 2 consecutive frames
		
		a[index] = 1;
		//if(framenum == 5175) imwrite("empty.jpg",frame); 			 For capturing empty frame  					
		//if(index == total - 1) imshow("video_queue", frame_final);
		//imshow("video_dynamic", dynamic_img);
		if (waitKey(10) == 27 || esc == 1)		//for testing purposes break at 100 seconds
		{
			cout << "Esc key is pressed by user. Stopping the video"<<framenum << endl;
		   	esc = 1;
		   	return NULL;
		}}
		
	}
	return NULL;
}



int main(int argc, char* argv[])
{
	userparameter.resize(0);
	esc = 0;
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
	
	userparameter.push_back(Point2f(1000,218));
	userparameter.push_back(Point2f(461,897));
	userparameter.push_back(Point2f(1521,924));
	userparameter.push_back(Point2f(1278,205));
  	
	Mat matrix[total];
	Mat back_final[total];					// intermediate homographic image,final cropped image

	auto start = high_resolution_clock::now();

	pthread_t ptid[total];
	forparallel n[total];

	VideoCapture cap(video);
	if (cap.isOpened() == false)  
	{
		cout << "Video file not found, you can download it from https://www.cse.iitd.ac.in/~rijurekha/cop290_2021/trafficvideo.mp4 or simply name path variable in code" << endl;
		return -1;
	}
	for(int i=0;i<total;i++)
	{
		a[i] = 1;
		n[i].index = i;
		n[i].background = background;
		pthread_create(&(ptid[i]), NULL, &consecutive, &(n[i]));
	}
	//cout << "hi";
	fnum = 0;
	while(fnum<5737)
	{
		int next = 1;
		for(int i=0;next && i<total;i++)
		{
			if(a[i]==0) next=0;
		}
		if(next)
		{
			int done = cap.read(frame_i);
			if(!done) break;
			fnum++;
			for(int i=0;i<total;i++)
			{
				a[i] = 0;
			}
			
		}
	}
	done = false;
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
