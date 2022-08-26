#ifndef _IRIS_RANDOM_
#define _IRIS_RANDOM_

#include <cstdint>
#include <memory>

namespace iris {

class Random {
 public:
  typedef uint_fast32_t result_type;
  virtual result_type min() const = 0;
  virtual result_type max() const = 0;
  virtual result_type operator()() = 0;
  virtual void discard(unsigned long long z);

  virtual std::unique_ptr<Random> Replicate();

  virtual ~Random() {}
};

}  // namespace iris

#endif  // _IRIS_RANDOM_