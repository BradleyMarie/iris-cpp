load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "float",
    hdrs = ["float.h"],
)

cc_library(
    name = "point",
    hdrs = ["point.h"],
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
    name = "vector",
    hdrs = ["vector.h"],
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