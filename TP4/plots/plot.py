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


# pass an argument : dataname
if len(sys.argv) < 2 :
    dataname = 'simple'
else :
    dataname = sys.argv[1]
    
# read data
points = read_data('../data/' + dataname + '.data')

# curve points dir
out_dir = '../plots/' + dataname + '/'
files = os.listdir(out_dir)
files.sort()

curves = []
for file in files :
    if file.endswith(".txt") :
        print(file)
        curve = read_curve( out_dir + file ) # read i-th curve
        curves.append( curve )

# init figure
fig = plt.figure()
ax = fig.add_subplot(111)

cc = 0
for curve in curves :
    cc = cc+1
        
    # clear the figure
    ax.clear()
    ax.axis('equal')
    ax.axis('off')

    # render datapoints
    ax.plot(points[:,0],points[:,1],'ko')
    # render curvepoints
    #ax.plot(curve[:,0],curve[:,1],'b.')
    
    # render a closed curve
    ax.fill(curve[:,0],curve[:,1],fill=False,edgecolor='b')
    
    plt.pause(0.25)
    
    out_png = '../plots/'+dataname+`cc`+'.png';
    print('saving '+ out_png)
    plt.savefig(out_png,dpi = 150)
    