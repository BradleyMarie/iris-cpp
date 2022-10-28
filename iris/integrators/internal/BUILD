load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//iris/integrators:__subpackages__"])

cc_library(
    name = "russian_roulette",
    srcs = ["russian_roulette.cc"],
    hdrs = ["russian_roulette.h"],
    deps = [
        "//iris:float",
        "//iris:random",
    ],
)

cc_test(
    name = "russian_roulette_test",
    srcs = ["russian_roulette_test.cc"],
    deps = [
        ":russian_roulette",
        "//iris/random:mock_random",
        "@com_google_googletest//:gtest_main",
    ],
)

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

cc_test(
    name = "sample_direct_lighting_test",
    srcs = ["sample_direct_lighting_test.cc"],
    deps = [
        ":sample_direct_lighting",
        "@com_google_googletest//:gtest_main",
    ],
)
