load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "matte",
    srcs = ["matte.cc"],
    hdrs = ["matte.h"],
    deps = [
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:texture_manager",
        "//iris:material",
        "//iris:reference_counted",
        "//iris/materials:matte_material",
    ],
)
