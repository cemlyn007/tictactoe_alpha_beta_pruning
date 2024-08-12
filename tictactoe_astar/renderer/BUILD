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
    data = [
        "shader.frag",
        "shader.vert",
    ],
    linkopts = OPENGL_LINKOPTS,
    deps = [
        ":gl_error_macro",
        "@glew//:glew_static",
        "@glfw",
    ],
)

cc_library(
    name = "gl_error_macro",
    srcs = ["gl_error_macro.cc"],
    hdrs = ["gl_error_macro.h"],
    linkopts = OPENGL_LINKOPTS,
    deps = [
        "@glew//:glew_static",
        "@glfw",
    ],
)
