### png output
#set terminal pngcairo
#set output '../plots/output.png'

### window output
set terminal qt size 1200,800

data  = '../data/simple.bcv'
#data  = '../data/infinity.bcv'
#data  = '../data/spiral.bcv'
curve = '../plots/bernstein.txt'
#curve = '../plots/casteljau.txt'

set style data lines
plot data every ::1 using 1:2, \
     curve using 1:2