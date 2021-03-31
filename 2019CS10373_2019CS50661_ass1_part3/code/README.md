# COP290

## Utility-Runtime tradeoff analysis for queue and dynamic density
* This program explores the various ways of approaching the problem of calculating queue and dynamic density of cars on road and analysis the tradeoff in each such way.
* More information can be found on our [course webpage](https://www.cse.iitd.ac.in/~rijurekha/cop290_2021.html) 

## Instructions
To run the program write the following commands on your terminal
* `$ make` to compile reference.cpp, method1.cpp, method2.cpp, method3.cpp, method4.cpp to binary ref.o, m1.o, m2.o, m3.o, m4.o
respectively.
* All the commands below will print the output in terminal you can add `> filename` in the end of each command to print the output in the file named `filename`.
* Use `$ ./ref.o img_name.extn video_name.extn` for running `ref.o`
* Use `$ ./m1.o img_name.extn video_name.extn x` for running `m1.o` where `x` will be the sub-sampling parameter
* Use `$ ./m2.o img_name.extn video_name.extn width height` for running `m2.o` where `width` and `height` will be the resolution parameters.
* Use `$ ./m3.o img_name.extn video_name.extn regions` for running `m3.o` where regions will be the number of crop regions user wants to anylyse the runtime for.
* Use `$ ./m4.o img_name.extn video_name.extn threads` for running `m4.o` where threads will be the number of threads in which the user wants to divide the computation.

## Error Handling
* You are expected to have a `img_name.extn` and `video_name.ext` to run the program.
* You are expected to maintain the number of arguments in each command line, any command with more or less commands than expected will display the error accordingly.
