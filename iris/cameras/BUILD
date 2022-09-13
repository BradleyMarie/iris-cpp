load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_camera",
    testonly = True,
    hdrs = ["mock_camera.h"],
    deps = [
        "//iris:camera",
        "//iris:float",
        "//iris:ray",
        "@com_google_googletest//:gtest",
    ],
)
