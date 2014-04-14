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

#include <cdisplaystring.h>
#include <cobjectfactory.h>
#include <cregistrationlist.h>
#include <csimulation.h>
#include <Consts80211p.h>
#include <Coord.h>
#include <FindModule.h>
#include <FloatingContentApp.h>
#include <onstartup.h>
#include <regmacros.h>
#include <simkerneldefs.h>
#include <simtime.h>
#include <simutil.h>
#include <TraCIScenarioManager.h>
#include <WaveShortMessage_m.h>
#include <cstdio>

Define_Module(FloatingContentApp);

void FloatingContentApp::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);

    if (stage == 0) {
        world = FindModule<WorldUtility*>::findGlobalModule();
        traci = TraCIMobilityAccess().get(getParentModule());
        traciManager = TraCIScenarioManagerAccess().get();
        annotations = AnnotationManagerAccess().getIfExists();
        requestTile = new cMessage("requestTile");
        poiCount = par("poiCount");
        ASSERT(annotations);

        sentMessage = false;
        lastDroveAt = simTime();
        setCircle = false;

        if (poiCount > 0) {
            simtime_t poiStart = par("poiStart");
            startPoiMsg = new cMessage("scheduledPoi");
            scheduleAt(simTime() + poiStart, startPoiMsg);
        }
    }
}

void FloatingContentApp::onBeacon(WaveShortMessage* wsm) {

}

void FloatingContentApp::onData(WaveShortMessage* wsm) {
    //findHost()->getDisplayString().updateWith("r=16,green");
    annotations->scheduleErase(1,
            annotations->drawLine(wsm->getSenderPos(),
                    traci->getPositionAt(simTime()), "blue"));
}

void FloatingContentApp::sendMessage(Storage storage, int address) {
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* new_wsm = prepareWSM("data", dataLengthBits, channel,
            dataPriority, -1, 2);
    new_wsm->setRecipientAddress(address);
    //new_wsm->set
    sendWSM(new_wsm);
}

void FloatingContentApp::handlePositionUpdate(cObject* obj) {
    char s[100];

    Coord tile = world->get_current_tile(traci->getCurrentPosition());
    sprintf(s, "t=current tile: %.2lf %.2lf;", tile.x, tile.y);
    if (setCircle)
        strcat(s, circleDisplayString);
    findHost()->getDisplayString().updateWith(s);

    if (tile == currentTile) {
        return;
    }

    currentTile = tile;

    if (std::find(tiles.begin(), tiles.end(), tile) == tiles.end()) {
        cancelEvent(requestTile); // if tile is not nedeed anymore
        findHost()->getDisplayString().updateWith("r1=16,red;");

        cMsgPar* x = new cMsgPar("x");
        x->setDoubleValue(tile.x);

        cMsgPar* y = new cMsgPar("y");
        y->setDoubleValue(tile.y);
        requestTile->addPar(x);
        requestTile->addPar(y);
        scheduleAt(simTime() + 20, requestTile);
    }

}

void FloatingContentApp::handleSelfMsg(cMessage *msg) {
    if (msg == requestTile) {
        cMsgPar x = msg->par("x");
        cMsgPar y = msg->par("y");
        Coord newTile = new Coord(x.doubleValue(), y.doubleValue(), 0);
        if (std::find(tiles.begin(), tiles.end(), newTile) == tiles.end()) {
            tiles.push_back(newTile);
        }
        findHost()->getDisplayString().updateWith("r1=16,green");
    } else if (msg == startPoiMsg) {
        circleCoord = traci->getCurrentPosition();
        setCircle = true;
        sprintf(circleDisplayString, "r2=250,-,,,%lf,%lf;r=16,green;", circleCoord.x, circleCoord.y);
    } else {
        BaseWaveApplLayer::handleSelfMsg(msg);
    }
}

FloatingContentMessage* FloatingContentApp::prepareMessage(std::string name,
        int lengthBits, t_channel channel, int priority, int rcvId,
        int serial) {
    FloatingContentMessage* wsm = new FloatingContentMessage(name.c_str());
    wsm->addBitLength(headerLength);
    wsm->addBitLength(lengthBits);

    switch (channel) {
    case type_SCH:
        wsm->setChannelNumber(Channels::SCH1);
        break; //will be rewritten at Mac1609_4 to actual Service Channel. This is just so no controlInfo is needed
    case type_CCH:
        wsm->setChannelNumber(Channels::CCH);
        break;
    }
    wsm->setPsid(0);
    wsm->setPriority(priority);
    wsm->setWsmVersion(1);
    wsm->setTimestamp(simTime());
    wsm->setSenderAddress(myId);
    wsm->setRecipientAddress(rcvId);
    wsm->setSenderPos(curPosition);
    wsm->setSerial(serial);

    if (name == "beacon") {
        DBG << "Creating Beacon with Priority " << priority
                   << " at Applayer at " << wsm->getTimestamp() << std::endl;
    }
    if (name == "data") {
        DBG << "Creating Data with Priority " << priority << " at Applayer at "
                   << wsm->getTimestamp() << std::endl;
    }

    return wsm;
}
