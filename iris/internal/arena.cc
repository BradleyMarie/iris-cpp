#include "iris/internal/arena.h"

namespace iris::internal {

void* Arena::Allocate(size_t size) {
  if (size == 0) {
    return nullptr;
  }

  if (data_index_ == data_.size()) {
    data_.emplace_back();
  }

  if (data_[data_index_].capacity() < size) {
    data_[data_index_].reserve(size);
  }

  return static_cast<void*>(data_[data_index_++].data());
}

void Arena::Clear() { data_index_ = 0; }

}  // namespace iris::internal