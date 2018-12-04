set terminal pdf enhanced

# Read space-delimited data file
set datafile separator space
set key outside
set key under title "Vad är det här för inställning?"
set encoding utf8
set xtics nomirror
set ytics nomirror

# Graph title
set title 'Default values'

# Label x
set xlabel 'Number of Instructions'

# Label y
set ylable 'Time [ms]'

# Use line graph
