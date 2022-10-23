load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "list_scene",
    srcs = ["list_scene.cc"],
    hdrs = ["list_scene.h"],
    deps = [
        "//iris:intersector",
        "//iris:ray",
        "//iris:scene",
        "//iris:scene_objects",
    ],
)

cc_test(
    name = "list_scene_test",
    srcs = ["list_scene_test.cc"],
    deps = [
        ":list_scene",
        "//iris/testing:scene",
        "@com_google_googletest//:gtest_main",
    ],
)
