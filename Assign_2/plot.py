import matplotlib.pyplot as plt
import numpy as np
import sys

x=np.array([2,4,8,16,32,64])
y1=np.loadtxt("bash_output_pthread_final.txt",delimiter=" ",dtype="double")
y2=np.loadtxt("bash_output_openmp_final.txt",delimiter=" ",dtype="double")
plt.plot(x,y1,marker="o",label="pthread")
plt.plot(x,y2,marker="o",label="openmp")
plt.legend()
plt.xlabel("No of threads")
plt.ylabel("Time Taken")
plt.title("Time Taken(\u03BC s) vs No of threads")
plt.savefig(sys.argv[1])