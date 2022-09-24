load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "composite_bxdf",
    hdrs = ["composite_bxdf.h"],
    deps = [
        "//iris:bxdf",
        "//iris:float",
        "//iris:random",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:vector",
    ],
)

cc_test(
    name = "composite_bxdf_test",
    srcs = ["composite_bxdf_test.cc"],
    deps = [
        ":composite_bxdf",
        ":mock_bxdf",
        "//iris/random:mock_random",
        "//iris/reflectors:mock_reflector",
        "//iris/testing:bxdf_tester",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "lambertian_brdf",
    srcs = ["lambertian_brdf.cc"],
    hdrs = ["lambertian_brdf.h"],
    deps = [
        "//iris:bxdf",
        "//iris:float",
        "//iris:random",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:vector",
    ],
)

cc_test(
    name = "lambertian_brdf_test",
    srcs = ["lambertian_brdf_test.cc"],
    deps = [
        ":lambertian_brdf",
        "//iris/random:mock_random",
        "//iris/reflectors:mock_reflector",
        "//iris/testing:bxdf_tester",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_bxdf",
    hdrs = ["mock_bxdf.h"],
    deps = [
        "//iris:bxdf",
        "//iris:float",
        "//iris:random",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:vector",
        "@com_google_googletest//:gtest",
    ],
)
