load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "diffuse",
    srcs = ["diffuse.cc"],
    hdrs = ["diffuse.h"],
    deps = [
        "//frontends/pbrt:object_builder",
        "//iris:reference_counted",
        "//iris/emissive_materials:constant_emissive_material",
    ],
)

cc_library(
    name = "parse",
    srcs = ["parse.cc"],
    hdrs = ["parse.h"],
    visibility = ["//frontends/pbrt:__subpackages__"],
    deps = [
        ":diffuse",
        "//frontends/pbrt:object_builder",
        "//frontends/pbrt:tokenizer",
        "//iris:emissive_material",
        "//iris:reference_counted",
    ],
)
