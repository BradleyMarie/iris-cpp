load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "all_light_scene",
    srcs = ["all_light_scene.cc"],
    hdrs = ["all_light_scene.h"],
    deps = [
        "//iris:light_sample",
        "//iris:light_sample_allocator",
        "//iris:light_scene",
        "//iris:point",
        "//iris:random",
        "//iris:scene_objects",
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
        "//iris/testing:light_sample_allocator",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "one_light_scene",
    srcs = ["one_light_scene.cc"],
    hdrs = ["one_light_scene.h"],
    deps = [
        "//iris:light_sample",
        "//iris:light_sample_allocator",
        "//iris:light_scene",
        "//iris:point",
        "//iris:random",
        "//iris:scene_objects",
    ],
)

cc_test(
    name = "one_light_scene_test",
    srcs = ["one_light_scene_test.cc"],
    deps = [
        ":one_light_scene",
        "//iris/lights:mock_light",
        "//iris/random:mock_random",
        "//iris/scenes:list_scene",
        "//iris/testing:light_sample_allocator",
        "@com_google_googletest//:gtest_main",
    ],
)
