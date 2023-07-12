import matplotlib.pyplot as plt
import numpy as np


def parsetime(str):
    arr1 = np.fromstring(str[0], dtype=np.int64, sep=':')
    arr2 = np.fromstring(str[1], dtype=np.int64, sep=':')
    arr3 = np.fromstring(str[2], dtype=np.int64, sep=':')
    return np.array([arr1, arr2, arr3])


def mx(x1):
    a1 = list()
    for i in range(x1.shape[0]):
        a1.append(parsetime(x1[i]))
    x11 = np.array(a1)
    y1 = np.array([])
    y2 = np.array([])
    y3 = np.array([])
    for i in range(0, x11.shape[0], 2):
        y1 = np.append(y1, [((x11[i+1][0][0]-x11[i][0][0]) *
                            60+(x11[i+1][0][1]-x11[i][0][1]) )*1000
                            + (x11[i+1][0][2]-x11[i][0][2])])

        y2 = np.append(y2, [((x11[i+1][1][0]-x11[i][1][0]) *
                            60+(x11[i+1][1][1]-x11[i][1][1]) )*1000
                            + (x11[i+1][1][2]-x11[i][1][2])])

        y3 = np.append(y3, [((x11[i+1][2][0]-x11[i][2][0]) *
                            60+(x11[i+1][2][1]-x11[i][2][1])) *1000
                             + (x11[i+1][2][2]-x11[i][2][2])])
    
    return np.array([[np.average(y1),np.average(y2),np.average(y3)],[np.max(y1),np.max(y2),np.max(y3)]])


x1 = np.loadtxt("bash_output_1.txt", delimiter="\t", dtype=object)
x2 = np.loadtxt("bash_output_2.txt", delimiter="\t", dtype=object)
x3 = np.loadtxt("bash_output_3.txt", delimiter="\t", dtype=object)
x4 = np.loadtxt("bash_output_4.txt", delimiter="\t", dtype=object)
x5 = np.loadtxt("bash_output_5.txt", delimiter="\t", dtype=object)

y1=mx(x1)
y2=mx(x2)
y3=mx(x3)
y4=mx(x4)
y5=mx(x5)

TAS_A=np.array([y1[0][0],y2[0][0],y3[0][0],y4[0][0],y5[0][0]])
CAS_A=np.array([y1[0][1],y2[0][1],y3[0][1],y4[0][1],y5[0][1]])
CAW_A=np.array([y1[0][2],y2[0][2],y3[0][2],y4[0][2],y5[0][2]])

TAS_B=np.array([y1[1][0],y2[1][0],y3[1][0],y4[1][0],y5[1][0]])
CAS_B=np.array([y1[1][1],y2[1][1],y3[1][1],y4[1][1],y5[1][1]])
CAW_B=np.array([y1[1][2],y2[1][2],y3[1][2],y4[1][2],y5[1][2]])



plt.plot([10,20,30,40,50],TAS_A,marker="o",label="TAS")
plt.plot([10,20,30,40,50],CAS_A,marker="o",label="CAS")
plt.plot([10,20,30,40,50],CAW_A,marker="o",label="Bounded CAS")

plt.legend()
plt.xlabel("No of threads")
plt.ylabel("Average Time Taken")
plt.title("Average Time Taken( ms) vs No of threads")
plt.savefig("fig1")
plt.close()


plt.plot([10,20,30,40,50],TAS_B,marker="o",label="TAS")
plt.plot([10,20,30,40,50],CAS_B,marker="o",label="CAS")
plt.plot([10,20,30,40,50],CAW_B,marker="o",label="Bounded CAS")
plt.legend()
plt.xlabel("No of threads")
plt.ylabel("Worst Time Taken")
plt.title("Worst Time Taken( ms) vs No of threads")
plt.savefig("fig2")
