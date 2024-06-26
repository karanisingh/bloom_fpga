//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

#include "MurmurHash3.hpp"

//-----------------------------------------------------------------------------

#define FORCE_INLINE inline __attribute__((always_inline))

inline uint32_t rotl32(uint32_t x, int8_t r)
{
  return (x << r) | (x >> (32 - r));
}

#define BIG_CONSTANT(x) (x##LLU)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

// FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i )
// {
//   return p[i];
// }

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

FORCE_INLINE uint32_t fmix32(uint32_t h)
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;

  return h;
}

//-----------------------------------------------------------------------------

void MurmurHash3_x86_32(const uint32_t *key,
                        uint32_t seed, uint32_t *out)
{
  // const uint8_t *data = (const uint8_t *)key;
  // const int nblocks = len / 4;

  uint32_t h1 = seed;

  const uint32_t c1 = 0xcc9e2d51;
  const uint32_t c2 = 0x1b873593;

  //----------
  // body

  // const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);
  uint32_t k1 = *key;

  // Unroll this prob
  // for (int i = -nblocks; i; i++)
  //{
  // uint32_t k1 = blocks[i]; // getblock32(blocks,i);

  k1 *= c1;
  k1 = rotl32(k1, 15);
  k1 *= c2;

  h1 ^= k1;
  h1 = rotl32(h1, 13);
  h1 = h1 * 5 + 0xe6546b64;
  //}

  //----------
  // tail

  // Not needed since we oonly operate on 32_bit uints
  /*
  const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

  uint32_t k1 = 0;

  switch (len & 3)
  {
  case 3:
    k1 ^= tail[2] << 16;
  case 2:
    k1 ^= tail[1] << 8;
  case 1:
    k1 ^= tail[0];
    k1 *= c1;
    k1 = rotl32(k1, 15);
    k1 *= c2;
    h1 ^= k1;
  };
  */

  //----------
  // finalization

  // h1 ^= len;
  h1 ^= 4; // length is always 4 bytes

  h1 = fmix32(h1);

  // no longer need to cast
  *out = h1;
}
