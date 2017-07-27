///
/// @file   api.cpp
/// @brief  primesieve C++ API.
///         Contains the implementations of the functions declared
///         in the primesieve.hpp header file.
///
/// Copyright (C) 2017 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#include <primesieve/config.hpp>
#include <primesieve/CpuInfo.hpp>
#include <primesieve/pmath.hpp>
#include <primesieve/PrimeSieve.hpp>
#include <primesieve/ParallelPrimeSieve.hpp>
#include <primesieve.hpp>

#include <stdint.h>
#include <cstddef>
#include <limits>
#include <string>

namespace {

int sieve_size = 0;

int num_threads = 0;

}

namespace primesieve {

uint64_t nth_prime(int64_t n, uint64_t start)
{
  ParallelPrimeSieve pps;
  pps.setSieveSize(get_sieve_size());
  pps.setNumThreads(get_num_threads());
  return pps.nthPrime(n, start);
}

uint64_t count_primes(uint64_t start, uint64_t stop)
{
  ParallelPrimeSieve pps;
  pps.setSieveSize(get_sieve_size());
  pps.setNumThreads(get_num_threads());
  return pps.countPrimes(start, stop);
}

uint64_t count_twins(uint64_t start, uint64_t stop)
{
  ParallelPrimeSieve pps;
  pps.setSieveSize(get_sieve_size());
  pps.setNumThreads(get_num_threads());
  return pps.countTwins(start, stop);
}

uint64_t count_triplets(uint64_t start, uint64_t stop)
{
  ParallelPrimeSieve pps;
  pps.setSieveSize(get_sieve_size());
  pps.setNumThreads(get_num_threads());
  return pps.countTriplets(start, stop);
}

uint64_t count_quadruplets(uint64_t start, uint64_t stop)
{
  ParallelPrimeSieve pps;
  pps.setSieveSize(get_sieve_size());
  pps.setNumThreads(get_num_threads());
  return pps.countQuadruplets(start, stop);
}

uint64_t count_quintuplets(uint64_t start, uint64_t stop)
{
  ParallelPrimeSieve pps;
  pps.setSieveSize(get_sieve_size());
  pps.setNumThreads(get_num_threads());
  return pps.countQuintuplets(start, stop);
}

uint64_t count_sextuplets(uint64_t start, uint64_t stop)
{
  ParallelPrimeSieve pps;
  pps.setSieveSize(get_sieve_size());
  pps.setNumThreads(get_num_threads());
  return pps.countSextuplets(start, stop);
}

void print_primes(uint64_t start, uint64_t stop)
{
  PrimeSieve ps;
  ps.setSieveSize(get_sieve_size());
  ps.printPrimes(start, stop);
}

void print_twins(uint64_t start, uint64_t stop)
{
  PrimeSieve ps;
  ps.setSieveSize(get_sieve_size());
  ps.printTwins(start, stop);
}

void print_triplets(uint64_t start, uint64_t stop)
{
  PrimeSieve ps;
  ps.setSieveSize(get_sieve_size());
  ps.printTriplets(start, stop);
}

void print_quadruplets(uint64_t start, uint64_t stop)
{
  PrimeSieve ps;
  ps.setSieveSize(get_sieve_size());
  ps.printQuadruplets(start, stop);
}

void print_quintuplets(uint64_t start, uint64_t stop)
{
  PrimeSieve ps;
  ps.setSieveSize(get_sieve_size());
  ps.printQuintuplets(start, stop);
}

void print_sextuplets(uint64_t start, uint64_t stop)
{
  PrimeSieve ps;
  ps.setSieveSize(get_sieve_size());
  ps.printSextuplets(start, stop);
}

int get_num_threads()
{
  if (num_threads)
    return num_threads;
  else
    return ParallelPrimeSieve::getMaxThreads();
}

void set_num_threads(int threads)
{
  num_threads = inBetween(1, threads, ParallelPrimeSieve::getMaxThreads());
}

uint64_t get_max_stop()
{
  return std::numeric_limits<uint64_t>::max();
}

std::string primesieve_version()
{
  return PRIMESIEVE_VERSION;
}

void set_sieve_size(int kilobytes)
{
  sieve_size = inBetween(1, kilobytes, 2048);
  sieve_size = floorPow2(sieve_size);
}

int get_sieve_size()
{
  // user specified sieve size
  if (sieve_size)
    return sieve_size;

  size_t l1CacheSize = cpuInfo.l1CacheSize();
  size_t l2CacheSize = cpuInfo.l2CacheSize();
  size_t l3CacheSize = cpuInfo.l3CacheSize();

  // convert to kilobytes
  l1CacheSize /= 1024;
  l2CacheSize /= 1024;
  l3CacheSize /= 1024;

  // we set the sieve size to the CPU's L2 cache size
  // if the L2 cache is private. If the CPU has both
  // an L2 and L3 cache we assume that each CPU
  // core has a private L2 cache.
  if (l2CacheSize > l1CacheSize &&
      l2CacheSize > 0 &&
      l3CacheSize > 0)
  {
    l2CacheSize = inBetween(32, l2CacheSize, 2048);
    l2CacheSize = floorPow2(l2CacheSize);
    return (int) l2CacheSize;
  }
  else
  {
    // failed to detect the CPU's L1 cache size
    if (l1CacheSize == 0)
      l1CacheSize = 32;

    // if the CPU does not have an L2 cache or if the
    // cache is shared between all CPU cores we
    // set the sieve size to the CPU's L1 cache size.

    l1CacheSize = inBetween(8, l1CacheSize, 2048);
    l1CacheSize = floorPow2(l1CacheSize);
    return (int) l1CacheSize;
  }
}

} // namespace
