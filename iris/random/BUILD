load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__subpackages__",
])

cc_library(
    name = "mock_random",
    testonly = True,
    hdrs = ["mock_random.h"],
    deps = [
        "//iris:random",
        "@com_google_googletest//:gtest",
    ],
)
