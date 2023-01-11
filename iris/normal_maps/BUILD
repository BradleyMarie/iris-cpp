load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_normal_map",
    testonly = True,
    hdrs = ["mock_normal_map.h"],
    deps = [
        "//iris:float",
        "//iris:normal_map",
        "//iris:reference_countable",
        "//iris:texture_coordinates",
        "//iris:vector",
        "@com_google_googletest//:gtest",
    ],
)
