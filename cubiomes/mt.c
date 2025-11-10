/*
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   
   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.
   
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
*/

#include <stdio.h>
#include "mt.h"

#define SIZE 624
#define PERIOD 397
#define DIFF (SIZE - PERIOD)

static const uint32_t MAGIC = 0x9908b0df;

struct MTState_
{
  uint32_t MT[SIZE];
  size_t index;
};

typedef struct MTState_ MTState;

#ifdef _MSC_VER
__declspec(thread) static MTState state;
#else
static __thread MTState state;
#endif

#define M32(x) ((x) & 0x80000000)
#define L31(x) ((x) & 0x7FFFFFFF)
#define MIXBITS(u,v) (M32(u) | L31(v))
#define TWIST(u,v) ((MIXBITS(u,v) >> 1) ^ ((v & 1) ? MAGIC : 0))

static inline void generate()
{
  uint32_t *p = state.MT;
  uint32_t *p_end = p + DIFF;
  uint32_t *p_end2 = p + SIZE - 1;
  
  while (p < p_end) {
    *p = p[PERIOD] ^ TWIST(p[0], p[1]);
    ++p;
  }
  
  while (p < p_end2) {
    *p = p[PERIOD - SIZE] ^ TWIST(p[0], p[1]);
    ++p;
  }
  
  *p = p[PERIOD - SIZE] ^ TWIST(p[0], state.MT[0]);
  
  state.index = 0;
}

static inline uint32_t temper(uint32_t y)
{
  y ^= y >> 11;
  y ^= (y << 7) & 0x9d2c5680;
  y ^= (y << 15) & 0xefc60000;
  y ^= y >> 18;
  return y;
}

void mt_seed(uint32_t value)
{
  state.MT[0] = value;
  
  for (uint_fast32_t i = 1; i < SIZE; ++i)
    state.MT[i] = 0x6c078965 * (state.MT[i - 1] ^ (state.MT[i - 1] >> 30)) + i;
  
  state.index = SIZE;
}

uint32_t mt_next()
{
  if (state.index >= SIZE) {
    generate();
  }
  
  return temper(state.MT[state.index++]);
}