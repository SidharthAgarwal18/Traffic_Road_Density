# COP290

## Camera angle correction and frame cropping 
* Since OpenCV algorithms work best on rectangular frames. We are going to use the concept of homogrpahy to get the bird's view of traffic
 and then use cropping to make our image exclusive to vehicles on the road. 
* More information can be found on our [course webpage](https://www.cse.iitd.ac.in/~rijurekha/cop290_2021.html) 

## Instructions
To run the program write the following commands on your terminal
* `$ make` to compile anglecorrection.cpp to a binary myapp.o
* Use `$ ./myapp.o img_name.extn` to view the img\_name.extn. Select 4 points and press `Esc` to view wrapped image. Press `Esc` again to view the cropped image.
