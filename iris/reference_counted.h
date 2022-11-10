#ifndef _IRIS_REFERENCE_COUNTED_
#define _IRIS_REFERENCE_COUNTED_

#include <algorithm>
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>

#include "iris/reference_countable.h"

namespace iris {

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

  template <class U>
  requires std::derived_from<U, T> ReferenceCounted(
      const ReferenceCounted<U>& value)
  noexcept : ptr_(value.ptr_) { ptr_->Increment(); }

  template <class U>
  requires std::derived_from<U, T> ReferenceCounted(ReferenceCounted<U>&& value)
  noexcept : ptr_(value.ptr_) { value.ptr_ = nullptr; }

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

  void Reset() {
    MaybeDecrement();
    ptr_ = nullptr;
  }

 private:
  void MaybeDecrement() {
    if (ptr_ && ptr_->Decrement()) {
      delete ptr_;
    }
  }

  T* ptr_;

  template <class U>
  requires std::derived_from<U, ReferenceCountable>
  friend class ReferenceCounted;
};

template <typename T, typename... Args>
ReferenceCounted<T> MakeReferenceCounted(Args... args) {
  return ReferenceCounted<T>(std::make_unique<T>(args...));
}

}  // namespace iris

namespace std {

template <typename T>
void swap(iris::ReferenceCounted<T>& lhs,
          iris::ReferenceCounted<T>& rhs) noexcept {
  lhs.Swap(rhs);
}

template <typename T>
struct hash<iris::ReferenceCounted<T>> {
  std::size_t operator()(
      const iris::ReferenceCounted<T>& value) const noexcept {
    std::hash<const T*> hash;
    return hash(value.Get());
  }
};

}  // namespace std

#endif  // _IRIS_REFERENCE_COUNTED_