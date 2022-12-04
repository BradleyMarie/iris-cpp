#include "frontends/pbrt/cameras/orthographic.h"

#include "iris/cameras/orthographic_camera.h"

namespace iris::pbrt_frontend::cameras {
namespace {

static const std::unordered_map<std::string_view, Parameter::Type>
    g_parameters = {
        {"focaldistance", Parameter::FLOAT},
        {"frameaspectratio", Parameter::FLOAT},
        {"lensradius", Parameter::FLOAT},
        {"screenwindow", Parameter::FLOAT},
};

class OrthographicObjectBuilder
    : public ObjectBuilder<std::function<std::unique_ptr<Camera>(
                               const std::pair<size_t, size_t>&)>,
                           const MatrixManager::Transformation&> {
 public:
  OrthographicObjectBuilder() : ObjectBuilder(g_parameters) {}

  std::function<std::unique_ptr<Camera>(const std::pair<size_t, size_t>&)>
  Build(const std::unordered_map<std::string_view, Parameter>& parameters,
        const MatrixManager::Transformation& transformation) const override;
};

std::function<std::unique_ptr<Camera>(const std::pair<size_t, size_t>&)>
OrthographicObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters,
    const MatrixManager::Transformation& transformation) const {
  return [transformation](const std::pair<size_t, size_t>& image_dimensions) {
    return std::make_unique<iris::cameras::OrthographicCamera>(
        transformation.start);
  };
}

}  // namespace

const std::unique_ptr<const ObjectBuilder<
    std::function<std::unique_ptr<Camera>(const std::pair<size_t, size_t>&)>,
    const MatrixManager::Transformation&>>
    g_orthographic_builder = std::make_unique<OrthographicObjectBuilder>();

}  // namespace iris::pbrt_frontend::cameras