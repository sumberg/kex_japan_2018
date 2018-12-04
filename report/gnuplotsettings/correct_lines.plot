set terminal pdf enhanced

# Read comma-delimited data from file
set datafile separator comma
set key outside 
set key under title "Threshold"
set encoding utf8
set xtics nomirror 
set ytics nomirror

# Set graph title
#set title 'Immediate'

# Set label of x-axis
set xlabel 'Time [ms]'

# Set label of y-axis
set ylabel 'Number of instructions'

# Use a line graph
set style data line

# Set y-range 
#set yrange [0: 300]


# Path to file 
filename = '../../tests/results/immediate/incr_instr_imm.dat'

# red, green, blue, orange, purple, cyan, magenta, lime, brown, black
plot i=2 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#e6194b", \
 i=3 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#3cb44b", \
 i=4 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#0082c8", \
 i=5 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#f58231", \
 i=6 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#000080", \
 i=7 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#46f0f0", \
 i=8 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#f032e6", \
 i=9 filename using i:xtic(1) smooth csplines title columnheader linewidth 2 linecolor rgb "#d2f53c"

# for [i=3:11] '50%.csv' using i:xtic(1) smooth csplines title columnheader linewidth 4 


