#ifndef WBLEVELMAP_H
#define WBLEVELMAP_H

#include <types.h>
#include <wbcontainer.h>

#include <assert.h>


class WBLevelMap {
    friend class WBContainer;


public:
    struct ItemCoords {
        ItemCoords(const uint8 x, const uint8 y):
            x(x), y(y){}

        const uint8 x;
        const uint8 y;
    };

public:
    WBLevelMap(ItemCoords *map, size_t count);
    ~WBLevelMap();

private:
    size_t mN;
    ItemCoords *mCoords;
};

#endif // WBLEVELMAP_H
