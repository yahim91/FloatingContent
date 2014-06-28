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

#ifndef ANCHORZONE_H_
#define ANCHORZONE_H_

#include <csimplemodule.h>
#include<omnetpp.h>
#include<Coord.h>
#include "world/annotations/AnnotationManager.h"

class AnchorZone: public cSimpleModule {
public:
    class Node {
    public:
        int id;
        simtime_t inTime;
        simtime_t outTime;
        int numContacts;

        Node() {
        }
        ;
        Node(const Node& node) :
                id(node.id), inTime(node.inTime), outTime(node.outTime), numContacts(
                        node.numContacts) {
        }
        Node& operator=(const Node& node) {
            this->id = node.id;
            this->inTime = node.inTime;
            this->inTime = node.outTime;
            this->numContacts = node.numContacts;
            return *this;
        }
    };

    static int idx;
    int replicas;
    int infected;
    double avgInfected;
    int irNumSamples;
    int removed;
    double avgRemoved;
    Coord pos;
    AnnotationManager::Annotation* ann;
    bool replicated;
    int contacts;
    simtime_t timeInContact;
    double avgContactsInSJNTime;
    std::map<int, Node> nodes;
    std::map<std::pair<int, int>, int> contactsBetweenNodes;
    simtime_t timeAverage;
    int numTransitNodes;
    int maxTransitNodes;
    cOutVector* currentRemoveVec;
    cOutVector* currentInfectVec;
    cOutVector* currentCopiesVec;

    AnchorZone();
    AnchorZone(Coord, cModule*);
    AnchorZone(const AnchorZone& az) :
            replicas(az.replicas), contacts(az.contacts), pos(az.pos), replicated(
                    az.replicated), timeInContact(az.timeInContact), ann(
                    az.ann), nodes(az.nodes), contactsBetweenNodes(
                    az.contactsBetweenNodes), numTransitNodes(
                    az.numTransitNodes), avgContactsInSJNTime(
                    az.avgContactsInSJNTime), maxTransitNodes(
                    az.maxTransitNodes), infected(az.infected), removed(
                    az.removed) {
    }

    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    void setCoord(Coord p);

    void setAnnotation(AnnotationManager::Annotation*);
    void recordScalars();
    AnchorZone& operator=(const AnchorZone& az) {
        replicas = az.replicas;
        contacts = az.contacts;
        pos = az.pos;
        replicated = az.replicated;
        ann = az.ann;
        nodes = az.nodes;
        timeInContact = az.timeInContact;
        contactsBetweenNodes = az.contactsBetweenNodes;
        numTransitNodes = az.numTransitNodes;
        avgContactsInSJNTime = az.avgContactsInSJNTime;
        maxTransitNodes = az.maxTransitNodes;
        infected = az.infected;
        removed = az.removed;
        currentInfectVec = az.currentInfectVec;
        currentRemoveVec = az.currentRemoveVec;
        currentCopiesVec = az.currentCopiesVec;
        return *this;
    }
};

#endif /* ANCHORZONE_H_ */
