load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_image_sampler",
    testonly = True,
    hdrs = ["mock_image_sampler.h"],
    deps = [
        "//iris:float",
        "//iris:image_sampler",
        "//iris:random",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "random_image_sampler",
    srcs = ["random_image_sampler.cc"],
    hdrs = ["random_image_sampler.h"],
    deps = [
        "//iris:float",
        "//iris:image_sampler",
        "//iris:random",
    ],
)

cc_test(
    name = "random_image_sampler_test",
    srcs = ["random_image_sampler_test.cc"],
    deps = [
        ":random_image_sampler",
        "//iris/random:mock_random",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "stratified_image_sampler",
    srcs = ["stratified_image_sampler.cc"],
    hdrs = ["stratified_image_sampler.h"],
    deps = [
        "//iris:float",
        "//iris:image_sampler",
        "//iris:random",
    ],
)

cc_test(
    name = "stratified_image_sampler_test",
    srcs = ["stratified_image_sampler_test.cc"],
    deps = [
        ":stratified_image_sampler",
        "//iris/random:mock_random",
        "@com_google_googletest//:gtest_main",
    ],
)
