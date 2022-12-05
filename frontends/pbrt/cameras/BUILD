load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "orthographic",
    srcs = ["orthographic.cc"],
    hdrs = ["orthographic.h"],
    deps = [
        "//frontends/pbrt:matrix_manager",
        "//frontends/pbrt:object_builder",
        "//iris:camera",
        "//iris/cameras:orthographic_camera",
    ],
)

cc_test(
    name = "orthographic_test",
    srcs = ["orthographic_test.cc"],
    deps = [
        ":orthographic",
        ":parse",
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
        ":orthographic",
        ":perspective",
        "//frontends/pbrt:matrix_manager",
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:tokenizer",
        "//iris:camera",
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

cc_library(
    name = "perspective",
    srcs = ["perspective.cc"],
    hdrs = ["perspective.h"],
    deps = [
        "//frontends/pbrt:matrix_manager",
        "//frontends/pbrt:object_builder",
        "//iris:camera",
        "//iris/cameras:perspective_camera",
    ],
)

cc_test(
    name = "perspective_test",
    srcs = ["perspective_test.cc"],
    deps = [
        ":parse",
        ":perspective",
        "//frontends/pbrt:build_objects",
        "//frontends/pbrt/spectrum_managers:test_spectrum_manager",
        "@com_google_googletest//:gtest_main",
    ],
)
