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

#ifndef WORLDUTILITY_H_
#define WORLDUTILITY_H_

#include <BaseWorldUtility.h>
#include "world/annotations/AnnotationManager.h"
#include "mobility/traci/TraCIScenarioManager.h"

class WorldUtility: public BaseWorldUtility {

protected:
    AnnotationManager* annotations;
    TraCIScenarioManager* manager;
    int anchorSize;
    int anchorDistance;

public:
    //WorldUtility();
    //virtual ~WorldUtility();
    virtual void initialize(int stage);
    int getAnchorSize();
    Coord get_current_tile(Coord coord);
    int getMaxX();
    int getMaxY();
};

class Tile: public std::pair<double, double>
{
    public:
        Tile(double a, double b): std::pair<double, double>(a,b){};
        bool operator==(const Tile& tile) {
            return this->first == tile.first && this->second == tile.second;
        }
};

#endif /* WORLDUTILITY_H_ */
