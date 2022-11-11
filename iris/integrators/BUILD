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

cc_library(
    name = "path_integrator",
    srcs = ["path_integrator.cc"],
    hdrs = ["path_integrator.h"],
    deps = [
        "//iris:integrator",
        "//iris:light_sampler",
        "//iris:random",
        "//iris:ray",
        "//iris:ray_tracer",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris:visibility_tester",
        "//iris/integrators/internal:path_builder",
        "//iris/integrators/internal:russian_roulette",
        "//iris/integrators/internal:sample_direct_lighting",
    ],
)

cc_test(
    name = "path_integrator_test",
    srcs = ["path_integrator_test.cc"],
    deps = [
        ":path_integrator",
        "//iris/bxdfs:mock_bxdf",
        "//iris/environmental_lights:mock_environmental_light",
        "//iris/random:mock_random",
        "//iris/reflectors:mock_reflector",
        "//iris/spectra:mock_spectrum",
        "//iris/testing:light_sampler",
        "//iris/testing:ray_tracer",
        "//iris/testing:spectral_allocator",
        "//iris/testing:visibility_tester",
        "@com_google_googletest//:gtest_main",
    ],
)
