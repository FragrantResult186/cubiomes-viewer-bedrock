#ifndef mt_H_
#define mt_H_

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

uint32_t mt_next();
void mt_seed(uint32_t seed_value);

static inline void setSeed(uint32_t value)
{
    mt_seed(value);
}

static inline int next()
{
    return (int)(mt_next() >> 1);
}

static inline int nextInt(const int n)
{
    return (int)(mt_next() % n);
}

static inline int nextIntRange(const int a, const int b)
{
    return (a < b) ? (a + nextInt(b - a)) : a;
}

static inline int nextBoolean()
{
    return (mt_next() & 0x8000000) != 0;
}

static inline double nextDouble()
{
    return mt_next() * (1.0 / 4294967296.0);
}

static inline float nextFloat()
{
    return (float)nextDouble();
}

static inline void skipNextN(int n)
{
    for (int i = 0; i < n; i++) mt_next();
}

//==============================================================================
// ChunkRandom
//==============================================================================

#define REGION_A 341873128712L
#define REGION_B 132897987541L

static inline int32_t setTerrainSeed(int32_t chunkX, int32_t chunkZ)
{
    uint32_t seed = (uint32_t)((int64_t)chunkX * REGION_A + (int64_t)chunkZ * REGION_B);
    setSeed(seed);
    return (int32_t)seed;
}

static inline uint32_t setPopulationSeed(uint64_t worldSeed, int32_t chunkX, int32_t chunkZ)
{
    setSeed(worldSeed);
    uint32_t a = next() | 1;
    uint32_t b = next() | 1;
    uint32_t seed = (chunkX * a + chunkZ * b) ^ (uint32_t)worldSeed;
    setSeed(seed);
    return seed;
}

static inline uint32_t setDecorationSeed(uint64_t worldSeed, int32_t chunkX, int32_t chunkZ, int salt)
{
    setSeed(worldSeed);
    uint32_t a = next() | 1;
    uint32_t b = next() | 1;
    uint32_t popSeed = (chunkX * a + chunkZ * b) ^ (uint32_t)worldSeed;
    uint32_t seed = ((popSeed >> 2) + (popSeed << 6) + (uint32_t)salt - 1640531527u) ^ popSeed;
    setSeed(seed);
    return seed;
}

static inline uint32_t setCarverSeed(uint64_t worldSeed, int32_t chunkX, int32_t chunkZ)
{
    setSeed((uint32_t)worldSeed);
    uint32_t seed = (chunkX * next()) ^ (chunkZ * next()) ^ (uint32_t)worldSeed;
    setSeed(seed);
    return seed;
}

static inline uint32_t setRegionSeed(uint64_t worldSeed, int32_t regionX, int32_t regionZ, int32_t salt)
{
    uint32_t seed = (uint32_t)((int64_t)regionX * REGION_A) + (uint32_t)((int64_t)regionZ * REGION_B) + (uint32_t)worldSeed + (uint32_t)salt;
    setSeed(seed);
    return seed;
}

static inline uint32_t setFortressSeed(uint64_t worldSeed, int32_t chunkX, int32_t chunkZ)
{
    uint32_t seed = ((chunkX >> 4) ^ ((chunkZ >> 4) << 4)) ^ (uint32_t)worldSeed;
    setSeed(seed);
    return seed;
}

static inline uint32_t seedSlimeChunk(int32_t chunkX, int32_t chunkZ)
{
    uint32_t seed = ((uint32_t)chunkX * 522133279U) ^ (uint32_t)chunkZ;
    setSeed(seed);
    return (int32_t)seed;
}

#ifdef __cplusplus
}
#endif

#endif