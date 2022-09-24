#ifndef _IRIS_RANDOM_
#define _IRIS_RANDOM_

#include <cstdint>
#include <limits>
#include <memory>

namespace iris {

class Random {
 public:
  typedef uint_fast32_t result_type;

  static constexpr result_type min() {
    return std::numeric_limits<uint32_t>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<uint32_t>::max();
  }

  virtual result_type operator()() = 0;
  virtual void discard(unsigned long long z) = 0;

  virtual std::unique_ptr<Random> Replicate() = 0;

  virtual ~Random() {}
};

}  // namespace iris

#endif  // _IRIS_RANDOM_