load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "reflector_manager",
    hdrs = ["reflector_manager.h"],
    deps = [
        "//iris:reflector",
    ],
)

cc_test(
    name = "reflector_manager_test",
    srcs = ["reflector_manager_test.cc"],
    deps = [
        ":reflector_manager",
        "//iris/reflectors:mock_reflector",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "spectrum_manager",
    hdrs = ["spectrum_manager.h"],
    deps = [
        "//iris:spectrum",
    ],
)

cc_test(
    name = "spectrum_manager_test",
    srcs = ["spectrum_manager_test.cc"],
    deps = [
        ":spectrum_manager",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)
