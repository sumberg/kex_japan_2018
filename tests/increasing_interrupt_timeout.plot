set terminal pdf enhanced

# Read space-delimited data file
set datafile separator whitespace
set key outside
set key under title 'Categories'
set encoding utf8
set xtics nomirror
set ytics nomirror

# Graph title
set title 'Increasing Interrupt Timeout'

# Label x
set xlabel 'Frequency of Interrupts [Hz]'

# Label y
set ylabel 'Time [ms]'

# Use line graph
set style data line

# y-range
#set yrange [0 : 250]

# Files to use
imm = './results/immediate/ver_2.0/incr_overflow_timeout_imm.dat'
zp = './results/zero_page/ver_2.0/incr_overflow_timeout_zp.dat'
abs = './results/absolute/ver_2.0/incr_overflow_timeout_abs.dat'
mix = './results/mixed/ver_2.0/incr_overflow_timeout_mixed.dat'

# red, green, blue, orange
plot \
imm smooth csplines title "Immediate" linewidth 1 linecolor rgb "#e6194b", \
zp smooth csplines title "Zero Page" linewidth 1 linecolor rgb "#3cb44b", \
abs smooth csplines title "Absolute" linewidth 1 linecolor rgb "#0082c8", \
mix smooth csplines title "Mixed" linewidth 1 linecolor rgb "#f58231"
