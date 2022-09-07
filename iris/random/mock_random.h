#ifndef _IRIS_RANDOM_MOCK_RANDOM_
#define _IRIS_RANDOM_MOCK_RANDOM_

#include <memory>

#include "googlemock/include/gmock/gmock.h"
#include "iris/random.h"

namespace iris {
namespace random {

class MockRandom : public Random {
 public:
  MOCK_METHOD(result_type, min, (), (const override));
  MOCK_METHOD(result_type, max, (), (const override));
  MOCK_METHOD(result_type, Invoke, (), ());
  MOCK_METHOD(void, discard, (unsigned long long z), (override));

  result_type operator()() override final { return Invoke(); }

  std::unique_ptr<Random> Replicate() override final {
    return std::make_unique<MockRandom>();
  }
};

}  // namespace random
}  // namespace iris

#endif  // _IRIS_RANDOM_MOCK_RANDOM_