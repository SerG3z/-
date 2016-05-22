#!/bin/bash

gnuplot << EOP

set terminal png size 800,600
set output "graph.png"

set yrange [0:]

set xlabel "Количество потоков"
set ylabel "Ускорение"

plot "results.txt" using 1:2 title "Ускорение" with lines

EOP
