//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "FloatingContentMobility.h"

Define_Module(FloatingContentMobility);

void FloatingContentMobility::initialize(int stage) {
    if (stage == 0) {
        TraCIMobility::initialize(stage);
        poiCount = par("poiCount");

        /*if (poiCount > 0) {
            simtime_t poiStart = par("poiStart");
            startPoiMsg = new cMessage("scheduledPoi");
            scheduleAt(simTime() + poiStart, startPoiMsg);
        }*/

    } else if (stage == 1) {
    } else {
        TraCIMobility::initialize(stage);
    }
}

void FloatingContentMobility::handleSelfMsg(cMessage *msg) {
}
