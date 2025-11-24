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

static inline int setTerrainSeed(int chunkX, int chunkZ)
{
    int seed = (chunkX * (int)REGION_A) + (chunkZ * (int)REGION_B);
    setSeed(seed);
    return seed;
}

static inline int setPopulationSeed(uint64_t worldSeed, int x, int z)
{
    setSeed(worldSeed);
    uint32_t a = next() | 1;
    uint32_t b = next() | 1;
    int seed = (x * a + z * b) ^ (int)worldSeed;
    setSeed(seed);
    return seed;
}

static inline int setDecorationSeed(int populationSeed, int salt)
{
    int seed = (((uint32_t)populationSeed >> 2) + (populationSeed << 6) + salt - 1640531527) ^ populationSeed;
    setSeed(seed);
    return seed;
}

static inline int setDecorationSeedFromWorld(uint64_t worldSeed, int chunkX, int chunkZ, int salt)
{
    return setDecorationSeed(setPopulationSeed(worldSeed, chunkX, chunkZ), salt);
}

static inline int setCarverSeed(uint64_t worldSeed, int chunkX, int chunkZ)
{
    setSeed(worldSeed);
    int seed = (chunkX * next()) ^ (chunkZ * next()) ^ (int)worldSeed;
    setSeed(seed);
    return seed;
}

static inline int setRegionSeed(uint64_t worldSeed, int regionX, int regionZ, int salt)
{
    int seed = (regionX * REGION_A) + (regionZ * REGION_B) + (int)worldSeed + salt;
    setSeed(seed);
    return seed;
}

static inline int setFortressSeed(uint64_t worldSeed, int chunkX, int chunkZ)
{
    int seed = ((chunkX >> 4) ^ ((chunkZ >> 4) << 4)) ^ (int)worldSeed;
    setSeed(seed);
    return seed;
}

static inline int seedSlimeChunk(int chunkX, int chunkZ)
{
    int seed = (chunkX * 522133279) ^ chunkZ;
    setSeed(seed);
    return seed;
}

#ifdef __cplusplus
}
#endif

#endif