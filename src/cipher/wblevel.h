#ifndef WBLEVEL_H
#define WBLEVEL_H

#include <types.h>
#include <wbcontainer.h>
#include <wblevelmap.h>


class WBContainer;
class WBLevelMap;

class WBLevel {
    friend class WBContainer;

public:
    static const uint8 kItemsCount = 16;

public:
    WBLevel(const byte *key, const WBLevelMap *map);
    void setData(const byte *bytes);

private:
    union Level {
        dword row;
        byte  b[4];
    };
    Level mData[4];
    Level mKey[4];

    const WBLevelMap *mMap;
};

#endif // WBLEVEL_H
