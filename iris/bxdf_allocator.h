#ifndef _IRIS_BXDF_ALLOCATOR_
#define _IRIS_BXDF_ALLOCATOR_

#include <concepts>
#include <type_traits>

#include "iris/bxdf.h"

namespace iris {
namespace internal {
class Arena;
}  // namespace internal

class BxdfAllocator {
 public:
  BxdfAllocator(internal::Arena& arena) noexcept : arena_(arena) {}

  template <typename T, typename... Args>
  requires(
      std::is_trivially_destructible<T>::value&& std::derived_from<T, Bxdf>)
      const Bxdf& Allocate(Args&&... args) {
    auto* result = new (Allocate(sizeof(T))) T(std::forward<Args>(args)...);
    return *result;
  }

 private:
  BxdfAllocator(const BxdfAllocator&) = delete;
  BxdfAllocator& operator=(const BxdfAllocator&) = delete;

  void* Allocate(size_t size);

  internal::Arena& arena_;
};

}  // namespace iris

#endif  // _IRIS_BXDF_ALLOCATOR_