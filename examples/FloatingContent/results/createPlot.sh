#!/bin/sh

scavetool scalar -p "module(*.anchorZone[*]) AND name($1) OR name(xpos) OR name(ypos)" \
    -g 'name' -O output.csv  nodebug-0.sca
tr ',' ' ' < output.csv > output_temp.csv
mv output_temp.csv output.csv

#remove header line
#sed -i 1d output.csv

xsize=$(cut -d ' ' -f 5 output.csv | sort -nr | head -n1)
ysize=$(cut -d ' ' -f 6 output.csv | sort -nr | head -n1)
cbrange=$(cut -d ' ' -f 4 output.csv | sort -nr | head -n1)

gnuplot << EOF
set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 800, 500
set output 'Criticality.png'
unset key
set view map
set xtics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ztics border in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
#set nocbtics
set rtics axis in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
set title "Criticality Map generated from SanFrancisco simulation\n"
set xrange [ 0 : $xsize ] noreverse nowriteback
set yrange [ 0 : $ysize ] reverse nowriteback
set cblabel "Criticality"
set cbrange [ 0.00000 : $cbrange ] noreverse nowriteback
set palette rgbformulae 7, 5, 15
plot 'output.csv' using 5:6:4 with image
EOF

