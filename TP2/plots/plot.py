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
    return bpoints

def read_curve( filename ) :
    curvefile = open(filename,'r');
    curve = np.fromfile(curvefile,sep=' ',dtype=float)
    curve = curve.reshape(-1,2)
    return curve


dataname = 'simple'
#dataname = 'infinity'
#dataname = 'spiral'
#dataname = 'semi'

bpoints = read_polygon('../data/'+dataname+'.bcv')
out_dir = '../plots/'+dataname+'/c1/'
#out_dir = '../plots/'+dataname+'/c2/'
        
plt.clf()
plt.axis('equal')
plt.axis('off')
axes = plt.gca()
        
for i in range(bpoints.shape[0]-1) :
    cfile = out_dir + 'curve' + `i` + '.txt'
    curve = read_curve(cfile)
    plt.plot(curve[:,0],curve[:,1],linestyle='-',linewidth=2.0)

plt.plot(bpoints[:,0],bpoints[:,1],'ko')
plt.savefig('../plots/'+dataname+'.png',dpi = 200)
