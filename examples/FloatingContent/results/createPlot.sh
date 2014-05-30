#!/bin/sh

scavetool scalar -p "module(*.anchorZone[*]) AND name(contacts) OR name(xpos) OR name(ypos)" \
    -g 'name' -O output.csv  nodebug-0.sca
tr ',' ' ' < output.csv > output_temp.csv
mv output_temp.csv output.csv
gnuplot heatmaps.gnu 
