load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "image",
    srcs = ["image.cc"],
    hdrs = ["image.h"],
    deps = [
        ":result",
        "//frontends/pbrt:object_builder",
        "//iris/file:pfm_writer",
    ],
)

cc_test(
    name = "image_test",
    srcs = ["image_test.cc"],
    deps = [
        ":image",
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
        ":image",
        ":result",
        "//frontends/pbrt:object_builder",
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
    name = "result",
    hdrs = ["result.h"],
    visibility = ["//frontends/pbrt:__subpackages__"],
    deps = [
        "//iris:framebuffer",
    ],
)
