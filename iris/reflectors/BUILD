load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_reflector",
    testonly = True,
    hdrs = ["mock_reflector.h"],
    deps = [
        ":reference_countable_reflector",
        "//iris:float",
        "//iris:reflector",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "reference_countable_reflector",
    hdrs = ["reference_countable_reflector.h"],
    deps = [
        "//iris:float",
        "//iris:reference_countable",
        "//iris:reflector",
    ],
)
