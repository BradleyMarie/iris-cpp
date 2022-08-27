#include "iris/scene.h"

namespace iris {

Scene::const_iterator::value_type Scene::const_iterator::operator*() noexcept {
  return value_type(*geometry_, matrix_);
}

Scene::const_iterator& Scene::const_iterator::operator++() noexcept {
  auto next = generator_();
  if (next) {
    geometry_ = &next->first;
    matrix_ = next->second;
  } else {
    geometry_ = nullptr;
    matrix_ = nullptr;
  }
  return *this;
}

Scene::const_iterator Scene::const_iterator::operator++(int) {
  auto result = *this;
  ++(*this);
  return result;
}

bool Scene::const_iterator::operator==(
    const const_iterator& iter) const noexcept {
  return geometry_ == iter.geometry_ && matrix_ == iter.matrix_;
}

bool Scene::const_iterator::operator!=(
    const const_iterator& iter) const noexcept {
  return geometry_ != iter.geometry_ || matrix_ != iter.matrix_;
}

Scene::const_iterator Scene::end() const {
  return Scene::const_iterator([]() { return std::nullopt; });
}

}  // namespace iris