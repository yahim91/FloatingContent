#!/bin/sh

mv nodebug-0.sca demo.sca
./createPlot.sh criticality3 demo.sca && mv criticality3_demo.png criticality_demo.png

./createPlot.sh avgContacts demo.sca && mv avgContacts_demo.png average_contacts_demo.png

eog criticality_demo.png average_contacts_demo.png &
