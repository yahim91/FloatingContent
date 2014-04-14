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
        ASSERT(annotations);
        interval = 500;
    }
    //draw border
    annotations->drawLine(new Coord(0, 0, 1.895),
            new Coord(getPgs()->x, 0, 1.895), "blue");
    annotations->drawLine(new Coord(0, 0, 1.895),
            new Coord(0, getPgs()->y, 1.895), "blue");
    annotations->drawLine(new Coord(0, getPgs()->y, 1.895),
            new Coord(getPgs()->x, getPgs()->y, 1.895), "blue");
    annotations->drawLine(new Coord(getPgs()->x, 0, 1.895),
            new Coord(getPgs()->x, getPgs()->y, 1.895), "blue");

    //draw long
    for (int i = interval; i < getPgs()->y; i += interval) {
        annotations->drawLine(new Coord(0, i, 1.895),
                new Coord(getPgs()->x, i, 1.895), "blue");
    }

    //draw lat
    for (int i = interval; i < getPgs()->x; i += interval) {
        annotations->drawLine(new Coord(i, 0, 1.895),
                new Coord(i, getPgs()->y, 1.895), "blue");
    }

}

Coord WorldUtility::get_current_tile(Coord pos) {
    int x = ((int)pos.x / interval) * interval;
    int y = ((int)pos.y / interval) * interval;
    return new Coord(x, y);
}
