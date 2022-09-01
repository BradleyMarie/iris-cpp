load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__subpackages__",
])

cc_library(
    name = "scene",
    testonly = True,
    srcs = ["scene.cc"],
    hdrs = ["scene.h"],
    deps = [
        "//iris:float",
        "//iris:geometry",
        "//iris:hit",
        "//iris:matrix",
        "//iris:ray",
        "//iris:scene",
        "//iris/internal:hit",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)
