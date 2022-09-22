load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "bxdf",
    hdrs = ["bxdf.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":random",
        ":reflector",
        ":spectral_allocator",
        ":vector",
    ],
)

cc_library(
    name = "bsdf",
    srcs = ["bsdf.cc"],
    hdrs = ["bsdf.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":bxdf",
        ":float",
        ":random",
        ":reflector",
        ":spectral_allocator",
        ":vector",
    ],
)

cc_library(
    name = "camera",
    hdrs = ["camera.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":ray",
    ],
)

cc_library(
    name = "color_matcher",
    hdrs = ["color_matcher.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":color",
        ":spectrum",
    ],
)

cc_library(
    name = "color",
    hdrs = ["color.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_test(
    name = "color_test",
    srcs = ["color_test.cc"],
    deps = [
        ":color",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "emissive_material",
    hdrs = ["emissive_material.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":spectrum",
        ":texture_coordinates",
    ],
)

cc_library(
    name = "float",
    hdrs = ["float.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "framebuffer",
    hdrs = ["framebuffer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":color",
        ":float",
    ],
)

cc_test(
    name = "framebuffer_test",
    srcs = ["framebuffer_test.cc"],
    deps = [
        ":framebuffer",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "geometry",
    srcs = ["geometry.cc"],
    hdrs = ["geometry.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":emissive_material",
        ":float",
        ":hit_allocator",
        ":integer",
        ":material",
        ":normal_map",
        ":point",
        ":random",
        ":ray",
        ":texture_coordinates",
        ":vector",
        "//iris/internal:hit_arena",
    ],
)

cc_test(
    name = "geometry_test",
    srcs = ["geometry_test.cc"],
    deps = [
        ":geometry",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "image_sampler",
    hdrs = ["image_sampler.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":random",
    ],
)

cc_library(
    name = "hit",
    hdrs = ["hit.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_library(
    name = "hit_allocator",
    srcs = ["hit_allocator.cc"],
    hdrs = ["hit_allocator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":hit",
        ":integer",
        ":ray",
        "//iris/internal:hit_arena",
    ],
)

cc_test(
    name = "hit_allocator_test",
    srcs = ["hit_allocator_test.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":hit_allocator",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "integer",
    hdrs = ["integer.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "integrator",
    hdrs = ["integrator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":light_sampler",
        ":random",
        ":ray",
        ":ray_tracer",
        ":spectral_allocator",
        ":spectrum",
        ":visibility_tester",
    ],
)

cc_library(
    name = "intersector",
    srcs = ["intersector.cc"],
    hdrs = ["intersector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":geometry",
        ":hit_allocator",
        ":matrix",
        "//iris/internal:hit",
    ],
)

cc_test(
    name = "intersector_test",
    srcs = ["intersector_test.cc"],
    deps = [
        ":intersector",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "light",
    hdrs = ["light.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":point",
        ":random",
        ":ray",
        ":spectral_allocator",
        ":spectrum",
        ":vector",
        ":visibility_tester",
    ],
)

cc_library(
    name = "light_sample_allocator",
    srcs = ["light_sample_allocator.cc"],
    hdrs = ["light_sample_allocator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":light",
        ":light_sample",
        "//iris/internal:arena",
    ],
)

cc_test(
    name = "light_sample_allocator_test",
    srcs = ["light_sample_allocator_test.cc"],
    deps = [
        ":light_sample_allocator",
        "//iris/internal:arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "light_sample",
    hdrs = ["light_sample.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":light",
    ],
)

cc_library(
    name = "light_sampler",
    srcs = ["light_sampler.cc"],
    hdrs = ["light_sampler.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":light_sample",
        ":light_sample_allocator",
        ":light_scene",
        ":random",
    ],
)

cc_test(
    name = "light_sampler_test",
    srcs = ["light_sampler_test.cc"],
    deps = [
        ":light_sampler",
        "//iris/internal:arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "light_scene",
    srcs = ["light_scene.cc"],
    hdrs = ["light_scene.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":light",
        ":light_sample",
        ":light_sample_allocator",
        ":point",
        ":random",
        "//iris/internal:visibility_tester",
    ],
)

cc_test(
    name = "light_scene_test",
    srcs = ["light_scene_test.cc"],
    deps = [
        ":light_scene",
        "//iris/emissive_materials:mock_emissive_material",
        "//iris/lights:mock_light",
        "//iris/random:mock_random",
        "//iris/scenes:list_scene",
        "//iris/spectra:mock_spectrum",
        "//iris/testing:light_tester",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "material",
    hdrs = ["material.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":bxdf",
        ":texture_coordinates",
    ],
)

cc_library(
    name = "matrix",
    srcs = ["matrix.cc"],
    hdrs = ["matrix.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":point",
        ":ray",
        ":vector",
        "@com_google_absl//absl/status:statusor",
    ],
)

cc_test(
    name = "matrix_test",
    srcs = ["matrix_test.cc"],
    deps = [
        ":matrix",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "normal_map",
    hdrs = ["normal_map.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":texture_coordinates",
        ":vector",
    ],
)

cc_library(
    name = "point",
    hdrs = ["point.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":vector",
    ],
)

cc_test(
    name = "point_test",
    srcs = ["point_test.cc"],
    deps = [
        ":point",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "random",
    hdrs = ["random.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "ray",
    hdrs = ["ray.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":point",
        ":vector",
    ],
)

cc_test(
    name = "ray_test",
    srcs = ["ray_test.cc"],
    deps = [
        ":ray",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "ray_tracer",
    srcs = ["ray_tracer.cc"],
    hdrs = ["ray_tracer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":bsdf",
        ":float",
        ":ray",
        ":scene",
        ":spectrum",
        "//iris/internal:ray_tracer",
    ],
)

cc_test(
    name = "ray_tracer_test",
    srcs = ["ray_tracer_test.cc"],
    deps = [
        ":ray_tracer",
        "//iris/bxdfs:mock_bxdf",
        "//iris/internal:ray_tracer",
        "//iris/scenes:list_scene",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "reflector",
    hdrs = ["reflector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_library(
    name = "renderer",
    srcs = ["renderer.cc"],
    hdrs = ["renderer.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":camera",
        ":color",
        ":color_matcher",
        ":float",
        ":framebuffer",
        ":image_sampler",
        ":integrator",
        ":light_sample_allocator",
        ":light_sampler",
        ":light_scene",
        ":ray_tracer",
        ":scene",
        ":spectral_allocator",
        "//iris/internal:arena",
        "//iris/internal:ray_tracer",
        "//iris/internal:visibility_tester",
    ],
)

cc_test(
    name = "renderer_test",
    srcs = ["renderer_test.cc"],
    deps = [
        ":renderer",
        "//iris/cameras:mock_camera",
        "//iris/color_matchers:mock_color_matcher",
        "//iris/image_samplers:mock_image_sampler",
        "//iris/integrators:mock_integrator",
        "//iris/light_scenes:all_light_scene",
        "//iris/random:mock_random",
        "//iris/scenes:list_scene",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "scene",
    srcs = ["scene.cc"],
    hdrs = ["scene.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":intersector",
        ":ray",
    ],
)

cc_test(
    name = "scene_test",
    srcs = ["scene_test.cc"],
    deps = [
        ":scene",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "spectrum",
    hdrs = ["spectrum.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_library(
    name = "spectral_allocator",
    srcs = ["spectral_allocator.cc"],
    hdrs = ["spectral_allocator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":reflector",
        ":spectrum",
        "//iris/internal:arena",
    ],
)

cc_test(
    name = "spectral_allocator_test",
    srcs = ["spectral_allocator_test.cc"],
    deps = [
        ":spectral_allocator",
        "//iris/internal:arena",
        "//iris/reflectors:mock_reflector",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "texture_coordinates",
    hdrs = ["texture_coordinates.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":vector",
    ],
)

cc_library(
    name = "vector",
    hdrs = ["vector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_test(
    name = "vector_test",
    srcs = ["vector_test.cc"],
    deps = [
        ":vector",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "visibility_tester",
    srcs = ["visibility_tester.cc"],
    hdrs = ["visibility_tester.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":ray",
        ":scene",
        "//iris/internal:ray_tracer",
    ],
)

cc_test(
    name = "visibility_tester_test",
    srcs = ["visibility_tester_test.cc"],
    deps = [
        ":visibility_tester",
        "//iris/internal:ray_tracer",
        "//iris/scenes:list_scene",
        "@com_google_googletest//:gtest_main",
    ],
)
