#ifndef _IRIS_REFERENCE_COUNTABLE_
#define _IRIS_REFERENCE_COUNTABLE_

#include <atomic>
#include <cstdint>

namespace iris {

class ReferenceCountable {
 public:
  ReferenceCountable() noexcept : reference_count_(1) {}

 private:
  typedef uint32_t reference_count_t;

  void Increment() noexcept;
  bool Decrement() noexcept;

  std::atomic<reference_count_t> reference_count_;

  template <typename T>
  requires std::derived_from<T, ReferenceCountable>
  friend class ReferenceCounted;
};

}  // namespace iris

#endif  // _IRIS_MEMORY_