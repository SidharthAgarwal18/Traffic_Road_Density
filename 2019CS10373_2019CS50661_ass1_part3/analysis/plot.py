import matplotlib.pyplot as plt
import sys
import csv

parameter = []
utility = []
runtime = []

if len(sys.argv)<2:
	print("Enter the name of the csv file you want to plot the graph for")
	exit()
elif len(sys.argv)>2:
	print("Enter only single file name of csv file")
	exit()
path  = str(sys.argv[1])

with open(path,'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    for row in plots:
        parameter.append(float(row[0]))
        utility.append(float(row[1]))
        runtime.append(float(row[2]))
        
plt.figure(figsize=(10, 10))

plt.scatter(runtime,utility,label='Absolute_difference')
for i,text in enumerate(parameter):
	plt.annotate(text,(runtime[i],utility[i]))

plt.plot(runtime,utility,'r')
plt.xlabel('runtime')
plt.ylabel('Absolute_difference')
plt.title(path+"_graph")
plt.legend()
plt.savefig(path+"_graph.png")
