load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//iris:__pkg__"])

cc_library(
    name = "hit_arena",
    srcs = ["hit_arena.cc"],
    hdrs = ["hit_arena.h"],
    deps = [
        ":hit",
    ],
)

cc_test(
    name = "hit_arena_test",
    srcs = ["hit_arena_test.cc"],
    deps = [
        ":hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "hit",
    hdrs = ["hit.h"],
    deps = [
        "//iris:hit",
        "//iris:integer",
    ],
)

cc_library(
    name = "ray_tracer",
    srcs = ["ray_tracer.cc"],
    hdrs = ["ray_tracer.h"],
    deps = [
        ":hit",
        ":hit_arena",
        "//iris:float",
        "//iris:ray",
        "//iris:scene",
    ],
)

cc_test(
    name = "ray_tracer_test",
    srcs = ["ray_tracer_test.cc"],
    deps = [
        ":ray_tracer",
        "@com_google_googletest//:gtest_main",
    ],
)