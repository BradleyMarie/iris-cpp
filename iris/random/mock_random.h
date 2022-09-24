#ifndef _IRIS_RANDOM_MOCK_RANDOM_
#define _IRIS_RANDOM_MOCK_RANDOM_

#include <memory>

#include "googlemock/include/gmock/gmock.h"
#include "iris/random.h"

namespace iris {
namespace random {

class MockRandom final : public Random {
 public:
  MOCK_METHOD(result_type, Invoke, (), ());
  MOCK_METHOD(void, discard, (unsigned long long z), (override));
  MOCK_METHOD(std::unique_ptr<Random>, Replicate, (), (override));

  result_type operator()() override final { return Invoke(); }
};

}  // namespace random
}  // namespace iris

#endif  // _IRIS_RANDOM_MOCK_RANDOM_