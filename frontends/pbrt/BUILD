load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//frontends/pbrt:__subpackages__"])

cc_library(
    name = "build_objects",
    hdrs = ["build_objects.h"],
    deps = [
        ":object_builder",
        ":parameter_list",
        ":spectrum_manager",
        ":texture_manager",
        ":tokenizer",
    ],
)

cc_test(
    name = "build_objects_test",
    srcs = ["build_objects_test.cc"],
    deps = [
        ":build_objects",
        "//frontends/pbrt/spectrum_managers:test_spectrum_manager",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "color",
    hdrs = ["color.h"],
    deps = [
        "//iris:float",
    ],
)

cc_library(
    name = "material_manager",
    srcs = ["material_manager.cc"],
    hdrs = ["material_manager.h"],
    deps = [
        ":object_builder",
        ":texture_manager",
        "//iris:material",
        "//iris:reference_counted",
    ],
)

cc_test(
    name = "material_manager_test",
    srcs = ["material_manager_test.cc"],
    deps = [
        ":material_manager",
        "//iris/materials:mock_material",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "matrix_manager",
    srcs = ["matrix_manager.cc"],
    hdrs = ["matrix_manager.h"],
    deps = [
        ":quoted_string",
        ":tokenizer",
        "//iris:matrix",
    ],
)

cc_test(
    name = "matrix_manager_test",
    srcs = ["matrix_manager_test.cc"],
    deps = [
        ":matrix_manager",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "object_builder",
    hdrs = ["object_builder.h"],
    deps = [
        ":parameter",
        ":parameter_list",
    ],
)

cc_test(
    name = "object_builder_test",
    srcs = ["object_builder_test.cc"],
    deps = [
        ":object_builder",
        "//frontends/pbrt/spectrum_managers:test_spectrum_manager",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "parameter_list",
    srcs = ["parameter_list.cc"],
    hdrs = ["parameter_list.h"],
    deps = [
        ":color",
        ":quoted_string",
        ":tokenizer",
        "//iris:point",
        "//iris:vector",
    ],
)

cc_test(
    name = "parameter_list_test",
    srcs = ["parameter_list_test.cc"],
    deps = [
        ":parameter_list",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "parameter",
    srcs = ["parameter.cc"],
    hdrs = ["parameter.h"],
    deps = [
        ":color",
        ":parameter_list",
        ":spectrum_manager",
        ":texture_manager",
        "//iris:reference_counted",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris/textures:texture2d",
    ],
)

cc_test(
    name = "parameter_test",
    srcs = ["parameter_test.cc"],
    deps = [
        ":parameter",
        "//frontends/pbrt/spectrum_managers:test_spectrum_manager",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        ":build_objects",
        ":matrix_manager",
        ":quoted_string",
        ":renderable",
        ":spectrum_manager",
        ":texture_manager",
        ":tokenizer",
        "//frontends/pbrt/cameras:parse",
        "//frontends/pbrt/integrators:parse",
        "//frontends/pbrt/materials:parse",
        "//frontends/pbrt/samplers:parse",
        "//iris:image_sampler",
        "//iris:integrator",
        "//iris:light_scene",
    ],
)

cc_test(
    name = "parser_test",
    srcs = ["parser_test.cc"],
    data = [
        "test_data/include_circular_first.pbrt",
        "test_data/include_circular_second.pbrt",
        "test_data/include_circular_self.pbrt",
        "test_data/include_circular_third.pbrt",
        "test_data/include_empty.pbrt",
    ],
    deps = [
        ":parser",
        "//frontends/pbrt/spectrum_managers:test_spectrum_manager",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "renderable",
    srcs = ["renderable.cc"],
    hdrs = ["renderable.h"],
    deps = [
        "//iris:camera",
        "//iris:color_matcher",
        "//iris:framebuffer",
        "//iris:image_sampler",
        "//iris:integrator",
        "//iris:light_scene",
        "//iris:random",
        "//iris:renderer",
        "//iris:scene",
        "//iris:scene_objects",
    ],
)

cc_test(
    name = "renderable_test",
    srcs = ["renderable_test.cc"],
    deps = [
        ":renderable",
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
    name = "spectrum_manager",
    hdrs = ["spectrum_manager.h"],
    deps = [
        ":color",
        "//iris:reference_counted",
        "//iris:reflector",
        "//iris:spectrum",
    ],
)

cc_library(
    name = "texture_manager",
    srcs = ["texture_manager.cc"],
    hdrs = ["texture_manager.h"],
    deps = [
        "//iris:reference_counted",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris/reflectors:uniform_reflector",
        "//iris/textures:constant_texture",
        "//iris/textures:texture2d",
    ],
)

cc_test(
    name = "texture_manager_test",
    srcs = ["texture_manager_test.cc"],
    deps = [
        ":texture_manager",
        "//iris/reflectors:mock_reflector",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "quoted_string",
    srcs = ["quoted_string.cc"],
    hdrs = ["quoted_string.h"],
)

cc_test(
    name = "quoted_string_test",
    srcs = ["quoted_string_test.cc"],
    deps = [
        ":quoted_string",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "tokenizer",
    srcs = ["tokenizer.cc"],
    hdrs = ["tokenizer.h"],
)

cc_test(
    name = "tokenizer_test",
    srcs = ["tokenizer_test.cc"],
    deps = [
        ":tokenizer",
        "@com_google_googletest//:gtest_main",
    ],
)
