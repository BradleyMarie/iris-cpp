load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__subpackages__",
])

cc_library(
    name = "bxdf_allocator",
    testonly = True,
    srcs = ["bxdf_allocator.cc"],
    hdrs = ["bxdf_allocator.h"],
    deps = [
        "//iris:bxdf_allocator",
        "//iris/internal:arena",
    ],
)

cc_library(
    name = "light_sample_allocator",
    testonly = True,
    srcs = ["light_sample_allocator.cc"],
    hdrs = ["light_sample_allocator.h"],
    deps = [
        "//iris:light_sample_allocator",
        "//iris/internal:arena",
    ],
)

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
        "//iris:scene_objects",
        "//iris/internal:hit",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "spectral_allocator",
    testonly = True,
    srcs = ["spectral_allocator.cc"],
    hdrs = ["spectral_allocator.h"],
    deps = [
        "//iris:spectral_allocator",
        "//iris/internal:arena",
    ],
)

cc_library(
    name = "visibility_tester",
    testonly = True,
    srcs = ["visibility_tester.cc"],
    hdrs = ["visibility_tester.h"],
    deps = [
        "//iris:visibility_tester",
        "//iris/internal:arena",
        "//iris/internal:ray_tracer",
        "//iris/internal:visibility_tester",
    ],
)
