import matplotlib.pyplot as plt
import numpy as np
from random import random

def read_polygon( filename ) :
    datafile = open(filename,'r');
    l = datafile.readline()
    degree = np.fromstring(l,sep=' ',dtype=int)
    pcount = degree+1
    bpoints = np.fromfile(datafile,count=2*pcount,sep=' ',dtype=float)
    bpoints = bpoints.reshape(pcount,2)
    datafile.close()
    return bpoints

def read_curve( filename ) :
    curvefile = open(filename,'r');
    curve = np.fromfile(curvefile,sep=' ',dtype=float)
    curve = curve.reshape(-1,2)
    curvefile.close()
    return curve


dataname = 'simple'
#dataname = 'infinity'
#dataname = 'spiral'
#dataname = 'semi'

bpoints = read_polygon('../data/' + dataname + '.bcv')

out_dir = '../plots/' + dataname + '/c1/'
#out_dir = '../plots/' + dataname + '/c2/'
        
plt.clf()
plt.axis('equal')
plt.axis('off')
        
n = bpoints.shape[0]-1; # n = number of curves
for i in range(n) :
    curve = read_curve( out_dir + 'curve' + `i` + '.txt') # read i-th curve
    plt.plot(curve[:,0],curve[:,1],linestyle='-',linewidth=2.0) # plot

plt.plot(bpoints[:,0],bpoints[:,1],'ko')
plt.show()

### Uncomment this to save the figure as png
#plt.savefig('../plots/'+dataname+'.png',dpi = 200)
