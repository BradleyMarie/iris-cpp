load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//frontends/pbrt:__subpackages__"])

cc_library(
    name = "test_spectrum_manager",
    testonly = True,
    srcs = ["test_spectrum_manager.cc"],
    hdrs = ["test_spectrum_manager.h"],
    deps = [
        "//frontends/pbrt:spectrum_manager",
        "//iris/reflectors:mock_reflector",
        "//iris/spectra:mock_spectrum",
    ],
)
