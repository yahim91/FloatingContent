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

package org.mixim.modules.application.traci;
import org.mixim.modules.application.ieee80211p.BaseWaveApplLayer;

simple FloatingContentApp extends BaseWaveApplLayer
{    
    parameters:          
	@class(FloatingContentApp);
	@display("i=block/app2");
	int poiCount = default(0);
	double poiStart @unit("s") = default(uniform(30s,60s));  // time until first generate poi
	int poiReplicationRange @unit("m") = default(0); //replication range
	string poiColor = default("black");
	int poiTTL @unit("s") = default(0);
	int ttl @unit("s") = default(0);
	int refreshInterval @unit("s") = default(0);
	
}