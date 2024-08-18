load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//tictactoe_ai:__subpackages__"])

opengl_linkopts = select({
    "@platforms//os:windows": ["-DEFAULTLIB:opengl32.lib"],
    "@platforms//os:linux": ["-lGL", "-lX11"],
})

cc_library(
    name = "renderer",
    srcs = ["renderer.cc"],
    hdrs = ["renderer.h"],
    data = [
        "//tictactoe_ai/renderer/shaders:shader.frag",
        "//tictactoe_ai/renderer/shaders:shader.vert",
    ],
    linkopts = opengl_linkopts,
    deps = [
        ":cross",
        ":gl_error_macro",
        ":grid",
        ":nought",
        ":shader",
        "//tictactoe_ai/engine",
        "@glew//:glew_static",
        "@glfw",
    ],
)

cc_library(
    name = "shader",
    srcs = ["shader.cc"],
    hdrs = ["shader.h"],
    linkopts = opengl_linkopts,
    deps = [
        ":gl_error_macro",
        "@glew//:glew_static",
        "@glfw",
    ],
)

cc_library(
    name = "grid",
    srcs = ["grid.cc"],
    hdrs = ["grid.h"],
    linkopts = opengl_linkopts,
    deps = [
        ":gl_error_macro",
        ":shader",
        "@glew//:glew_static",
        "@glfw",
    ],
)

cc_library(
    name = "nought",
    srcs = ["nought.cc"],
    hdrs = ["nought.h"],
    linkopts = opengl_linkopts,
    deps = [
        ":gl_error_macro",
        ":shader",
        "@glew//:glew_static",
        "@glfw",
    ],
)

cc_library(
    name = "cross",
    srcs = ["cross.cc"],
    hdrs = ["cross.h"],
    linkopts = opengl_linkopts,
    deps = [
        ":gl_error_macro",
        ":shader",
        "@glew//:glew_static",
        "@glfw",
    ],
)

cc_library(
    name = "gl_error_macro",
    srcs = ["gl_error_macro.cc"],
    hdrs = ["gl_error_macro.h"],
    linkopts = opengl_linkopts,
    deps = [
        "@glew//:glew_static",
        "@glfw",
    ],
)
