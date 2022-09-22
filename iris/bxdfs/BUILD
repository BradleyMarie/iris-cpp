load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_bxdf",
    hdrs = ["mock_bxdf.h"],
    deps = [
        "//iris:bxdf",
        "//iris:float",
        "//iris:random",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:vector",
        "@com_google_googletest//:gtest",
    ],
)
