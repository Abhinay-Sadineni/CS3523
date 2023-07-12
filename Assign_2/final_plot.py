# Python program to demonstrate
# command line arguments
 
 
import matplotlib.pyplot as plt

x = [9,16,25,36,49,64,81,100]

y = []

y2 = []

f = open('final1.txt','r')

for row in f:
	row = row.split(' ')
	y.append(float(row[0]))
	
f = open('final2.txt','r')

for row in f:
    row = row.split(' ')
    y2.append(float(row[0]))
	
plt.plot(x,y,label="Pthreads")

plt.plot(x,y2,label="Openmp")

plt.scatter(x,y)
plt.xlabel("N")
plt.ylabel("Time Taken(\u03BCs)")
plt.scatter(x,y2)
plt.legend()
plt.title("Time Taken(\u03BCs) vs N")
plt.savefig("plot_const_K")