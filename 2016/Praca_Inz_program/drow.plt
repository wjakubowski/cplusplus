#!/usr/bin/gnuplot -persist


set terminal postscript eps colortext color font 'Arial,20'
set tmargin 2

set xlabel 'z[nm]'

set key rmargin


filename='wyniki/fun_fal'.V.'.txt'

set output 'wykresy/fun.eps'
set ylabel '{/Symbol Y }[nm^{-3/2}]'
plot filename using 1:2  title '0' with lines lc rgb "red", \
filename using 1:3  title '1' with lines lc rgb "violet", \
filename using 1:4  title '2' with lines lc rgb "black", \
filename using 1:5  title '3' with lines lc rgb "blue", \
filename using 1:6  title '4' with lines lc rgb "pink"

filename='wyniki/dens'.V.'.txt'

set output 'wykresy/dens.eps'
set ylabel '{/Symbol r} [nm^{-1}]'
set autoscale x
plot filename using 1:2  title "total" w l lc rgb "red", \
filename using 1:3  title "el"  with lines lc rgb "green", \
filename using 1:4  title "dop" with lines lc rgb "blue"

filename='wyniki/V'.V.'.txt'
filename2='wyniki/fun_fal'.V.'.txt'

set output 'wykresy/V_fun.eps'
set ylabel 'V[eV]'
set autoscale x
plot filename using 1:2  title "V_{sc}" w l lc rgb "red", \
filename using 1:3  title "V_{el}"  with lines lc rgb "green", \
filename using 1:4  title "V_{g}"  with lines lc rgb "blue", \
filename using 1:5  title "V_{CB}" with lines lc rgb "black", \
filename2 using 1:2  title '{/Symbol Y }_0' with lines ls 5 lc rgb "orange", \
filename2 using 1:3  title '{/Symbol Y }_1' with lines ls 5 lc rgb "violet"

set output 'wykresy/alfa.eps'
set terminal postscript eps colortext color font 'Arial,13'
set key below
set multiplot layout 2,2

filename='wyniki/wyniki_alfa.txt'


set ylabel '{/Symbol a }_{nm}[meV nm]'
set xlabel 'V_g[eV]'
set autoscale x
plot filename using 1:2  title "{/Symbol a}_{00}" dt '-' lc rgb "black"  lw 2 smooth sbezier, \
filename using 1:3  title "{/Symbol a}_{10}"  dt '.' lc rgb "black" lw 2 smooth sbezier, \
filename using 1:4  title "{/Symbol a}_{11}" dt '_' lc rgb "black"  lw 2 smooth sbezier

filename='wyniki/wyniki_alfa.txt'

#set output 'wykresy/gammy00.png'
set ylabel '{/Symbol a }_{00}[meV nm]'
plot filename using 1:2  title "{/Symbol a}_{00}"  lc rgb "black"  lw 2 smooth sbezier, \
filename using 1:5  title "{/Symbol G}^w"  w l lc rgb "red" lw 2 smooth sbezier, \
filename using 1:7  title "{/Symbol G}^{el}" w l lc rgb "violet" lw 2 smooth sbezier, \
filename using 1:8  title "{/Symbol G}^{g}" w l lc rgb "blue" lw 2 smooth sbezier, \
filename using 1:6  title "{/Symbol G}^b" w l lc rgb "green" lw 2 smooth sbezier

filename='wyniki/wyniki_alfa.txt'

#set output 'wykresy/gammy10.png'
set ylabel '{/Symbol a }_{10}[meV nm]'
plot filename using 1:3  title "{/Symbol a}_{10}"  lc rgb "black"  lw 2 smooth sbezier, \
filename using 1:9  title "{/Symbol G}^w"  w l lc rgb "red" lw 2 smooth sbezier, \
filename using 1:11  title "{/Symbol G}^{el}" w l lc rgb "violet" lw 2 smooth sbezier, \
filename using 1:12  title "{/Symbol G}^{g}" w l lc rgb "blue" lw 2 smooth sbezier, \
filename using 1:10  title "{/Symbol G}^b" w l lc rgb "green" lw 2 smooth sbezier

filename='wyniki/wyniki_alfa.txt'

#set output 'wykresy/gammy11.png'
set ylabel '{/Symbol a }_{11}[meV nm]'
plot filename using 1:4  title "{/Symbol a}_{11}"  lc rgb "black"  lw 2 smooth sbezier, \
filename using 1:13  title "{/Symbol G}^w"  w l lc rgb "red" lw 2 smooth sbezier, \
filename using 1:15  title "{/Symbol G}^{el}" w l lc rgb "violet" lw 2 smooth sbezier, \
filename using 1:16 title "{/Symbol G}^{g}" w l lc rgb "blue" lw 2 smooth sbezier, \
filename using 1:14  title "{/Symbol G}^b" w l lc rgb "green" lw 2 smooth sbezier

unset multiplot









