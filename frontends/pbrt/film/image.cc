#include "frontends/pbrt/film/image.h"

#include <limits>

#include "iris/file/pfm_writer.h"

namespace iris::pbrt_frontend::film {
namespace {

static const std::unordered_map<std::string_view, Parameter::Type>
    g_parameters = {
        {"cropwindow", Parameter::FLOAT},
        {"diagonal", Parameter::FLOAT},
        {"filename", Parameter::STRING},
        {"maxsampleluminance", Parameter::FLOAT},
        {"scale", Parameter::FLOAT},
        {"x_resolution", Parameter::INTEGER},
        {"y_resolution", Parameter::INTEGER},
};

class ImageObjectBuilder : public ObjectBuilder<Result> {
 public:
  ImageObjectBuilder() : ObjectBuilder(g_parameters) {}

  Result Build(const std::unordered_map<std::string_view, Parameter>&
                   parameters) const override;
};

Result ImageObjectBuilder::Build(
    const std::unordered_map<std::string_view, Parameter>& parameters) const {
  std::array<geometric_t, 4> crop_window = {0.0, 1.0, 0.0, 1.0};
  geometric_t diagonal = 35.0;
  std::filesystem::path filename = "pbrt.pfm";  // TODO: Set to exr
  visual_t scale = 1.0;
  std::optional<visual_t> max_sample_luminance = std::nullopt;
  size_t x_resolution = 640;
  size_t y_resolution = 480;

  auto cropwindow = parameters.find("cropwindow");
  if (cropwindow != parameters.end()) {
    auto values = cropwindow->second.GetFloatValues(4, 4);
    for (size_t i = 0; i < 4; i++) {
      if (values[i] < 0.0 || values[i] > 1.0) {
        std::cerr << "ERROR: Out of range value for parameter: cropwindow"
                  << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    if (values[0] >= values[1] || values[2] >= values[3]) {
      std::cerr << "ERROR: Invalid values for parameter list: cropwindow"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    crop_window[0] = values[0];
    crop_window[1] = values[1];
    crop_window[2] = values[2];
    crop_window[3] = values[3];
  }

  auto diagonal_iter = parameters.find("diagonal");
  if (diagonal_iter != parameters.end()) {
    auto value = diagonal_iter->second.GetFloatValues(1).front();
    if (value <= 0.0) {
      std::cerr << "ERROR: Out of range value for parameter: diagonal"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    diagonal = value;
  }

  auto filename_iter = parameters.find("filename");
  if (filename_iter != parameters.end()) {
    filename = filename_iter->second.GetStringValues(1).front();
  }

  auto maxsampleluminance = parameters.find("maxsampleluminance");
  if (maxsampleluminance != parameters.end()) {
    auto value = maxsampleluminance->second.GetFloatValues(1).front();
    if (value <= 0.0) {
      std::cerr << "ERROR: Out of range value for parameter: maxsampleluminance"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    max_sample_luminance = value;
  }

  auto scale_iter = parameters.find("scale");
  if (scale_iter != parameters.end()) {
    auto value = scale_iter->second.GetFloatValues(1).front();
    if (value <= 0.0) {
      std::cerr << "ERROR: Out of range value for parameter: scale"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    scale = value;
  }

  auto x_resolution_iter = parameters.find("x_resolution");
  if (x_resolution_iter != parameters.end()) {
    auto value = x_resolution_iter->second.GetIntegerValues(1).front();
    if (value <= 0) {
      std::cerr << "ERROR: Out of range value for parameter: x_resolution"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    x_resolution = value;
  }

  auto y_resolution_iter = parameters.find("y_resolution");
  if (y_resolution_iter != parameters.end()) {
    auto value = y_resolution_iter->second.GetIntegerValues(1).front();
    if (value <= 0) {
      std::cerr << "ERROR: Out of range value for parameter: y_resolution"
                << std::endl;
      exit(EXIT_FAILURE);
    }

    y_resolution = value;
  }

  std::function<void(Framebuffer&, std::ofstream&)> write_to_file_function;
  if (filename.extension() == ".pfm") {
    write_to_file_function = [](Framebuffer& filename, std::ofstream& output) {
      file::WritePfm(filename, iris::Color::LINEAR_SRGB, output);
    };
  } else {
    std::cerr << "ERROR: Unsupported file extension for parameter: filename"
              << std::endl;
    exit(EXIT_FAILURE);
  }

  std::function<void(Framebuffer&, std::ofstream&)> write_function =
      [scale, write_to_file_function](Framebuffer& framebuffer,
                                      std::ofstream& output) {
        auto size = framebuffer.Size();

        if (scale != 1.0) {
          for (size_t y = 0; y < size.first; y++) {
            for (size_t x = 0; x < size.second; x++) {
              auto color =
                  framebuffer.Get(y, x).ConvertTo(iris::Color::CIE_XYZ);
              framebuffer.Set(y, x,
                              iris::Color(color.x, color.y * scale, color.z,
                                          iris::Color::CIE_XYZ));
            }
          }
        }

        write_to_file_function(framebuffer, output);
      };

  return Result{filename,       std::make_pair(y_resolution, x_resolution),
                write_function, crop_window,
                diagonal,       max_sample_luminance};
}

}  // namespace

const std::unique_ptr<const ObjectBuilder<Result>> g_image_builder(
    std::make_unique<ImageObjectBuilder>());

}  // namespace iris::pbrt_frontend::film