load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parse",
    srcs = ["parse.cc"],
    hdrs = ["parse.h"],
    visibility = ["//frontends/pbrt:__subpackages__"],
    deps = [
        ":random",
        ":stratified",
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:tokenizer",
        "//iris:image_sampler",
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
    name = "random",
    srcs = ["random.cc"],
    hdrs = ["random.h"],
    deps = [
        "//frontends/pbrt:object_builder",
        "//iris:image_sampler",
        "//iris/image_samplers:random_image_sampler",
    ],
)

cc_test(
    name = "random_test",
    srcs = ["random_test.cc"],
    deps = [
        ":random",
        "//frontends/pbrt:build_objects",
        "//iris/reflectors:mock_reflector",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "stratified",
    srcs = ["stratified.cc"],
    hdrs = ["stratified.h"],
    deps = [
        "//frontends/pbrt:object_builder",
        "//iris:image_sampler",
        "//iris/image_samplers:stratified_image_sampler",
    ],
)

cc_test(
    name = "stratified_test",
    srcs = ["stratified_test.cc"],
    deps = [
        ":stratified",
        "//frontends/pbrt:build_objects",
        "//iris/reflectors:mock_reflector",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)
