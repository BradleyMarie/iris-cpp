load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "constant_emissive_material",
    srcs = ["constant_emissive_material.cc"],
    hdrs = ["constant_emissive_material.h"],
    deps = [
        "//iris:emissive_material",
        "//iris:reference_counted",
        "//iris:spectral_allocator",
        "//iris:texture_coordinates",
    ],
)

cc_test(
    name = "constant_emissive_material_test",
    srcs = ["constant_emissive_material_test.cc"],
    deps = [
        ":constant_emissive_material",
        "//iris/spectra:mock_spectrum",
        "//iris/testing:emissive_material_tester",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_emissive_material",
    testonly = True,
    hdrs = ["mock_emissive_material.h"],
    deps = [
        "//iris:emissive_material",
        "//iris:spectral_allocator",
        "//iris:texture_coordinates",
        "@com_google_googletest//:gtest",
    ],
)
