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

#ifndef FLOATINGCONTENTAPP_H_
#define FLOATINGCONTENTAPP_H_

#include "BaseWaveApplLayer.h"
#include "mobility/traci/TraCIMobility.h"
#include <WorldUtility.h>
#include "AnnotationManager.h"
#include <set>
#include <utility>
#include <algorithm>
#include <FloatingContentMessage_m.h>

class FloatingContentApp: public BaseWaveApplLayer {
public:
    class Statistics {
    public:
        int contacts;

        void initialize();
        void watch(cSimpleModule& module);
        void recordScalars(cSimpleModule& module);
    };
    virtual void initialize(int stage);
    virtual void finish();
    typedef std::pair<double, double> Tile;

protected:
    WorldUtility* world;
    TraCIMobility* traci;
    TraCIScenarioManager* traciManager;
    AnnotationManager* annotations;
    simtime_t lastDroveAt;
    Coord currentTile;
    bool sentMessage;
    std::vector<int> activePeers;
    std::map<std::pair<double, double>, bool> currentAnchors;
    std::vector<Coord> tiles;
    cOutVector currentTilesX;
    cOutVector currentTilesY;
    Statistics statistics;

    // point of interest parameters
    int poiCount;
    double poiStart;
    cMessage *startPoiMsg;
    cMessage *stopPoiMsg;
    cMessage *refreshPoi;
    bool setCircle;
    Coord circleCoord;
    char circleDisplayString[50];
    int poiReplicationRange;
    simtime_t ttl;
    std::string poiColor;
    simtime_t poiTTL;
    simtime_t refreshInterval;
    bool stored;

protected:
    virtual void onBeacon(WaveShortMessage* wsm);
    virtual void onData(WaveShortMessage* wsm);
    void sendMessage(Storage storage, int address);
    virtual void handlePositionUpdate(cObject* obj);
    virtual void handleSelfMsg(cMessage* msg);
    //virtual void handleLowerMsg(cMessage* msg);
    virtual FloatingContentMessage* prepareMessage(std::string name,
            int dataLengthBits, t_channel channel, int priority, int rcvId,
            int serial = 0);
    void refreshLocalStorage();

};

#endif /* FLOATINGCONTENTAPP_H_ */
