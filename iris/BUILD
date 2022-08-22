load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "float",
    hdrs = ["float.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "geometry",
    srcs = ["geometry.cc"],
    hdrs = ["geometry.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":hit_allocator",
        ":integer",
        ":normal_map",
        ":point",
        ":ray",
        ":texture_coordinates",
        ":vector",
        "//iris/internal:hit_arena",
    ],
)

cc_test(
    name = "geometry_test",
    srcs = ["geometry_test.cc"],
    deps = [
        ":geometry",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "hit",
    hdrs = ["hit.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_library(
    name = "hit_allocator",
    srcs = ["hit_allocator.cc"],
    hdrs = ["hit_allocator.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":hit",
        ":integer",
        ":ray",
        "//iris/internal:hit_arena",
    ],
)

cc_test(
    name = "hit_allocator_test",
    srcs = ["hit_allocator_test.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":hit_allocator",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "integer",
    hdrs = ["integer.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "intersector",
    srcs = ["intersector.cc"],
    hdrs = ["intersector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":geometry",
        ":hit_allocator",
        ":matrix",
        "//iris/internal:hit",
    ],
)

cc_test(
    name = "intersector_test",
    srcs = ["intersector_test.cc"],
    deps = [
        ":intersector",
        "//iris/internal:hit_arena",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "matrix",
    srcs = ["matrix.cc"],
    hdrs = ["matrix.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":point",
        ":ray",
        ":vector",
        "@com_google_absl//absl/status:statusor",
    ],
)

cc_test(
    name = "matrix_test",
    srcs = ["matrix_test.cc"],
    deps = [
        ":matrix",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "normal_map",
    hdrs = ["normal_map.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":texture_coordinates",
        ":vector",
    ],
)

cc_library(
    name = "point",
    hdrs = ["point.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":vector",
    ],
)

cc_test(
    name = "point_test",
    srcs = ["point_test.cc"],
    deps = [
        ":point",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "ray",
    hdrs = ["ray.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":point",
        ":vector",
    ],
)

cc_test(
    name = "ray_test",
    srcs = ["ray_test.cc"],
    deps = [
        ":ray",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "reflector",
    hdrs = ["reflector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_library(
    name = "scene",
    hdrs = ["scene.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":intersector",
        ":ray",
    ],
)

cc_library(
    name = "spectrum",
    hdrs = ["spectrum.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_library(
    name = "texture_coordinates",
    hdrs = ["texture_coordinates.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":vector",
    ],
)

cc_library(
    name = "vector",
    hdrs = ["vector.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
    ],
)

cc_test(
    name = "vector_test",
    srcs = ["vector_test.cc"],
    deps = [
        ":vector",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "visibility_tester",
    srcs = ["visibility_tester.cc"],
    hdrs = ["visibility_tester.h"],
    visibility = ["//visibility:public"],
    deps = [
        ":float",
        ":ray",
        ":scene",
        "//iris/internal:ray_tracer",
    ],
)

cc_test(
    name = "visibility_tester_test",
    srcs = ["visibility_tester_test.cc"],
    deps = [
        ":visibility_tester",
        "//iris/internal:ray_tracer",
        "@com_google_googletest//:gtest_main",
    ],
)
