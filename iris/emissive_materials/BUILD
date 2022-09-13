load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_emissive_material",
    testonly = True,
    hdrs = ["mock_emissive_material.h"],
    deps = [
        "//iris:emissive_material",
        "//iris:texture_coordinates",
        "@com_google_googletest//:gtest",
    ],
)
