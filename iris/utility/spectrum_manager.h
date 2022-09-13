#ifndef _IRIS_UTILITY_SPECTRUM_MANAGER_
#define _IRIS_UTILITY_SPECTRUM_MANAGER_

#include <cassert>
#include <memory>
#include <vector>

#include "iris/spectrum.h"

namespace iris {
namespace utility {

class SpectrumManager {
 public:
  SpectrumManager() { spectra_.push_back(nullptr); }

  size_t Add(std::unique_ptr<Spectrum> spectrum) {
    assert(spectrum);
    size_t result = spectra_.size();
    spectra_.push_back(std::move(spectrum));
    return result;
  }

  const Spectrum* Get(size_t index) const { return spectra_.at(index).get(); }

 private:
  std::vector<std::unique_ptr<Spectrum>> spectra_;
};

}  // namespace utility
}  // namespace iris

#endif  // _IRIS_UTILITY_SPECTRUM_MANAGER_