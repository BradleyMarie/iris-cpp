load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "diffuse",
    srcs = ["diffuse.cc"],
    hdrs = ["diffuse.h"],
    deps = [
        "//frontends/pbrt:object_builder",
        "//iris:reference_counted",
        "//iris/emissive_materials:constant_emissive_material",
    ],
)

cc_test(
    name = "diffuse_test",
    srcs = ["diffuse_test.cc"],
    deps = [
        ":diffuse",
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
        ":diffuse",
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:tokenizer",
        "//iris:emissive_material",
        "//iris:reference_counted",
    ],
)

cc_test(
    name = "parse_test",
    srcs = ["parse_test.cc"],
    deps = [
        ":parse",
        "@com_google_googletest//:gtest_main",
    ],
)
