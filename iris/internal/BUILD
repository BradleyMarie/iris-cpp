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
        "//iris:geometry",
        "//iris:hit",
        "//iris:integer",
        "//iris:matrix",
    ],
)
