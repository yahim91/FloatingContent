#!/bin/bash

network=${1//.osm/.net.xml}
trips=${1//.osm/.trips.xml}
routes=${1//.osm/.rou.xml}
routes_alt=${1//.osm/.rou.alt.xml}
poly=${1//.osm/.poly.xml}
launchd=${1//.osm/.launchd.xml}
cfg=${1//.osm/.sumo.cfg}

create_net()
{

  echo "Creating network file for $1"
  netconvert --type-files typemap.xml \
    --ramps.guess --remove-edges.by-vclass hov,taxi,bus,delivery,transport,lightrail,cityrail,rail_slow,rail_fast,motorcycle,bicycle,pedestrian \
    --geometry.remove --remove-edges.isolated --tls.join --verbose --osm-files $1 --output-file $network --random true \
    --tls.green.time 45
}

create_routes()
{
  echo "Creating route file for $1"

  ~/sumo/tools/trip/randomTrips.py -n $network --min-distance 1500 -e 3600 -o $trips --trip-parameters='departLane="best" departSpeed="max" departPos="random"'
  duarouter -n $network -t $trips -o $routes --ignore-errors
}

create_poly()
{
  echo "Creating poly file for $1"
  polyconvert --net-file $network --osm-files $1 -o $poly --ignore-errors
}

create_cfg_files()
{
  echo 'Creating launchd file'
  echo '<?xml version="1.0"?>' > $launchd && \
    echo '<launch>' >> $launchd
  echo '<copy file="'$network'" />' >> $launchd && \
    echo '<copy file="'$routes_alt'" />' >> $launchd && \
    echo '<copy file="'$poly'" />' >> $launchd && \
    echo '<copy file="'$cfg'" type="config" />' >> $launchd && \
    echo '</launch>' >> $launchd

  echo "Creating cfg file"

  echo '<?xml version="1.0" encoding="iso-8859-1"?>' > $cfg && \
    echo '<configuration xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:noNamespaceSchemaLocation="http://sumo.sf.net/xsd/sumoConfiguration.xsd">' >> $cfg &&\
    echo '<input>' >> $cfg &&\
    echo '<net-file value="'$network'"/>' >> $cfg &&\
    echo '<route-files value="'$routes_alt'"/>' >> $cfg &&\
    echo '<additional-files value="'$poly'"/>' >> $cfg &&\
    echo '</input>' >> $cfg &&\
    echo '<time>' >> $cfg && \
    echo '<begin value="0"/>' >> $cfg &&\
    echo '<end value="1000"/>' >> $cfg &&\
    echo '<step-length value="0.1"/>' >> $cfg &&\
    echo '</time>' >> $cfg &&\
    echo '</configuration>' >> $cfg
}

#create_net
create_routes
#create_poly
#create_cfg_files


