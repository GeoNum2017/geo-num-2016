### png output
#set terminal pngcairo
#set output '../plots/output.png'

### window output
set terminal qt size 1200,800

data  = '../data/simple.bcv'
#data  = '../data/infinity.bcv'
#data  = '../data/spiral.bcv'
plot data every ::1 using 1:2 with linespoints pt 7 ps 1 

curve = '../plots/output.txt'
#replot curve using 1:2 with lines