import matplotlib.pyplot as plt
import numpy as np

def read_polygon( filename ) :
    datafile = open(filename,'r');
    l = datafile.readline()
    degree = np.fromstring(l,sep=' ',dtype=int)
    pcount = degree+1
    bpoints = np.fromfile(datafile,count=2*pcount,sep=' ',dtype=float)
    bpoints = bpoints.reshape(pcount,2)
    return bpoints

def read_curve( filename ) :
    curvefile = open(filename,'r');
    curve = np.fromfile(curvefile,sep=' ',dtype=float)
    curve = curve.reshape(-1,2)
    return curve


infile = '../data/simple.bcv'
#infile = '../data/infinity.bcv'
#infile = '../data/spiral.bcv'
cvfile = '../plots/bernstein.txt'
#cvfile = '../plots/casteljau.txt'

bpoints = read_polygon(infile)
curve = read_curve(cvfile)

plt.plot(bpoints[:,0],bpoints[:,1],c='b')
plt.plot(curve[:,0],curve[:,1],c='r')

plt.title('TP1 : Bezier curves')
plt.grid(True)
plt.axis('equal')
#plt.savefig("output.png")
plt.show()
