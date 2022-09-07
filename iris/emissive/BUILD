load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__subpackages__",
])

cc_library(
    name = "mock_emissive_material",
    testonly = True,
    hdrs = ["mock_emissive_material.h"],
    deps = [
        "//iris:float",
        "//iris:spectrum",
        "@com_google_googletest//:gtest",
    ],
)
