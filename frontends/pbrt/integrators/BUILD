load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//frontends/pbrt:__subpackages__"])

cc_library(
    name = "light_scenes",
    srcs = ["light_scenes.cc"],
    hdrs = ["light_scenes.h"],
    visibility = ["//visibility:public"],
    deps = [
        "//iris:light_scene",
        "//iris/light_scenes:all_light_scene",
        "//iris/light_scenes:one_light_scene",
    ],
)

cc_test(
    name = "light_scenes_test",
    srcs = ["light_scenes_test.cc"],
    deps = [
        ":light_scenes",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "parse",
    srcs = ["parse.cc"],
    hdrs = ["parse.h"],
    deps = [
        ":path",
        ":result",
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:tokenizer",
    ],
)

cc_library(
    name = "path",
    srcs = ["path.cc"],
    hdrs = ["path.h"],
    deps = [
        ":light_scenes",
        ":pixel_bounds",
        ":result",
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:quoted_string",
        "//iris/integrators:path_integrator",
    ],
)

cc_library(
    name = "pixel_bounds",
    srcs = ["pixel_bounds.cc"],
    hdrs = ["pixel_bounds.h"],
    deps = [
        "//frontends/pbrt:parameter",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//iris:integrator",
        "//iris:light_scene",
    ],
)
