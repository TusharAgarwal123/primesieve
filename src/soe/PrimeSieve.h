///
/// @file   PrimeSieve.h
/// @brief  The PrimeSieve class provides an easy API for prime
///         sieving (single-threaded).
///
/// Copyright (C) 2013 Kim Walisch, <kim.walisch@gmail.com>
///
/// This file is distributed under the BSD License. See the COPYING
/// file in the top level directory.
///

#ifndef PRIMESIEVE_H
#define PRIMESIEVE_H

#define PRIMESIEVE_VERSION "4.3"
#define PRIMESIEVE_MAJOR_VERSION 4
#define PRIMESIEVE_MINOR_VERSION 3
#define PRIMESIEVE_YEAR "2013"

#include "PrimeSieveCallback.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <exception>
#include <cmath>

namespace soe {
  class PrimeFinder;
  class LockGuard;
}

/// PrimeSieve is a highly optimized C++ implementation of the
/// segmented sieve of Eratosthenes that generates primes and prime
/// k-tuplets (twin primes, prime triplets, ...) in order up to 2^64
/// maximum. The README file describes the algorithms used in more
/// detail and doc/EXAMPLES contains source code examples.
///
class PrimeSieve {
  friend class soe::PrimeFinder;
  friend class soe::LockGuard;
public:
  /// Public flags for use with setFlags(int)
  /// @pre flag < (1 << 20)
  enum {
    COUNT_PRIMES      = 1 << 0,
    COUNT_TWINS       = 1 << 1,
    COUNT_TRIPLETS    = 1 << 2,
    COUNT_QUADRUPLETS = 1 << 3,
    COUNT_QUINTUPLETS = 1 << 4,
    COUNT_SEXTUPLETS  = 1 << 5,
    COUNT_SEPTUPLETS  = 1 << 6,
    PRINT_PRIMES      = 1 << 7,
    PRINT_TWINS       = 1 << 8,
    PRINT_TRIPLETS    = 1 << 9,
    PRINT_QUADRUPLETS = 1 << 10,
    PRINT_QUINTUPLETS = 1 << 11,
    PRINT_SEXTUPLETS  = 1 << 12,
    PRINT_SEPTUPLETS  = 1 << 13,
    PRINT_STATUS      = 1 << 14,
    CALCULATE_STATUS  = 1 << 15
  };
  PrimeSieve();
  PrimeSieve(PrimeSieve&, int);
  virtual ~PrimeSieve();
  // Getters
  uint64_t getStart() const;
  uint64_t getStop() const;
  int getSieveSize() const;
  int getFlags() const;
  double getStatus() const;
  double getSeconds() const;
  // Setters
  void setStart(uint64_t);
  void setStop(uint64_t);
  void setSieveSize(int);
  void setFlags(int);
  void addFlags(int);
  // Bool is*
  bool isFlag(int) const;
  bool isGenerate() const;
  bool isCount() const;
  bool isCount(int) const;
  bool isPrint() const;
  bool isPrint(int) const;
  // Sieve
  virtual void sieve();
  void sieve(uint64_t, uint64_t);
  void sieve(uint64_t, uint64_t, int);
  // Generate
  template <typename T>
  void generatePrimes(uint64_t start, uint64_t stop, std::vector<T>* vect)
  {
    if (vect) {
      PushBackPrimes1<T> pbp(*vect);
      pbp.generatePrimes(start, stop, *this);
    }
  }
  template <typename T>
  void generate_N_Primes(uint64_t start, uint64_t n, std::vector<T>* vect)
  {
    if (vect) {
      PushBackPrimes2<T> pbp(*vect);
      pbp.generate_N_Primes(start, n, *this);
    }
  }
  void generatePrimes(uint32_t, uint32_t, void (*)(uint32_t));
  void generatePrimes(uint64_t, uint64_t, void (*)(uint64_t));
  void generatePrimes(uint64_t, uint64_t, void (*)(uint64_t, int));
  void generatePrimes(uint32_t, uint32_t, PrimeSieveCallback<uint32_t>*);
  void generatePrimes(uint64_t, uint64_t, PrimeSieveCallback<uint64_t>*);
  void generatePrimes(uint64_t, uint64_t, PrimeSieveCallback<uint64_t, int>*);
  // nth prime
  uint64_t nthPrime(uint64_t);
  uint64_t nthPrime(uint64_t, uint64_t);
  // Print
  void printPrimes(uint64_t, uint64_t);
  void printTwins(uint64_t, uint64_t);
  void printTriplets(uint64_t, uint64_t);
  void printQuadruplets(uint64_t, uint64_t);
  void printQuintuplets(uint64_t, uint64_t);
  void printSextuplets(uint64_t, uint64_t);
  void printSeptuplets(uint64_t, uint64_t);
  // Count
  uint64_t countPrimes(uint64_t, uint64_t);
  uint64_t countTwins(uint64_t, uint64_t);
  uint64_t countTriplets(uint64_t, uint64_t);
  uint64_t countQuadruplets(uint64_t, uint64_t);
  uint64_t countQuintuplets(uint64_t, uint64_t);
  uint64_t countSextuplets(uint64_t, uint64_t);
  uint64_t countSeptuplets(uint64_t, uint64_t);
  // Count getters
  uint64_t getPrimeCount() const;
  uint64_t getTwinCount() const;
  uint64_t getTripletCount() const;
  uint64_t getQuadrupletCount() const;
  uint64_t getQuintupletCount() const;
  uint64_t getSextupletCount() const;
  uint64_t getSeptupletCount() const;
  uint64_t getCount(int) const;
  // Version
  static std::string getVersion();
  static int getMajorVersion();
  static int getMinorVersion();
protected:
  /// Sieve primes >= start_
  uint64_t start_;
  /// Sieve primes <= stop_
  uint64_t stop_;
  /// Prime number and prime k-tuplet counts
  std::vector<uint64_t> counts_;
  /// Time elapsed of sieve()
  double seconds_;
  uint64_t getInterval() const;
  void reset();
  virtual bool updateStatus(uint64_t, bool);
  virtual void setLock();
  virtual void unsetLock();
private:
  struct SmallPrime
  {
    uint32_t firstPrime;
    uint32_t lastPrime;
    int index;
    std::string str;
  };
  static const SmallPrime smallPrimes_[8];
  /// Sum of all processed segments
  uint64_t processed_;
  /// Sum of processed segments that hasn't been updated yet
  uint64_t toUpdate_;
  /// Status of sieve() in percent
  double percent_;
  /// Sieve size in kilobytes
  int sieveSize_;
  /// Flags (settings) for PrimeSieve e.g. COUNT_PRIMES, PRINT_TWINS, ...
  int flags_;
  /// ParallelPrimeSieve thread number
  int threadNum_;
  /// Pointer to the parent ParallelPrimeSieve object
  PrimeSieve* parent_;
  /// Callbacks for use with generatePrimes()
  void (*callback32_)(uint32_t);
  void (*callback64_)(uint64_t);
  void (*callback64_tn_)(uint64_t, int);
  PrimeSieveCallback<uint32_t>* psc32_;
  PrimeSieveCallback<uint64_t>* psc64_;
  PrimeSieveCallback<uint64_t, int>* psc64_tn_;
  static void printStatus(double, double);
  bool isFlag(int, int) const;
  bool isPublicFlags(int) const;
  bool isStatus() const;
  bool isParallelPrimeSieveChild() const;
  void doSmallPrime(const SmallPrime&);
  ////////////////////////////////////////////////////////////////////
  ///                      Private inner classes
  ////////////////////////////////////////////////////////////////////
  enum {
      INIT_STATUS = 0,
    FINISH_STATUS = 10
  };
  /// Private flags
  /// @pre flag >= (1 << 20)
  enum {
    CALLBACK32        = 1 << 20,
    CALLBACK64        = 1 << 21,
    CALLBACK64_TN     = 1 << 22,
    CALLBACK32_OBJ    = 1 << 23,
    CALLBACK64_OBJ    = 1 << 24,
    CALLBACK64_OBJ_TN = 1 << 25
  };
  template <typename T>
  class PushBackPrimes1 : public PrimeSieveCallback<uint64_t> {
  public:
    PushBackPrimes1(std::vector<T>& primes)
      : primes_(primes)
    { }
    /// Generate the primes in the interval [start, stop]
    /// store them in the primes vector.
    void generatePrimes(uint64_t start, uint64_t stop, PrimeSieve& ps)
    {
      primes_.reserve(primes_.size() + approxPrimeCount(start, stop));
      ps.generatePrimes(start, stop, this);
    }
    void callback(uint64_t prime)
    {
      primes_.push_back( static_cast<T>(prime) );
    }
  private:
    std::vector<T>& primes_;
    static uint64_t approxPrimeCount(uint64_t start, uint64_t stop)
    {
      double count = 0;

      if (start > stop)
        return 0;
      if (stop > 10)
        count += stop / (std::log(stop) - 1.1);
      if (start > 10)
        count -= (start / std::log(start) - 1.1);

      return static_cast<uint64_t>(count);      
    }
  };
  template <typename T>
  class PushBackPrimes2 : public PrimeSieveCallback<uint64_t> {
  public:
    PushBackPrimes2(std::vector<T>& primes)
      : primes_(primes)
    { }
    /// Generate the next n primes >= start and
    /// store them in the primes vector.
    void generate_N_Primes(uint64_t start, uint64_t n, PrimeSieve& ps)
    {
      n_ = n;
      primes_.reserve(primes_.size() + n_);
      try {
        while (n_ > 0)
        {
          uint64_t stop = start + n_ * 50 + 10000;
          ps.generatePrimes(start, stop, this);
          start = stop + 1;
        }
      } catch (stop_primesieve&) { }
    }
    void callback(uint64_t prime)
    {
      primes_.push_back( static_cast<T>(prime) );
      if (--n_ == 0)
        throw stop_primesieve();
    }
  private:
    std::vector<T>& primes_;
    uint64_t n_;
    class stop_primesieve : public std::exception { };
  };
};

#endif
