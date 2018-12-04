set terminal pdf

set yrange [0 : 1300]

set xtics nomirror 

set ytics nomirror

unset key

# Set label of x-axis
set xlabel 'Routing Algorithm'

# Set label of y-axis
set ylabel 'Network Lifetime (time units)'

set style line 1 lt 1 lc rgb "green"
set style line 2 lt 1 lc rgb "red"

set boxwidth 0.5
set style fill solid


plot "55dev70%.dat"  using 2:xtic(1) with boxes linecolor rgb "#000080",\
  ""  using 0:($2-12):2 with labels notitle offset 0,0.7

