#include "iris/scene.h"

namespace iris {

void Scene::Builder::Add(std::unique_ptr<Geometry> geometry,
                         const Matrix& matrix) {
  auto iter = numbered_matrices_.insert({matrix, numbered_matrices_.size()});
  size_t geometry_index = geometry_.size();
  geometry_.push_back(std::move(geometry));
  indices_.push_back({geometry_index, iter.first->second});
}

std::unique_ptr<Scene> Scene::Builder::Build() {
  std::map<size_t, Matrix> index_to_matrix;
  while (!numbered_matrices_.empty()) {
    auto first = *numbered_matrices_.begin();
    index_to_matrix.insert({first.second, first.first});
    numbered_matrices_.erase(numbered_matrices_.begin());
  }

  std::vector<Matrix> matrices;
  while (!index_to_matrix.empty()) {
    matrices.emplace_back(index_to_matrix.begin()->second);
    index_to_matrix.erase(index_to_matrix.begin());
  }

  indices_.shrink_to_fit();
  geometry_.shrink_to_fit();
  matrices.shrink_to_fit();

  auto result =
      Build(std::move(indices_), std::move(geometry_), std::move(matrices));

  indices_.clear();
  geometry_.clear();
  numbered_matrices_.clear();
  numbered_matrices_.insert({Matrix::Identity(), 0});

  return result;
}

Scene::const_iterator::value_type Scene::const_iterator::operator*() noexcept {
  return value_type(*geometry_, matrix_);
}

Scene::const_iterator& Scene::const_iterator::operator++() noexcept {
  if (auto next = generator_()) {
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