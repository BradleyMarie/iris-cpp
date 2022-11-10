load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_environmental_light",
    testonly = True,
    hdrs = ["mock_environmental_light.h"],
    deps = [
        "//iris:environmental_light",
        "//iris:float",
        "//iris:sampler",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris:vector",
        "@com_google_googletest//:gtest",
    ],
)
