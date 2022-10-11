#ifndef _IRIS_UTILITY_MEMORY_
#define _IRIS_UTILITY_MEMORY_

#include <algorithm>
#include <atomic>
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>

namespace iris {
namespace utility {

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

template <typename T>
requires std::derived_from<T, ReferenceCountable>
class ReferenceCounted final {
 public:
  ReferenceCounted(std::unique_ptr<T> value) noexcept : ptr_(value.release()) {}

  ReferenceCounted() noexcept : ptr_(nullptr) {}

  ReferenceCounted(const ReferenceCounted& value) noexcept : ptr_(value.ptr_) {
    ptr_->Increment();
  }

  ReferenceCounted(ReferenceCounted&& value) noexcept : ptr_(value.ptr_) {
    value.ptr_ = nullptr;
  }

  ReferenceCounted& operator=(const ReferenceCounted& value) noexcept {
    MaybeDecrement();
    ptr_ = value.ptr_;
    ptr_->Increment();
    return *this;
  }

  ReferenceCounted& operator=(ReferenceCounted&& value) noexcept {
    MaybeDecrement();
    ptr_ = value.ptr_;
    value.ptr_ = nullptr;
    return *this;
  }

  ~ReferenceCounted() noexcept { MaybeDecrement(); }

  auto operator<=>(const ReferenceCounted& rhs) const = default;
  explicit operator bool() const noexcept { return ptr_; }
  const T* operator->() const noexcept { return ptr_; }
  const T& operator*() const noexcept { return *ptr_; }

  void Swap(ReferenceCounted& other) noexcept { std::swap(ptr_, other.ptr_); }
  const T* Get() const noexcept { return ptr_; }

 private:
  void MaybeDecrement() {
    if (ptr_ && ptr_->Decrement()) {
      delete ptr_;
    }
  }

  T* ptr_;
};

template <typename T, typename... Args>
ReferenceCounted<T> MakeReferenceCounted(Args... args) {
  return ReferenceCounted<T>(std::make_unique<T>(args...));
}

}  // namespace utility
}  // namespace iris

namespace std {

template <typename T>
void swap(iris::utility::ReferenceCounted<T>& lhs,
          iris::utility::ReferenceCounted<T>& rhs) noexcept {
  lhs.Swap(rhs);
}

template <typename T>
struct hash<iris::utility::ReferenceCounted<T>> {
  std::size_t operator()(
      const iris::utility::ReferenceCounted<T>& value) const noexcept {
    std::hash<const T*> hash;
    return hash(value.Get());
  }
};

}  // namespace std

#endif  // _IRIS_UTILITY_MEMORY_