//
// This program is free software: you can redistribute it and/or thisify
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

#include <AnchorZone.h>

Define_Module(AnchorZone);
AnchorZone::AnchorZone() {
}

int AnchorZone::idx;

void AnchorZone::initialize() {
    this->replicas = 0;
    this->contacts = 0;
    this->numTransitNodes = 0;
    this->avgContactsInSJNTime = 0;
    this->maxTransitNodes = 0;
    this->infected = 0;
    this->removed = 0;
    this->currentCopiesVec = new cOutVector("copies");
    this->currentInfectVec = new cOutVector("infect");
    this->currentRemoveVec = new cOutVector("remove");
}

void AnchorZone::finish() {

}

void AnchorZone::handleMessage(cMessage *msg) {

}
void AnchorZone::setCoord(Coord pos) {
    this->pos = pos;
}

void AnchorZone::setAnnotation(AnnotationManager::Annotation* ann) {
    this->ann = ann;
}

void AnchorZone::recordScalars() {
    double numContacts;
    double avgTimeInAnchor;
    typedef std::map<std::pair<int, int>, int>::iterator it_type;
    typedef std::map<int, Node>::iterator node_type;
    std::map<std::pair<int, int>, int>::iterator contactsIt;
    int peers, contactsNum;
    simtime_t endTime, totalTime;

    double encounters = 0;

    if (contactsBetweenNodes.size()) {
        for (it_type i = contactsBetweenNodes.begin();
                i != contactsBetweenNodes.end(); i++) {
            encounters += i->second;
        }
        //encounters /= contactsBetweenNodes.size();
    }

    for (node_type it = nodes.begin(); it != nodes.end(); it++) {
        endTime = simTime();
        totalTime = endTime - it->second.inTime;
        timeAverage += totalTime;
        numTransitNodes++;
        peers = 0;
        contactsNum = 0;
        for (contactsIt = contactsBetweenNodes.begin();
                contactsIt != contactsBetweenNodes.end(); contactsIt++) {
            if (contactsIt->first.first == it->first
                    || contactsIt->first.second == it->first) {
                contactsNum += contactsIt->second;
                peers++;

            }
        }
        if (peers != 0 && totalTime.inUnit(SIMTIME_S) != 0) {
            avgContactsInSJNTime += ((double) contactsNum / peers)
                    / totalTime.inUnit(SIMTIME_S);
        }
    }
    if (numTransitNodes == 0) {
        avgTimeInAnchor = 0;
        encounters = 0;
        avgContactsInSJNTime = 0;
    } else {
        avgTimeInAnchor = (double) timeAverage.inUnit(SIMTIME_S)
                / numTransitNodes;
        encounters /= numTransitNodes;
        avgContactsInSJNTime /= numTransitNodes;
    }

    this->recordScalar("criticality", numTransitNodes * encounters);
    this->recordScalar("criticality2",
            numTransitNodes * avgContactsInSJNTime * avgTimeInAnchor);
    this->recordScalar("criticality3",
            maxTransitNodes * avgContactsInSJNTime * avgTimeInAnchor);
    this->recordScalar("removed", removed);
    this->recordScalar("infected", infected);
    this->recordScalar("transit", numTransitNodes);
    this->recordScalar("contactsPerSecond", avgContactsInSJNTime);
    this->recordScalar("timeAverage", avgTimeInAnchor);
    this->recordScalar("avgContacts", encounters);
    this->recordScalar("contactTime", timeInContact.inUnit(SIMTIME_MS));
    this->recordScalar("contacts", this->contacts);
    this->recordScalar("replicas", this->replicas);
    this->recordScalar("xpos", this->pos.x);
    this->recordScalar("ypos", this->pos.y);
    this->recordScalar("numNodes", this->nodes.size());
}
