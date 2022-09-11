load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__subpackages__",
])

cc_library(
    name = "mock_color_matcher",
    testonly = True,
    hdrs = ["mock_color_matcher.h"],
    deps = [
        "//iris:color_matcher",
        "//iris:float",
        "//iris:spectrum",
        "@com_google_googletest//:gtest",
    ],
)
