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

bfile = '../data/simple.bcv'
#bfile = '../data/infinity.bcv'
#bfile = '../data/spiral.bcv'
bpoints = read_polygon(bfile)
plt.plot(bpoints[:,0],bpoints[:,1],'bo-')

cfile = '../plots/output.txt'
#curve = read_curve(cfile)
#plt.plot(curve[:,0],curve[:,1],'r-')

plt.title('TP1 : Bezier curves')
plt.grid(True)
plt.axis('equal')
#plt.savefig("../plots/output.png")
plt.show()
