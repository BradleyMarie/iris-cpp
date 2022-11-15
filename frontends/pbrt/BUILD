load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//frontends/pbrt:__subpackages__"])

cc_library(
    name = "parameter_list",
    srcs = ["parameter_list.cc"],
    hdrs = ["parameter_list.h"],
    deps = [
        ":quoted_string",
        ":tokenizer",
        "//iris:color",
        "//iris:point",
        "//iris:vector",
    ],
)

cc_test(
    name = "parameter_list_test",
    srcs = ["parameter_list_test.cc"],
    deps = [
        ":parameter_list",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "parameter",
    srcs = ["parameter.cc"],
    hdrs = ["parameter.h"],
    deps = [
        ":parameter_list",
        ":spectrum_manager",
        ":texture_manager",
        "//iris:reference_counted",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris:spectrum",
        "//iris/textures:texture2d",
    ],
)

cc_test(
    name = "parameter_test",
    srcs = ["parameter_test.cc"],
    deps = [
        ":parameter",
        "//iris/reflectors:mock_reflector",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "spectrum_manager",
    srcs = ["spectrum_manager.cc"],
    hdrs = ["spectrum_manager.h"],
    deps = [
        "//iris:color",
        "//iris:reference_counted",
        "//iris:reflector",
        "//iris:spectrum",
    ],
)

cc_test(
    name = "spectrum_manager_test",
    srcs = ["spectrum_manager_test.cc"],
    deps = [
        ":spectrum_manager",
        "//iris/reflectors:mock_reflector",
        "//iris/spectra:mock_spectrum",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "texture_manager",
    srcs = ["texture_manager.cc"],
    hdrs = ["texture_manager.h"],
    deps = [
        "//iris:reference_counted",
        "//iris:reflector",
        "//iris:spectral_allocator",
        "//iris/textures:constant_texture",
        "//iris/textures:texture2d",
    ],
)

cc_test(
    name = "texture_manager_test",
    srcs = ["texture_manager_test.cc"],
    deps = [
        ":texture_manager",
        "//iris/reflectors:mock_reflector",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "quoted_string",
    srcs = ["quoted_string.cc"],
    hdrs = ["quoted_string.h"],
)

cc_test(
    name = "quoted_string_test",
    srcs = ["quoted_string_test.cc"],
    deps = [
        ":quoted_string",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "tokenizer",
    srcs = ["tokenizer.cc"],
    hdrs = ["tokenizer.h"],
)

cc_test(
    name = "tokenizer_test",
    srcs = ["tokenizer_test.cc"],
    deps = [
        ":tokenizer",
        "@com_google_googletest//:gtest_main",
    ],
)
