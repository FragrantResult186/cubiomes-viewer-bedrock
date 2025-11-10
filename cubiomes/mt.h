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

static inline void setSeed(uint64_t *seed, uint32_t value)
{
    mt_seed(value);
}

static inline int next(uint64_t *seed)
{
    return (int)(mt_next() >> 1);
}

static inline int nextInt(uint64_t *seed, const int n)
{
    return (int)(mt_next() % n);
}

static inline int nextIntRange(uint64_t *seed, const int a, const int b)
{
    return (a < b) ? (a + nextInt(seed, b - a)) : a;
}

static inline int nextBoolean(uint64_t *seed)
{
    return mt_next() & 0x8000000 != 0;
}

static inline double nextDouble(uint64_t *seed)
{
    return mt_next() * (1.0 / 4294967296.0);
}

static inline float nextFloat(uint64_t *seed)
{
    return (float)nextDouble(seed);
}

static inline void skipNextN(uint64_t *seed, int n)
{
    for (int i = 0; i < n; i++)
        mt_next();
}

//==============================================================================
// ChunkRandom
//==============================================================================

static inline int setTerrainSeed(uint64_t *rng, int chunkX, int chunkZ)
{
    int seed = (chunkX * (int)341873128712L) + (chunkZ * (int)132897987541L);
    setSeed(rng, seed);
    return seed;
}

static inline int setPopulationSeed(uint64_t *rng, uint64_t worldSeed, int x, int z)
{
    setSeed(rng, worldSeed);
    uint64_t a = next(rng) | 1;
    uint64_t b = next(rng) | 1;
    int seed = (x * a + z * b) ^ (int)worldSeed;
    setSeed(rng, seed);
    return seed;
}

static inline int setDecorationSeed(uint64_t *rng, int populationSeed, int salt)
{
    int seed = ((uint32_t)populationSeed >> 2) + (populationSeed << 6) + salt - 1640531527 ^ populationSeed;
    setSeed(rng, seed);
    return seed;
}

static inline int setDecorationSeedFromWorld(uint64_t *rng, uint64_t worldSeed, int chunkX, int chunkZ, int salt)
{
    return setDecorationSeed(rng, setPopulationSeed(rng, worldSeed, chunkX, chunkZ), salt);
}

static inline int setCarverSeed(uint64_t *rng, uint64_t worldSeed, int chunkX, int chunkZ)
{
    setSeed(rng, worldSeed);
    int seed = (chunkX * next(rng)) ^ (chunkZ * next(rng)) ^ (int)worldSeed;
    setSeed(rng, seed);
    return seed;
}

static inline int setRegionSeed(uint64_t *rng, uint64_t worldSeed, int regionX, int regionZ, int salt)
{
    int seed = (regionX * (int)341873128712L) + (regionZ * (int)132897987541L) + (int)worldSeed + salt;
    setSeed(rng, seed);
    return seed;
}

static inline int setStrongholdSeed(uint64_t *rng, uint64_t worldSeed, int regionX, int regionZ, int salt)
{
    int seed = (regionX * (int)784295783249L) + (regionZ * (int)827828252345L) + (int)worldSeed + salt;
    setSeed(rng, seed);
    return seed;
}

static inline int setFortressSeed(uint64_t *rng, uint64_t worldSeed, int chunkX, int chunkZ)
{
    int seed = ((chunkX >> 4) ^ ((chunkZ >> 4) << 4)) ^ (int)worldSeed;
    setSeed(rng, seed);
    return seed;
}

static inline int seedSlimeChunk(uint64_t *rng, int chunkX, int chunkZ)
{
    int seed = (chunkX * 522133279) ^ chunkZ;
    setSeed(rng, seed);
    return seed;
}

#ifdef __cplusplus
}
#endif

#endif