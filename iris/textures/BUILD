load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "constant_texture",
    hdrs = ["constant_texture.h"],
    deps = [
        ":texture2d",
        "//iris:texture_coordinates",
    ],
)

cc_test(
    name = "constant_texture_test",
    srcs = ["constant_texture_test.cc"],
    deps = [
        ":constant_texture",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "texture2d",
    hdrs = ["texture2d.h"],
    deps = [
        "//iris:texture_coordinates",
    ],
)
