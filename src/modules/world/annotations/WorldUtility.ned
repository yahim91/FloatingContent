package org.mixim.modules.world.annotations;
import org.mixim.base.modules.BaseWorldUtility;

simple WorldUtility extends BaseWorldUtility {
    @class(WorldUtility);
    int anchorRange @unit(m) = default(1m);
    int anchorDistance @unit(m) = default(1m);
}