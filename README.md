# COP290

## Camera angle correction and frame cropping 
* Since OpenCV algorithms work best on rectangular frames. We are going to use the concept of homogrpahy to get the bird's view of traffic
 and then use cropping to make our image exclusive to vehicles on the road. 
* More information can be found on our [course webpage](https://www.cse.iitd.ac.in/~rijurekha/cop290_2021.html) 

## Instructions
To run the program write the following commands on your terminal
* `makefile` for compling anglecorrection.cpp to myapp
* `make runempty` or `make runtraffic` to give empty.jpg or traffic.jpg respectively as input select points for homography 
* press `esc` to view wrapped source image
* press `esc` to view the cropped image
* press `esc` to exit the program


