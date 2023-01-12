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

cc_test(
    name = "triangle_mesh_test",
    srcs = ["triangle_mesh_test.cc"],
    deps = [
        ":triangle_mesh",
        "//iris/emissive_materials:mock_emissive_material",
        "//iris/materials:mock_material",
        "//iris/normal_maps:mock_normal_map",
        "//iris/random:mock_random",
        "//iris/testing:hit_allocator",
        "@com_google_googletest//:gtest_main",
    ],
)
