load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_spectrum",
    testonly = True,
    hdrs = ["mock_spectrum.h"],
    deps = [
        "//iris:float",
        "//iris:spectrum",
        "@com_google_googletest//:gtest",
    ],
)
