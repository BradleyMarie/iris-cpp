#include "iris/internal/hit_arena.h"

#include <cstring>

namespace iris::internal {

Hit& HitArena::Allocate(iris::Hit* next, geometric_t distance, face_t front,
                        face_t back, const void* additional_data,
                        size_t additional_data_size) {
  if (hit_index_ == hits_.size()) {
    hits_.emplace_back(std::make_unique<Hit>());
  }

  void* allocated_additional_data = nullptr;
  if (additional_data_size != 0) {
    if (additional_data_index_ == additional_data_.size()) {
      additional_data_.emplace_back();
    }
    if (additional_data_[additional_data_index_].capacity() <
        additional_data_size) {
      additional_data_[additional_data_index_].reserve(additional_data_size);
    }
    allocated_additional_data =
        static_cast<void*>(additional_data_[additional_data_index_].data());
    memcpy(allocated_additional_data, additional_data, additional_data_size);
  }

  auto* result = new (hits_[hit_index_++].get())
      Hit(next, distance, front, back, allocated_additional_data);

  return *result;
}

void HitArena::Clear() {
  hit_index_ = 0;
  additional_data_index_ = 0;
}

}  // namespace iris::internal