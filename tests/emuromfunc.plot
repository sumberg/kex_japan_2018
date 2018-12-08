set terminal pdf enhanced

# Read space-delimited data file
set datafile separator whitespace
set key inside title "Categories"
set encoding utf8
set xtics nomirror
set ytics nomirror

# Settings from stack overflow
set auto x
set style data histogram
set style histogram cluster gap 1
set style fill solid border -1
set boxwidth 0.9
set xtic scale 0

immcolor = "#e6194b";
zeropagecolor = "#3cb44b";
absolutecolor = "#0082c8";
mixed = "#f58231";

# Files to use
imm = './results/immediate/ver_2.0/default_settings_imm.txt'
zp = './results/zero_page/ver_2.0/default_settings_zp.txt'
abs = './results/absolute/ver_2.0/default_settings_abs.txt'
mix = './results/mixed/ver_2.0/default_settings_mixed.txt'

# Graph title
set title 'Emulated ROM Functions'
# Label y
set ylabel 'Time [ms]'

plot './defaultDat.dat' using 2:xtic(1) ti col linecolor rgb immcolor, '' u 3 ti col linecolor rgb zeropagecolor, '' u 4 ti col linecolor rgb absolutecolor, '' u 5 ti col linecolor rgb mixed
