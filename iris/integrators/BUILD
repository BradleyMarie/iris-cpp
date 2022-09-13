load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mock_integrator",
    testonly = True,
    hdrs = ["mock_integrator.h"],
    deps = [
        "//iris:integrator",
        "//iris:light_sampler",
        "//iris:random",
        "//iris:ray",
        "//iris:ray_tracer",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris:visibility_tester",
        "@com_google_googletest//:gtest",
    ],
)
