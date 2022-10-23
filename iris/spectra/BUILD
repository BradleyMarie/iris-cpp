load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_spectrum",
    testonly = True,
    hdrs = ["mock_spectrum.h"],
    deps = [
        ":reference_countable_spectrum",
        "//iris:float",
        "//iris:spectrum",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "reference_countable_spectrum",
    hdrs = ["reference_countable_spectrum.h"],
    deps = [
        "//iris:float",
        "//iris:reference_countable",
        "//iris:spectrum",
    ],
)
