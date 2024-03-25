set title "log2 comparision"
set xlabel "x"
set ylabel "log2(x)"
set terminal png font " Times_New_Roman,12 "
set output "log2.png"
set xtic 1000
set key left


plot \
"log2.csv" using 1:2 with points pointsize 1 title "log2", \
"log2.csv" using 1:3 with points pointsize 1 pointtype 7 title "fixlog2_16", \
"log2.csv" using 1:4 with points pointsize 1 title "fixlog2_1", \