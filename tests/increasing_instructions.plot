set terminal pdf enhanced

# Read space-delimited data file
set datafile separator whitespace
set key outside
set key under title 'Categories'
set encoding utf8
set xtics nomirror
set ytics nomirror

# Graph title
set title 'Increasing instructions'

# Label x
set xlabel 'Number of Instructions'

# Label y
set ylabel 'Time [ms]'

# Use line graph
set style data line

# y-range
#set yrange [0 : 250]

# Files to use
imm = './results/immediate/incr_instr_imm.dat'
zp = './results/zero_page/incr_instr_zp.dat'
abs = './results/absolute/incr_instr_abs.dat'
mix = './results/mixed/incr_instr_mixed.dat'

# red, green, blue, orange
plot \
imm smooth csplines title "Immediate" linewidth 1 linecolor rgb "#e6194b", \
zp smooth csplines title "Zero Page" linewidth 1 linecolor rgb "#3cb44b", \
abs smooth csplines title "Absolute" linewidth 1 linecolor rgb "#0082c8", \
mix smooth csplines title "Mixed" linewidth 1 linecolor rgb "#f58231"
