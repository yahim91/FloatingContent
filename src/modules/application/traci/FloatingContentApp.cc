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
        refreshInterval = par("refreshInterval");
        poiReplicationRange = par("poiReplicationRange");
        refreshPoi = new cMessage("refreshPoi");

        ASSERT(annotations);
        stored = false;

        sentMessage = false;
        lastDroveAt = simTime();
        setCircle = false;

        if (poiCount > 0) {
            simtime_t poiStart = par("poiStart");
            startPoiMsg = new cMessage("scheduledStartPoi");
            stopPoiMsg = new cMessage("scheduledStopPoi");
            poiColor = par("poiColor").stringValue();
            poiTTL = par("poiTTL");
            scheduleAt(simTime() + poiStart, startPoiMsg);
        }
        if (refreshInterval != 0) {
            scheduleAt(simTime() + refreshInterval, refreshPoi);
        }
    }
}

void FloatingContentApp::onBeacon(WaveShortMessage* wsm) {
    //beacon received - transmitting all stored informations
    Storage storage;
    if (!tiles.size())
        return;

    storage.writeInt(tiles.size());
    for (unsigned int i = 0; i < tiles.size(); i++) {
        storage.writeDouble(tiles[i].x);
        storage.writeDouble(tiles[i].y);
    }
    sendMessage(storage, wsm->getSenderAddress());

}

void FloatingContentApp::onData(WaveShortMessage* wsm) {
    //annotations->scheduleErase(1,
            //annotations->drawLine(wsm->getSenderPos(),
              //      traci->getPositionAt(simTime()), "blue"));
    FloatingContentMessage* msg = dynamic_cast<FloatingContentMessage*>(wsm);
    Storage storage = msg->getStorage();
    storage.resetIter();
    int poiCount = storage.readInt();
    Coord temp;
    for (int i = 0; i < poiCount; i++) {
        temp = new Coord(storage.readDouble(), storage.readDouble());
        if (traci->getCurrentPosition().distance(temp) < poiReplicationRange
                && std::find(tiles.begin(), tiles.end(), temp) == tiles.end()) {
            tiles.push_back(temp);
            findHost()->getDisplayString().updateWith("r1=16,green");
            traciManager->addPOIReplica(temp);
            stored = true;
        }
    }

    if (!stored) {
        //findHost()->getDisplayString().updateWith("r1=16,yellow;");
    }

}

void FloatingContentApp::sendMessage(Storage storage, int address) {
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    FloatingContentMessage* new_msg = prepareMessage("data", dataLengthBits,
            channel, dataPriority, -1, 2);
    new_msg->setRecipientAddress(address);
    new_msg->setStorage(storage);

    sendWSM(new_msg);
}

void FloatingContentApp::handlePositionUpdate(cObject* obj) {
    char s[100];
    BaseWaveApplLayer::handlePositionUpdate(obj);
    //Coord tile = world->get_current_tile(traci->getCurrentPosition());
    Coord anchorPoint;
    traciManager->getCurrentPOI(traci->getCurrentPosition(), anchorPoint);
    tiles.push_back(anchorPoint);
    findHost()->getDisplayString().updateWith("r1=16,green");

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
        Coord geoCoord;
        setCircle = true;
        circleCoord = traci->getCurrentPosition();
        geoCoord = traci->commandPositionConversion(circleCoord);
        AnnotationManager::Annotation* circle = annotations->drawPoint(circleCoord, "blue", "");
        traciManager->addPOIReplica(circleCoord, circle);
        /*sprintf(circleDisplayString, "r2=500,-,%s,,%lf,%lf;r1=16,%s",
                poiColor.c_str(), circleCoord.x, circleCoord.y, poiColor.c_str());
        findHost()->getDisplayString().updateWith(circleDisplayString);*/
        tiles.push_back(circleCoord);
        //scheduleAt(simTime() + poiTTL, stopPoiMsg);
    } else if (msg == stopPoiMsg) {
        findHost()->getDisplayString().removeTag("r2");
    } else if (msg == refreshPoi) {
        scheduleAt(simTime() + refreshInterval, refreshPoi);
        refreshLocalStorage();
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

void FloatingContentApp::refreshLocalStorage() {
    std::vector<Coord>::iterator tile = tiles.begin();
    while(tile != tiles.end()) {
        if (tile->distance(traci->getCurrentPosition()) > poiReplicationRange) {
            tile = tiles.erase(tile);
            traciManager->removePOIReplica(*tile);
            stored = false;
            findHost()->getDisplayString().removeTag("r1");
        } else {
            ++tile;
        }
    }
}
