load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "all_light_scene",
    srcs = ["all_light_scene.cc"],
    hdrs = ["all_light_scene.h"],
    deps = [
        "//iris:light",
        "//iris:light_sample",
        "//iris:light_sample_allocator",
        "//iris:light_scene",
        "//iris:point",
        "//iris:random",
    ],
)

cc_test(
    name = "all_light_scene_test",
    srcs = ["all_light_scene_test.cc"],
    deps = [
        ":all_light_scene",
        "//iris/lights:mock_light",
        "//iris/random:mock_random",
        "//iris/scenes:list_scene",
        "//iris/testing:light_scene_tester",
        "@com_google_googletest//:gtest_main",
    ],
)
