#ifndef _IRIS_BXDFS_COMPOSITE_BXDF_
#define _IRIS_BXDFS_COMPOSITE_BXDF_

#include <array>
#include <cassert>
#include <random>

#include "iris/bxdf.h"
#include "iris/float.h"
#include "iris/random.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {
namespace bxdfs {

template <typename... Bxdfs>
class CompositeBxdf final : public Bxdf {
 public:
  CompositeBxdf(Bxdfs&&... bxdfs) {
    size_t i = 0;
    (void(bxdfs_[i++] = &bxdfs), ...);
    assert(std::all_of(bxdfs_.begin(), bxdfs_.end(),
                       [](const Bxdf* bxdf) { return bxdf != nullptr; }));
  }

  Vector Sample(const Vector& incoming, Random& rng) const override {
    size_t index = distribution_(rng);
    return bxdfs_[index]->Sample(incoming, rng);
  }

  std::optional<visual_t> Pdf(const Vector& incoming,
                              const Vector& outgoing) const override {
    visual_t specular_pdf = 0.0;

    std::optional<visual_t> result;
    for (const auto* bxdf : bxdfs_) {
      auto pdf = bxdf->Pdf(incoming, outgoing);
      if (!pdf.has_value()) {
        specular_pdf += 1.0;
      } else {
        result = result.value_or(0.0) + *pdf;
      }
    }

    if (result) {
      *result += specular_pdf;
      *result /= static_cast<visual_t>(sizeof...(Bxdfs));
    }

    return result;
  }

  const Reflector* Reflectance(const Vector& incoming, const Vector& outgoing,
                               Type type,
                               SpectralAllocator& allocator) const override {
    const Reflector* result = nullptr;
    for (const auto* bxdf : bxdfs_) {
      auto reflectance = bxdf->Reflectance(incoming, outgoing, type, allocator);
      result = allocator.Add(result, reflectance);
    }
    return result;
  }

 private:
  static std::uniform_int_distribution<uint_fast32_t> distribution_;
  std::array<const Bxdf*, sizeof...(Bxdfs)> bxdfs_;

  static_assert(sizeof...(Bxdfs) != 0);
};

template <typename... Bxdfs>
std::uniform_int_distribution<uint_fast32_t>
    CompositeBxdf<Bxdfs...>::distribution_(0, sizeof...(Bxdfs));

template <typename... Bxdfs>
CompositeBxdf<Bxdfs...> MakeComposite(Bxdfs&&... bxdfs) {
  return CompositeBxdf<Bxdfs...>(bxdfs...);
}

}  // namespace bxdfs
}  // namespace iris

#endif  // _IRIS_BXDFS_COMPOSITE_BXDF_