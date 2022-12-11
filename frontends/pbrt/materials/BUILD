load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "matte",
    srcs = ["matte.cc"],
    hdrs = ["matte.h"],
    deps = [
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:texture_manager",
        "//iris:material",
        "//iris:reference_counted",
        "//iris/materials:matte_material",
        "//iris/reflectors:uniform_reflector",
        "//iris/textures:constant_texture",
    ],
)

cc_test(
    name = "matte_test",
    srcs = ["matte_test.cc"],
    deps = [
        ":matte",
        "//frontends/pbrt:build_objects",
        "//frontends/pbrt/spectrum_managers:test_spectrum_manager",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "parse",
    srcs = ["parse.cc"],
    hdrs = ["parse.h"],
    visibility = ["//frontends/pbrt:__subpackages__"],
    deps = [
        ":matte",
        "//frontends/pbrt:material_manager",
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:quoted_string",
        "//frontends/pbrt:texture_manager",
        "//iris:material",
        "//iris:reference_counted",
    ],
)

cc_test(
    name = "parse_test",
    srcs = ["parse_test.cc"],
    deps = [
        ":parse",
        "//frontends/pbrt/spectrum_managers:test_spectrum_manager",
        "@com_google_googletest//:gtest_main",
    ],
)
