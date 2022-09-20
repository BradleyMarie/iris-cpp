load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lambertian_brdf",
    srcs = ["lambertian_brdf.cc"],
    hdrs = ["lambertian_brdf.h"],
    deps = [
        "//iris:bsdf",
        "//iris:float",
        "//iris:random",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:vector",
        "//iris/bsdfs/utility:local",
    ],
)

cc_library(
    name = "mock_bsdf",
    hdrs = ["mock_bsdf.h"],
    deps = [
        "//iris:bsdf",
        "//iris:float",
        "//iris:random",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:vector",
        "@com_google_googletest//:gtest",
    ],
)
