#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <stdexcept>
using namespace std;

void readstring(string data,float err_queue[],float err_dynamic[],int index)
{
	int initial = 0;
	int pos = 0;
	float queue = 0;
	float dynamic = 0;

	while((data[pos]-'0'>=0 && data[pos]-'9'<=0) || data[pos]==46) pos++;
	pos++;
	initial = pos;
	while((data[pos]-'0'>=0 && data[pos]-'9'<=0) || data[pos]==46) pos++;
	char first[pos-initial+1];
	strcpy(first, data.substr(initial,pos-initial).c_str());
	queue = atof(first);

	pos++;
	initial = pos;

	while(pos<data.size() && ((data[pos]-'0'>=0 && data[pos]-'9'<=0) || data[pos]==46)) pos++;
	char second[pos-initial+1];
	strcpy(second, data.substr(initial,pos-initial).c_str());
	dynamic = atof(second);
	
	err_queue[index] = abs(err_queue[index] - queue);
	err_dynamic[index] = abs(err_dynamic[index] - dynamic);

	return;
}

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		cout<<"Please enter the location of the output files to compare for error evaluation";
	}
	freopen(argv[1], "r", stdin);
	
	string data;
	float err_queue[6000] = {0};
	float err_dynamic[6000] = {0};

	int index = -1;
	while(getline(cin,data))
	{
		if(index!=-1 && (data[0]-'0'>=0 && data[0]-'9'<=0))	
		{
			readstring(data,err_queue,err_dynamic,index);
		}	
		if(index!=-1 && !(data[0]-'0'>=0 && data[0]-'9'<=0))
		{
			index++;
			break;
		}
		index++;
	}
	getline(cin,data);

	char third1[data.size()+1];
	strcpy(third1,data.c_str());
	float reference_time = atof(third1);
	fclose(stdin);

	index = -1;
	freopen(argv[2], "r", stdin);
	while(getline(cin,data))
	{
		if(index!=-1 && (data[0]-'0'>=0 && data[0]-'9'<=0))	
		{
			readstring(data,err_queue,err_dynamic,index);
		}	
		if(index!=-1 && !(data[0]-'0'>=0 && data[0]-'9'<=0))
		{
			index++;
			break;
		}
		index++;
	}
	getline(cin,data);

	char third2[data.size()+1];
	strcpy(third2,data.c_str());
	float runtime = atof(third2);
	fclose(stdin);

	float abs_error_queue = 0;
	float sqr_error_queue = 0;
	float abs_error_dynamic = 0;
	float sqr_error_dynamic = 0;
	int it;

	while(it<index)
	{
		abs_error_queue = (abs_error_queue + err_queue[it]);
		sqr_error_queue = sqr_error_queue + err_queue[it]*err_queue[it];
		abs_error_dynamic = abs_error_dynamic + err_dynamic[it];
		sqr_error_dynamic = sqr_error_dynamic + err_dynamic[it]*err_dynamic[it];
		it++;
	}

	cout<<"The runtime for reference is "<<reference_time<<endl<<"The runtime for this method is "<<runtime<<endl;
	cout<<"The utility of queue density using\nAbsolute difference : "<<abs_error_queue/index<<"\nSquared difference : "<<sqrt(sqr_error_queue/(index*index))<<endl;
	cout<<"The utility of dynamic density using\nAbsolute difference : "<<abs_error_dynamic/index<<"\nSquared difference : "<<(sqrt(sqr_error_dynamic))/index<<endl;

}