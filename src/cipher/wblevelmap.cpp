#include "wblevelmap.h"

WBLevelMap::WBLevelMap(ItemCoords *map, size_t count){
    assert(map != nullptr);
    assert(count > 0);

    mCoords = map;
    mN = count;
}
