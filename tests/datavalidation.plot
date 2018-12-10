set terminal pdf enhanced

set datafile separator whitespace
set key inside title "Categories"
set encoding utf8
set xtics nomirror
set ytics nomirror

set auto x
set style data histogram
set style fill solid border -1
set boxwidth 0.9
set xtic scale 0

immcolor = "green"
zeropagecolor = "red"
absolutecolor = "red"
mixedcolor = "red"

set title 'Response time & Data validation'
set ylabel 'Time [↓s]'

set style line 1 lc rgb immcolor
set style line 2 lc rgb zeropagecolor
set style line 3 lc rgb absolutecolor
set style line 4 lc rgb mixedcolor

plot './datavalidation.dat' with boxes

plot "datavalidation.dat" every ::0::0 using 1:3:xtic(1) with boxes ls 1, \
     "datavalidation.dat" every ::1::1 using 1:3:xtic(1) with boxes ls 2, \
     "datavalidation.dat" every ::2::2 using 1:3:xtic(1) with boxes ls 3, \
     "datavalidation.dat" every ::3::3 using 1:3:xtic(1) with boxes ls 4
