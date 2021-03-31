# COP290

## Utility and Runtime calculator and graph plotter
* `utility.cpp` is for calculating the utility of a method by comparing it to some variable baseline and `plot.py` uses matplotlib to plot 
the graph of different runtime and utility values for different graph.
* More information can be found on our [course webpage](https://www.cse.iitd.ac.in/~rijurekha/cop290_2021.html) 

##Instructions
* Use the command `$ g++ utility.cpp -o ut.o` to compile `utility.cpp` to `ut.o`
* Use the command `$ ./ut.o reference.csv second.csv` to compare the runtime and absolute difference of `second.csv` with `reference.csv` as baseline
* Use the command `$ python3 plot.py filename.csv` to plot the graph for the data in csv file named filename.csv

##Error Handling
* You are expected to maintain the number of arguments in each command line, any command with more or less commands than expected will display the error accordingly.
* You are expected to maintain the number of arguments in each command line, any command with more or less commands than expected will display the error accordingly.
