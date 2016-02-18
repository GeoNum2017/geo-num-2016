import sys
import matplotlib.pyplot as plt
import numpy as np
from random import random

# function to read B-spline data
def read_bspline( filename ) :
    # open the file
    datafile = open(filename,'r')
    # get number of control points
    l = datafile.readline()
    point_count = np.fromstring(l,count=1,sep=' ',dtype=int)
    # read control points
    points = np.fromfile(datafile,count=2*point_count,sep=' ',dtype=float)
    points = points.reshape(point_count,2)
    # get number of nodes
    l = datafile.readline()
    node_count = np.fromstring(l,count=1,sep=' ',dtype=int)
    # read nodes
    nodes = np.fromfile(datafile,count=node_count,sep=' ',dtype=float)
    # close the file
    datafile.close()
    return points, nodes


# function to read evaluated curve points
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


### you can now pass the dataname to this script
### just do e.g.
###
### >> python ../plots/plot.py spiral
###

# get command dataname from command line or use default
if len(sys.argv) < 2 :
    dataname = 'simple'
else :
    dataname = sys.argv[1]
    
# check if valid dataname
if not dataname in ['simple','spiral','circle','camel'] :
    print('invalid dataname: ' + dataname)
    sys.exit()
    
# read bspline data
points, nodes = read_bspline('../data/' + dataname + '.bspline')
# curve points dir
out_dir = '../plots/' + dataname + '/'

# init figure
plt.clf()
plt.axis('equal')
plt.axis('off')

# bspline degree
degree = nodes.shape[0] - points.shape[0] - 1
# how many segments
curve_count = points.shape[0] - degree

# plot segments
for i in range(curve_count) :
    curve = read_curve( out_dir + 'curve' + `i` + '.txt') # read i-th curve
    plt.plot(curve[:,0],curve[:,1],linestyle='-',linewidth=2.0) # plot
  
# plot control points
plt.plot(points[:,0],points[:,1],'ko')

# render
plt.show()

###
### uncomment to save the figure as png
###
#plt.savefig('../plots/'+dataname+'.png',dpi = 200)
