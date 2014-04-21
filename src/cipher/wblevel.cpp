#include "wblevel.h"

WBLevel::WBLevel(const byte *key, const WBLevelMap *map){
    mMap = map;
    memcpy(mKey, key, WBLevel::kItemsCount);
    memset(mData, 0, WBLevel::kItemsCount);
}

void WBLevel::setData(const byte *bytes){
    memcpy(mData, bytes, WBLevel::kItemsCount);
}
