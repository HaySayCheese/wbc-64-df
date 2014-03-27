#ifndef WBCONTAINER_H
#define WBCONTAINER_H


#include <types.h>
#include <wblevel.h>
#include <wblevelmap.h>

#include <limits>
#include <cstring> // memcpy
#include <assert.h>


class WBLevel;
class WBLevelMap;

class WBContainer {
public:
    static const uint8 kLevelsCount = 4;
    static const uint8 kSideSize = kLevelsCount;

public:
    WBContainer(byte *key);
    void processBlock(byte *dataBuffer, byte *resultBuffer);

private:
    void processLevel(uint8 index);
    inline void processItem(const uint8 levelIndex, const uint8 x, const uint8 y);
    inline void processBlueItem(const uint8 levelIndex, const uint8 x, const uint8 y, const byte key);
    inline void processWhiteItem(const uint8 levelIndex, const uint8 x, const uint8 y, const byte key);

    inline WBLevel* upperLevel(const uint8 levelIndex) const;
    inline WBLevel* lowerLevel(const uint8 levelIndex) const;

    static inline uint8 rightNeighborIndex(uint8 x);
    static inline uint8 leftNeighborIndex(uint8 x);
    static inline uint8 topNeighborIndex(uint8 y);
    static inline uint8 bottomNeighborIndex(uint8 y);

    inline dword rorDWord(const dword data, const uint8 n);
    inline dword rolDWord(const dword data, const uint8 n);

private:
    WBLevel *mLevels[4];
};

#endif // WBCONTAINER_H
