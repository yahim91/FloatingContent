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
        ASSERT(annotations);

        sentMessage = false;
        lastDroveAt = simTime();
    }
}

void FloatingContentApp::onBeacon(WaveShortMessage* wsm) {
    char data[dataLengthBits];
    /*double x = 3.2;
    double y = 3.7;*/
    int offset = 0;
    memset(data, 0, dataLengthBits);


    for (int i = 0; i < tiles.size(); i++) {
        double x = (double)58;
        double y = (double)tiles[i].y;
        memcpy(data + offset, &x, sizeof(double));
        offset += sizeof(double);
        memcpy(data + offset, &y, sizeof(double));
        offset += sizeof(double);
    }


    /*memcpy(data, &x, sizeof(double));
    memcpy(data + sizeof(double), &y, sizeof(double));*/

    EV << "BEACON received" << data << endl;


    sendMessage(data, wsm->getSenderAddress());
}

void FloatingContentApp::onData(WaveShortMessage* wsm) {
    const char *data;
    double x, y;
    int offset = 0;

    findHost()->getDisplayString().updateWith("r=16,green");
    annotations->scheduleErase(1,
            annotations->drawLine(wsm->getSenderPos(),
                    traci->getPositionAt(simTime()), "blue"));
    data = wsm->getWsmData();
    while(true) {
        memcpy(&x, data + offset, sizeof(double));
        offset+= sizeof(double);
        memcpy(&y, data + offset, sizeof(double));
        offset+= sizeof(double);
    }
}

void FloatingContentApp::sendMessage(char *data, int address) {
    t_channel channel = dataOnSch ? type_SCH : type_CCH;
    WaveShortMessage* new_wsm = prepareWSM("data", dataLengthBits, channel,
                dataPriority, -1, 2);
    new_wsm->setRecipientAddress(address);
    new_wsm->setWsmData(data);
    sendWSM (new_wsm);
}

void FloatingContentApp::handlePositionUpdate(cObject* obj) {
    char s[100];

    Coord tile = world->get_current_tile(traci->getCurrentPosition());
    if (tile == currentTile) {
        return;
    }
    currentTile = tile;

    sprintf(s, "t=current tile: %.2lf %.2lf;", tile.x, tile.y);

    findHost()->getDisplayString().updateWith(s);

    if (std::find(tiles.begin(), tiles.end(), tile) == tiles.end()) {
        cancelEvent(requestTile); // if tile is not nedeed anymore
        findHost()->getDisplayString().updateWith("r=16,red");

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
        findHost()->getDisplayString().updateWith("r=16,green");
    } else {
        BaseWaveApplLayer::handleSelfMsg(msg);
    }
}
