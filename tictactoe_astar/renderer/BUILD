load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//tictactoe_astar:__subpackages__"])

OPENGL_LINKOPTS = [
    "-lGL",
    "-lX11",
]

cc_library(
    name = "renderer",
    srcs = ["renderer.cc"],
    hdrs = ["renderer.h"],
    linkopts = OPENGL_LINKOPTS,
    deps = [
        "@glew//:glew_static",
        "@glfw",
    ],
)
