load("@rules_cc//cc:defs.bzl", "cc_binary")

OPENGL_LINKOPTS = [
    "-lGL",
    "-lX11",
]

cc_binary(
    name = "game",
    srcs = ["game.cc"],
    linkopts = OPENGL_LINKOPTS,
    deps = [
        "//tictactoe_astar/engine",
        "//tictactoe_astar/renderer",
        "@glfw",
    ],
)
