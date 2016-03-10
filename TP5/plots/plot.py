import sys
import os
import matplotlib.pyplot as plt
import numpy as np
from random import random


# read datapoints
def read_data( filename ) :
    # open the file
    datafile = open(filename,'r')
    # get number of points
    l = datafile.readline()
    point_count = np.fromstring(l,count=1,sep=' ',dtype=int)
    # read points
    points = np.fromfile(datafile,count=2*point_count,sep=' ',dtype=float)
    points = points.reshape(point_count,2)
    # close the file
    datafile.close()
    return points


# read evaluated curve points
def read_curve( filename ) :
    # open the file
    curvefile = open(filename,'r');
    # read the whole file
    curve = np.fromfile(curvefile,sep=' ',dtype=float)
    # reshape as to get plane coordinates
    curve = curve.reshape(-1,2)
    # close
    curvefile.close()
    return curve


# 1st argument : dataname
if len(sys.argv) < 2 :
    dataname = 'simple'
else :
    dataname = sys.argv[1]
    
# read data
points = read_data('../data/' +dataname+'.data')
curve = read_curve('../plots/'+dataname+'.txt')

# init figure
fig = plt.figure()
ax = fig.add_subplot(111)
ax.clear()
ax.axis('equal')
ax.axis('off')

# render datapoints
ax.plot(points[:,0],points[:,1],'ko')
ax.fill(points[:,0],points[:,1],fill=False,ec='#666666',ls='dashed')
# render closed curve
ax.fill(curve[:,0],curve[:,1],fill=False,ec='b')
# save figure
#plt.savefig('../plots/'+dataname+'.png',dpi = 200)
plt.show()    
