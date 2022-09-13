load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = [
    "//iris:__subpackages__",
])

cc_library(
    name = "cie_color_matcher",
    srcs = ["cie_color_matcher.cc"],
    hdrs = ["cie_color_matcher.h"],
    deps = [
        "//iris:color",
        "//iris:color_matcher",
        "//iris:float",
        "//iris:spectrum",
    ],
)

cc_test(
    name = "cie_color_matcher_test",
    srcs = ["cie_color_matcher_test.cc"],
    deps = [
        ":cie_color_matcher",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_color_matcher",
    testonly = True,
    hdrs = ["mock_color_matcher.h"],
    deps = [
        "//iris:color_matcher",
        "//iris:float",
        "//iris:spectrum",
        "@com_google_googletest//:gtest",
    ],
)
