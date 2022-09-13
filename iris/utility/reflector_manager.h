#ifndef _IRIS_UTILITY_REFLECTOR_MANAGER_
#define _IRIS_UTILITY_REFLECTOR_MANAGER_

#include <cassert>
#include <memory>
#include <vector>

#include "iris/reflector.h"

namespace iris {
namespace utility {

class ReflectorManager {
 public:
  ReflectorManager() { reflectors_.push_back(nullptr); }

  size_t Add(std::unique_ptr<Reflector> reflector) {
    assert(reflector);
    size_t result = reflectors_.size();
    reflectors_.push_back(std::move(reflector));
    return result;
  }

  const Reflector* Get(size_t index) const {
    return reflectors_.at(index).get();
  }

 private:
  std::vector<std::unique_ptr<Reflector>> reflectors_;
};

}  // namespace utility
}  // namespace iris

#endif  // _IRIS_UTILITY_REFLECTOR_MANAGER_