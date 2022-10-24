load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//iris/integrators:__subpackages__"])

cc_library(
    name = "sample_direct_lighting",
    srcs = ["sample_direct_lighting.cc"],
    hdrs = ["sample_direct_lighting.h"],
    deps = [
        "//iris:light",
        "//iris:random",
        "//iris:ray",
        "//iris:ray_tracer",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris:visibility_tester",
    ],
)
