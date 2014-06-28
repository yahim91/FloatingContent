#!/bin/bash

if [[ -z "$1" ]]; then
    echo "Usage: createPlot.sh [option] infile"
    exit
fi

if [[ -z "$2" ]]; then
    echo "Usage: createPlot.sh [option] infile"
    exit
fi

out_file=$1_${2//.sca/}
cblabel=""
if [[ "${1:0:11}" == "criticality" ]]; then
    cblabel="Criticality"
elif [[ "$1" == "transit" ]]; then
    cblabel="Transit"
fi

scavetool scalar -p "module(*.anchorZone[*]) AND name($1) OR name(xpos) OR name(ypos)" \
    -g 'name' -O output.csv  "${2}"
tr ',' ' ' < output.csv > output_temp.csv

dir=`echo "${2}" | cut -d"/" -f2`
row=3
if [ "$dir" == "media" ]; then 
    row=4
fi
cut -d" " -f$row- output_temp.csv > output.csv
#mv output_temp.csv output.csv

#remove header line
#sed -i 1d output.csv

xsize=$(cut -d ' ' -f 3 output.csv | sort -nr | head -n1)
ysize=$(cut -d ' ' -f 4 output.csv | sort -nr | head -n1)
cbrange=$(cut -d ' ' -f 2 output.csv | sort -nr | head -n1)


gnuplot << EOF
set terminal pngcairo  transparent enhanced font "arial,16" fontscale 1.0 size 800, 500
#set terminal cairolatex color colortext size 400, 300
#set terminal postscript color enhanced font "arial,16" fontscale 1.0 size 300, 200
set output '$out_file.png'
unset key
set view map
set xtics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ztics border in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
#set nocbtics
set rtics axis in scale 0,0 nomirror norotate  offset character 0, 0, 0 autojustify
set title "$3"
set xrange [ 0 : $xsize ] noreverse nowriteback
set yrange [ 0 : $ysize ] reverse nowriteback
set cblabel "$cblabel"
set cbrange [ 0.00000 : $cbrange ] noreverse nowriteback
set palette rgbformulae 7, 5, 15
plot 'output.csv' using 3:4:2 with image
EOF

if [ "$4" = "" ]; then
    exit
fi

awk '{if(NR > 1 && $4 != prev_two){printf "\n";} prev_two=$4; print $0}' output.csv > output_pm3d.csv

img_xsize=$(file SanFrancisco6.png | cut -d ' ' -f 5)
img_ysize=$(file SanFrancisco6.png | cut -d ' ' -f 7 | sed 's/[^0-9]//g')

gnuplot << EOF

#set terminal svg size 600,400 dynamic enhanced fname 'arial'  fsize 10 mousing name "surface1_3" butt solid
set terminal pngcairo  transparent enhanced font "arial,10" fontscale 1.0 size 800, 500
set output '$out_file_3D.png'

set samples 25, 25
set isosamples 30, 30

set grid layerdefault   linetype -1 linecolor rgb "gray"  linewidth 0.200,  linetype -1 linecolor rgb "gray"  linewidth 0.200
set title "$3"
set cbtics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify

set xlabel "X axis"
set xlabel  offset character -3, -2, 0 font "" textcolor lt -1 norotate

set multiplot
set ticslevel 0
set noxtics
set noytics
set noztics
set xrange [ 0 : $img_xsize ] noreverse nowriteback
set ylabel  offset character 3, -2, 0 font "" textcolor lt -1 rotate by -270
set yrange [ 0 : $img_ysize ] noreverse nowriteback
set zlabel ""
set zrange [ 0.0000 : 100 ] noreverse nowriteback

splot "$4" binary filetype=png center=($((img_xsize/2)),$((img_ysize/2)),0) with rgbimage title ""

set ztics
set xtics
set ytics
set contour
set ticslevel 1
set xrange [ 0 : $xsize ] noreverse nowriteback
set ylabel "Y axis"
set ylabel  offset character 3, -2, 0 font "" textcolor lt -1 rotate by -270
set yrange [ 0 : $ysize ] reverse nowriteback
set zlabel "Z axis"
set zlabel  offset character -5, 0, 0 font "" textcolor lt -1 norotate
set zrange [ 0.0000 : $cbrange ] noreverse nowriteback

#set pm3d implicit at s
set pm3d depthorder hidden3d 1

set style line 2 linetype 2 linecolor rgb "#a0a0f0"  linewidth 0.5
set style fill transparent solid 0.65 border

splot 'output_pm3d.csv' using 3:4:2 with pm3d title ""

unset multiplot
EOF
