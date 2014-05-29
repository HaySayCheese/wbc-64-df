#include "wbcontainer.h"


WBContainer::WBContainer(byte *key){
    assert(key != nullptr);

    /* maps initialisation */
    // map 0
    WBLevelMap::ItemCoords coords0[] = {
        WBLevelMap::ItemCoords(0,0), WBLevelMap::ItemCoords(1,0),
        WBLevelMap::ItemCoords(2,0), WBLevelMap::ItemCoords(3,0),
        WBLevelMap::ItemCoords(0,1), WBLevelMap::ItemCoords(1,1),
        WBLevelMap::ItemCoords(2,1), WBLevelMap::ItemCoords(3,1),
        WBLevelMap::ItemCoords(0,2), WBLevelMap::ItemCoords(1,2),
        WBLevelMap::ItemCoords(2,2), WBLevelMap::ItemCoords(3,2),
        WBLevelMap::ItemCoords(0,3), WBLevelMap::ItemCoords(1,3),
        WBLevelMap::ItemCoords(2,3), WBLevelMap::ItemCoords(3,2),
    };
    auto heapCoords0 = (WBLevelMap::ItemCoords*) malloc(sizeof(coords0));
    memcpy(heapCoords0, &coords0, sizeof(coords0));

    // map 1
    WBLevelMap::ItemCoords coords1[] = {
        WBLevelMap::ItemCoords(0,0), WBLevelMap::ItemCoords(0,1),
        WBLevelMap::ItemCoords(1,1), WBLevelMap::ItemCoords(1,0),
        WBLevelMap::ItemCoords(2,0), WBLevelMap::ItemCoords(3,0),
        WBLevelMap::ItemCoords(3,1), WBLevelMap::ItemCoords(2,1),
        WBLevelMap::ItemCoords(2,2), WBLevelMap::ItemCoords(3,2),
        WBLevelMap::ItemCoords(3,3), WBLevelMap::ItemCoords(2,3),
        WBLevelMap::ItemCoords(1,3), WBLevelMap::ItemCoords(1,2),
        WBLevelMap::ItemCoords(0,2), WBLevelMap::ItemCoords(0,3),
    };
    auto heapCoords1 = (WBLevelMap::ItemCoords*) malloc(sizeof(coords1));
    memcpy(heapCoords1, &coords1, sizeof(coords1));

    // map 2
    WBLevelMap::ItemCoords coords2[] = {
        WBLevelMap::ItemCoords(3,3), WBLevelMap::ItemCoords(2,3),
        WBLevelMap::ItemCoords(1,3), WBLevelMap::ItemCoords(0,3),
        WBLevelMap::ItemCoords(0,2), WBLevelMap::ItemCoords(0,1),
        WBLevelMap::ItemCoords(0,0), WBLevelMap::ItemCoords(1,0),
        WBLevelMap::ItemCoords(2,0), WBLevelMap::ItemCoords(3,0),
        WBLevelMap::ItemCoords(3,1), WBLevelMap::ItemCoords(3,2),
        WBLevelMap::ItemCoords(2,2), WBLevelMap::ItemCoords(1,2),
        WBLevelMap::ItemCoords(1,1), WBLevelMap::ItemCoords(2,1),
    };
    auto heapCoords2 = (WBLevelMap::ItemCoords*) malloc(sizeof(coords2));
    memcpy(heapCoords2, &coords2, sizeof(coords2));

    // map 3
    WBLevelMap::ItemCoords coords3[] = {
        WBLevelMap::ItemCoords(0,0), WBLevelMap::ItemCoords(1,0),
        WBLevelMap::ItemCoords(1,1), WBLevelMap::ItemCoords(0,1),
        WBLevelMap::ItemCoords(0,2), WBLevelMap::ItemCoords(1,2),
        WBLevelMap::ItemCoords(1,3), WBLevelMap::ItemCoords(0,3),
        WBLevelMap::ItemCoords(3,3), WBLevelMap::ItemCoords(2,3),
        WBLevelMap::ItemCoords(2,2), WBLevelMap::ItemCoords(3,2),
        WBLevelMap::ItemCoords(3,1), WBLevelMap::ItemCoords(2,1),
        WBLevelMap::ItemCoords(2,0), WBLevelMap::ItemCoords(3,0),
    };
    auto heapCoords3 = (WBLevelMap::ItemCoords*) malloc(sizeof(coords3));
    memcpy(heapCoords3, &coords3, sizeof(coords3));


    mMaps[0] = new WBLevelMap(heapCoords0, 16);
    mMaps[1] = new WBLevelMap(heapCoords1, 16);
    mMaps[2] = new WBLevelMap(heapCoords2, 16);
    mMaps[3] = new WBLevelMap(heapCoords3, 16);


    /* levels initialisation */
    for (uint8 i=0; i<WBContainer::kLevelsCount; ++i){
        byte *levelKey = key + i*WBLevel::kItemsCount;

        size_t levelKeySum = 0;
        for (uint8 k=0; k<16; k++)
            levelKeySum += levelKey[i];

        size_t m = levelKeySum % 4;
        mLevels[i] = new WBLevel(levelKey, mMaps[m]);
    }
}

WBContainer::~WBContainer(){
    for (size_t i=0; i<kSideSize; ++i){
        delete mLevels[i];
    }
    for (size_t i=0; i<kSideSize; ++i){
        delete mMaps[i];
    }
}

void WBContainer::processBlock(byte *dataBuffer, byte *resultBuffer){
    assert(dataBuffer != nullptr);
    assert(resultBuffer != nullptr);
    assert(resultBuffer != dataBuffer);

    // setting data
    byte *data = dataBuffer;
    for (uint8 i=0; i<WBContainer::kLevelsCount; ++i){
        mLevels[i]->setData(data);
        data += WBLevel::kItemsCount;
    }

    // processing
    for (uint8 round=0; round<3; ++round){
        for (uint8 i=0; i<WBContainer::kLevelsCount; ++i){
            processLevel(i);
        }
    }

    memcpy(resultBuffer,    &mLevels[0]->mData, 16);
    memcpy(resultBuffer+16, &mLevels[1]->mData, 16);
    memcpy(resultBuffer+32, &mLevels[2]->mData, 16);
    memcpy(resultBuffer+48, &mLevels[3]->mData, 16);
}

void WBContainer::processLevel(uint8 levelIndex){
    assert(levelIndex < WBContainer::kLevelsCount);

    auto level = mLevels[levelIndex];
    const auto *map = level->mMap;
    for (size_t iteration=0; iteration<level->mMap->mN; ++iteration){
        WBLevelMap::ItemCoords next = map->mCoords[iteration];
        processItem(levelIndex, next.x, next.y);
    }
}

void WBContainer::processItem(const uint8 levelIndex, const uint8 x, const uint8 y){
    assert(levelIndex < WBContainer::kLevelsCount);
    assert(x < WBContainer::kSideSize);
    assert(y < WBContainer::kSideSize);

    auto *level = mLevels[levelIndex];
    auto key = level->mKey[y].b[x];

    if (key & 0b10000000)
        processBlueItem(levelIndex, x, y, key);
    else
        processWhiteItem(levelIndex, x, y, key);
}

void WBContainer::processBlueItem(const uint8 levelIndex, const uint8 x, const uint8 y, const byte key){
    assert(levelIndex < WBContainer::kLevelsCount);
    assert(x < WBContainer::kSideSize);
    assert(y < WBContainer::kSideSize);

    bool moveUp = ((key >> 6) & 1);
    bool moveLeft = ((key >> 5) & 1);

    WBLevel *level = mLevels[levelIndex];
    WBLevel *nextLevel = nullptr;
    if (moveUp)
        nextLevel = upperLevel(levelIndex);
    else
        nextLevel = lowerLevel(levelIndex);

    uint8 nextX;
    if (moveLeft)
        nextX = leftNeighborIndex(x);
    else
        nextX = rightNeighborIndex(x);

    dword *pLevelKey = &level->mKey[y].row;
    dword *pNextLevelKey = &nextLevel->mKey[y].row;

    // first step
    byte *pk0 = &level->mKey[y].b[x];
    byte *pd0 = &level->mData[y].b[x];
    byte *pk1 = &nextLevel->mKey[y].b[x];

    *pk0 = ~(*pk1 ^ *pk0); // first step uses inverted XOR
    *pLevelKey = rolDWord(*pLevelKey, 5);
    *pd0 ^= *pk0;

    // second step
    byte *pk2 = &nextLevel->mKey[y].b[nextX];
    byte *pd1 = &nextLevel->mData[y].b[x];

    *pk1 ^= *pk2;
    *pNextLevelKey = rolDWord(*pNextLevelKey, 5);
    *pd1 ^= *pk1;

    // third step
    byte *pk3 = &level->mKey[y].b[nextX];
    byte *pd2 = &nextLevel->mData[y].b[nextX];
    
    *pk2 = ~(*pk3 ^ *pk2); // third iteration uses inverted XOR
    *pNextLevelKey = rolDWord(*pNextLevelKey, 7);
    *pd2 ^= *pk2;

    // fourth step
    byte *pd3 = &level->mData[y].b[nextX];

    *pk3 ^= *pk0;
    *pLevelKey = rolDWord(*pLevelKey, 7);
    *pd3 ^= *pk3;
}

void WBContainer::processWhiteItem(uint8 levelIndex, uint8 x, uint8 y, byte key){
    assert(levelIndex < WBContainer::kLevelsCount);
    assert(x < WBContainer::kSideSize);
    assert(y < WBContainer::kSideSize);

    bool moveUp = ((key >> 6) & 1);
    bool moveLeft = ((key >> 5) & 1);

    WBLevel *level = mLevels[levelIndex];
    dword *pLevelKey = &level->mKey[y].row;

    uint8 nextY;
    if (moveUp)
        nextY = topNeighborIndex(y);
    else
        nextY = bottomNeighborIndex(y);

    uint8 nextX;
    if (moveLeft)
        nextX = leftNeighborIndex(x);
    else
        nextX = rightNeighborIndex(x);


    // first step
    byte *pk0 = &level->mKey[y].b[x];
    byte *pd0 = &level->mData[y].b[x];
    byte *pk1 = &level->mKey[nextY].b[x];

    *pk0 = ~(*pk1 ^ *pk0); // first step uses inverted XOR
    *pLevelKey = rolDWord(*pLevelKey, 5);
    *pd0 ^= *pk0;

    // second step
    byte *pk2 = &level->mKey[nextY].b[nextX];
    byte *pd1 = &level->mData[nextY].b[x];

    *pk1 ^= *pk2;
    *pLevelKey = rolDWord(*pLevelKey, 5);
    *pd1 ^= *pk1;

    // third step
    byte *pk3 = &level->mKey[y].b[nextX];
    byte *pd2 = &level->mData[nextY].b[nextX];

    *pk2 = ~(*pk3 ^ *pk2); // third iteration uses inverted XOR
    *pLevelKey = rolDWord(*pLevelKey, 7);
    *pd2 ^= *pk2;

    // fourth step
    byte *pd3 = &level->mData[y].b[nextX];

    *pk3 ^= *pk0;
    *pLevelKey = rolDWord(*pLevelKey, 7);
    *pd3 ^= *pk3;
}

WBLevel* WBContainer::upperLevel(const uint8 levelIndex) const {
    assert(levelIndex < WBContainer::kLevelsCount);

    if (levelIndex == 0)
        return mLevels[kSideSize - 1];
    return mLevels[levelIndex - 1];
}

WBLevel* WBContainer::lowerLevel(const uint8 levelIndex) const {
    assert(levelIndex < WBContainer::kLevelsCount);

    if (levelIndex == kSideSize - 1)
        return mLevels[0];
    return mLevels[levelIndex + 1];
}

dword WBContainer::rorDWord(const dword data, uint8 n){
    assert(n <= sizeof(data) * 8);

    return (data << n) | (data >> (32 - n));
}

dword WBContainer::rolDWord(const dword data, uint8 n){
    assert(n <= sizeof(data) * 8);

    return (data >> n) | (data << (32 - n));
}

uint8 WBContainer::rightNeighborIndex(uint8 x){
    assert(x < WBContainer::kSideSize);

    ++x;
    if (x >= kSideSize-1)
        x = 0;
    return x;
}

uint8 WBContainer::leftNeighborIndex(uint8 x){
    assert(x < WBContainer::kSideSize);

    --x;
    if (x == std::numeric_limits<uint8>::max()) // WARN: overflow here
        x = kSideSize-1;
    return x;
}

uint8 WBContainer::bottomNeighborIndex(uint8 y){
    assert(y < WBContainer::kSideSize);

    ++y;
    if (y >= kSideSize-1)
        y = 0;
    return y;
}

uint8 WBContainer::topNeighborIndex(uint8 y){
    assert(y < WBContainer::kSideSize);

    --y;
    if (y == std::numeric_limits<uint8>::max()) // WARN: overflow here
        y = kSideSize-1;
    return y;
}
