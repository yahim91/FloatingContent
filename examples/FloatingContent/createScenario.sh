#!/bin/bash

echo "Creating network file for $1"
network=${1//.osm/.net.xml}
trips=${1//.osm/.trips.xml}
routes=${1//.osm/.rou.xml}
poly=${1//.osm/.poly.xml}

netconvert --type-files typemap.xml \
   --ramps.guess --remove-edges.by-vclass hov,taxi,bus,delivery,transport,lightrail,cityrail,rail_slow,rail_fast,motorcycle,bicycle,pedestrian \
  --geometry.remove --remove-edges.isolated --tls.join --verbose --osm-files $1 --output-file $network --random true \
  --tls.green.time 45


echo "Creating route file for $1"

~/sumo/tools/trip/randomTrips.py -n $network --min-distance 1500 -e 3600 -o $trips --trip-parameters='departLane="best" departSpeed="max" departPos="random"'
duarouter -n $network -t $trips -o $routes --ignore-errors

echo "Creating poly file for $1"
polyconvert --net-file $network --osm-files $1 -o $poly --ignore-errors

