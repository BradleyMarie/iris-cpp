load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "lambertian_brdf",
    srcs = ["lambertian_brdf.h"],
    hdrs = ["lambertian_brdf.h"],
    deps = [
        "//iris:bsdf",
        "//iris:float",
        "//iris:random",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:vector",
    ],
)
