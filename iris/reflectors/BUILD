load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__subpackages__",
])

cc_library(
    name = "mock_reflector",
    testonly = True,
    hdrs = ["mock_reflector.h"],
    deps = [
        "//iris:float",
        "//iris:reflector",
        "@com_google_googletest//:gtest",
    ],
)
