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

#include "WorldUtility.h"

Define_Module(WorldUtility);

void WorldUtility::initialize(int stage) {
    BaseWorldUtility::initialize(stage);
    if (stage == 0) {
        annotations = AnnotationManagerAccess().getIfExists();
        manager = TraCIScenarioManagerAccess().get();
        ASSERT(annotations);
        ASSERT(manager);

        anchorSize = par("anchorRange");
        anchorDistance = par("anchorDistance");
    }

    for (double i = 0; i <= getPgs()->x; i += anchorDistance) {
        for (double j = 0; j <= getPgs()->y; j += anchorDistance) {

            AnnotationManager::Annotation* circle = annotations->drawPoint(
                    Coord(i, j), "blue", "", anchorSize);
            manager->addPOIReplica(Coord(i, j),
                    circle);

        }
    }

}

Coord WorldUtility::get_current_tile(Coord pos) {
    int x = ((int) pos.x / anchorSize) * anchorSize;
    int y = ((int) pos.y / anchorSize) * anchorSize;
    return new Coord(x, y);
}

int WorldUtility::getAnchorSize() {
    return anchorSize;
}

int WorldUtility::getMaxX() {
    return getPgs()->x / anchorDistance;
}

int WorldUtility::getMaxY() {
    return getPgs()->y / anchorDistance;
}
