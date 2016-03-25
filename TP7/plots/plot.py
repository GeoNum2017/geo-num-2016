import pdb
import sys
import os
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import cm
import matplotlib.pyplot as plt
import numpy as np
from random import random

# read from off file
def read_off( filename ) :
    f = open(filename, "r")
    assert f.readline() == "OFF\n"
    nv, nf, zero = f.readline().split()
    nv = int(nv)
    nf = int(nf)
    print "%d vertices, %d facets" % (nv, nf)
    assert zero == "0"
    V = np.fromfile(f, dtype = float, count = 3 * nv, sep = " ")
    V = V.reshape(-1, 3)
    F = np.fromfile(f, dtype = int, count = 4 * nf, sep = " ")
    assert F.size == 4 * nf
    F = F.reshape(-1, 4)[:, 1:]
    return V, F


# 1st argument : dataname
if len(sys.argv) < 2 :
    dataname = 'simple'
else :
    dataname = sys.argv[1]
    
# read data
off_file = '../plots/' + dataname + '/patch.off'
V, F = read_off( off_file )

# plot
fig = plt.figure()
ax = fig.gca(projection='3d')
ax.clear()
ax.axis('equal')
ax.axis('on')
ax.plot_trisurf(V[:,0], V[:,1], V[:,2], edgecolor='none')
#plt.savefig('../plots/out.png',dpi = 200)
plt.show()
