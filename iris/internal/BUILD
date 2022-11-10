load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__pkg__",
    "//iris/testing:__pkg__",
])

cc_library(
    name = "area_light",
    srcs = ["area_light.cc"],
    hdrs = ["area_light.h"],
    deps = [
        ":visibility_tester",
        "//iris:float",
        "//iris:geometry",
        "//iris:integer",
        "//iris:light",
        "//iris:matrix",
        "//iris:ray",
        "//iris:spectral_allocator",
        "//iris:spectrum",
    ],
)

cc_test(
    name = "area_light_test",
    srcs = ["area_light_test.cc"],
    deps = [
        ":area_light",
        "//iris/emissive_materials:mock_emissive_material",
        "//iris/random:mock_random",
        "//iris/spectra:mock_spectrum",
        "//iris/testing:spectral_allocator",
        "//iris/testing:visibility_tester",
        "@com_google_googletest//:gtest_main",
    ],
)

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
    name = "environmental_light",
    srcs = ["environmental_light.cc"],
    hdrs = ["environmental_light.h"],
    deps = [
        "//iris:environmental_light",
        "//iris:float",
        "//iris:geometry",
        "//iris:integer",
        "//iris:light",
        "//iris:matrix",
        "//iris:ray",
        "//iris:reference_counted",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris:visibility_tester",
    ],
)

cc_test(
    name = "environmental_light_test",
    srcs = ["environmental_light_test.cc"],
    deps = [
        ":environmental_light",
        "//iris/environmental_lights:mock_environmental_light",
        "//iris/random:mock_random",
        "//iris/spectra:mock_spectrum",
        "//iris/testing:spectral_allocator",
        "//iris/testing:visibility_tester",
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
        "//iris:scene_objects",
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
