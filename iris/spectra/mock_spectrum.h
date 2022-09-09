#ifndef _IRIS_SPECTRA_MOCK_SPECTRUM_
#define _IRIS_SPECTRA_MOCK_SPECTRUM_

#include "googlemock/include/gmock/gmock.h"
#include "iris/float.h"
#include "iris/spectrum.h"

namespace iris {
namespace spectra {

class MockSpectrum : public Spectrum {
 public:
  MOCK_METHOD(visual_t, Intensity, (visual_t), (const));
};

}  // namespace spectra
}  // namespace iris

#endif  // _IRIS_SPECTRA_MOCK_SPECTRUM_