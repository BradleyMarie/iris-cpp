#ifndef _IRIS_BXDFS_COMPOSITE_BXDF_
#define _IRIS_BXDFS_COMPOSITE_BXDF_

#include <array>
#include <random>

#include "iris/bxdf.h"
#include "iris/float.h"
#include "iris/random.h"
#include "iris/reflector.h"
#include "iris/spectral_allocator.h"
#include "iris/vector.h"

namespace iris {
namespace bxdfs {

template <const Bxdf&... Bxdfs>
class CompositeBxdf final : public Bxdf {
 public:
  CompositeBxdf(Bxdfs... args) : bxdfs_{&args, ...} {}

  SampleResult Sample(const Vector& incoming, Random& rng,
                      SpectralAllocator& allocator) const override {
    size_t index = distribution_(rng);
    return bxdfs_[index]->Sample(incoming, rng, allocator);
  }

  std::optional<visual_t> Pdf(const Vector& incoming,
                              const Vector& outgoing) const override {
    auto result = bxdfs_[0]->Pdf(incoming, outgoing);
    if (bxdfs_.size() == 1) {
      return result;
    }

    visual_t pdf = result.value_or(1.0);
    for (size_t i = 0; i < bxdfs_.size(); i++) {
      pdf += bxdfs_[0]->Pdf(incoming, outgoing).value_or(1.0);
    }

    return pdf / static_cast<visual_t>(sizeof...(Bxdfs));
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
  static uniform_int_distribution<size_t> distribution_;
  std::array<const Bxdf*, sizeof...(Bxdfs)> bxdfs_;

  static_assert(sizeof...(Bxdfs) != 0);
};

}  // namespace bxdfs
}  // namespace iris

#endif  // _IRIS_BXDFS_COMPOSITE_BXDF_