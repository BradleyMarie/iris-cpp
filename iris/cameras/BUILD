load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_camera",
    testonly = True,
    hdrs = ["mock_camera.h"],
    deps = [
        "//iris:camera",
        "//iris:float",
        "//iris:ray",
        "@com_google_googletest//:gtest",
    ],
)

cc_library(
    name = "orthographic_camera",
    srcs = ["orthographic_camera.cc"],
    hdrs = ["orthographic_camera.h"],
    deps = [
        "//iris:camera",
        "//iris:float",
        "//iris:matrix",
        "//iris:ray",
    ],
)

cc_test(
    name = "orthographic_camera_test",
    srcs = ["orthographic_camera_test.cc"],
    deps = [
        ":orthographic_camera",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "perspective_camera",
    srcs = ["perspective_camera.cc"],
    hdrs = ["perspective_camera.h"],
    deps = [
        "//iris:camera",
        "//iris:float",
        "//iris:matrix",
        "//iris:ray",
        "//iris:vector",
    ],
)

cc_test(
    name = "perspective_camera_test",
    srcs = ["perspective_camera_test.cc"],
    deps = [
        ":perspective_camera",
        "@com_google_googletest//:gtest_main",
    ],
)
