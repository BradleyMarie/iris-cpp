load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__pkg__",
    "//iris/testing:__pkg__",
])

cc_library(
    name = "arena",
    srcs = ["arena.cc"],
    hdrs = ["arena.h"],
)

cc_test(
    name = "arena_test",
    srcs = ["arena_test.cc"],
    deps = [
        ":arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "hit_arena",
    srcs = ["hit_arena.cc"],
    hdrs = ["hit_arena.h"],
    deps = [
        ":arena",
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
        "//iris/scenes:list_scene",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "visibility_tester",
    srcs = ["visibility_tester.cc"],
    hdrs = ["visibility_tester.h"],
    deps = [
        ":ray_tracer",
        "//iris:float",
        "//iris:geometry",
        "//iris:integer",
        "//iris:matrix",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris:visibility_tester",
    ],
)

cc_test(
    name = "visibility_tester_test",
    srcs = ["visibility_tester_test.cc"],
    deps = [
        ":arena",
        ":visibility_tester",
        "//iris/scenes:list_scene",
        "@com_google_googletest//:gtest_main",
    ],
)
