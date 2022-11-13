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
