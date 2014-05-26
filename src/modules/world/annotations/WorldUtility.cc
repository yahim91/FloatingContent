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

        anchorSize = 500;//par("anchor_size");
    }

    for (double i = 0; i < getPgs()->x; i += anchorSize * 2) {
        for (double j = 0; j < getPgs()->y; j += anchorSize * 2) {
            AnnotationManager::Annotation* circle = annotations->drawPoint(
                    Coord(i + anchorSize, j + anchorSize), "blue", "");
            manager->addPOIReplica(Coord(i + anchorSize, j + anchorSize), circle);
        }
    }

}

Coord WorldUtility::get_current_tile(Coord pos) {
    int x = ((int) pos.x / anchorSize) * anchorSize;
    int y = ((int) pos.y / anchorSize) * anchorSize;
    return new Coord(x, y);
}
