load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "triangle_mesh",
    srcs = ["triangle_mesh.cc"],
    hdrs = ["triangle_mesh.h"],
    deps = [
        "//iris:geometry",
        "//iris:reference_counted",
    ],
)
