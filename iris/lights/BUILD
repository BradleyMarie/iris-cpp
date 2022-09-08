load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "point_light",
    srcs = ["point_light.cc"],
    hdrs = ["point_light.h"],
    deps = [
        "//iris:float",
        "//iris:light",
        "//iris:point",
        "//iris:random",
        "//iris:ray",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris:visibility_tester",
    ],
)

cc_test(
    name = "point_light_test",
    srcs = ["point_light_test.cc"],
    deps = [
        ":point_light",
        "//iris/random:mock_random",
        "//iris/spectra:mock_spectrum",
        "//iris/testing:light_tester",
        "//iris/testing:mock_visibility_tester",
        "@com_google_googletest//:gtest_main",
    ],
)
