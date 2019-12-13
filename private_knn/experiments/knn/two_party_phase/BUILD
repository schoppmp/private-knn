package(default_visibility = ["//visibility:public"])

load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")
load("@io_bazel_rules_docker//container:image.bzl", "container_image")

#load("@io_bazel_rules_docker//docker:docker.bzl", "docker_push")

cc_library(
    name = "knn_config",
    srcs = ["knn_config.cpp"],
    hdrs = ["knn_config.hpp"],
    deps = [
        "//private_knn/applications/knn/two_party_phase:knn_protocol",
        "@room_framework//sparse_linear_algebra/util",
    ],
)

cc_binary(
    name = "knn",
    srcs = [
        "knn.cpp",
    ],
    data = glob(["*.ini"]),
    deps = [
        ":knn_config",
        "//private_knn/applications/knn/two_party_phase:knn_protocol",
        "@room_framework//sparse_linear_algebra/util",
    ],
)

cc_binary(
    name = "knn_non_uniform_sparsity",
    srcs = [
        "knn_non_uniform_sparsity.cpp",
    ],
    data = glob(["*.ini"]),
    deps = [
        ":knn_config",
        "//private_knn/applications/knn/two_party_phase:knn_protocol",
        "@room_framework//sparse_linear_algebra/util",
    ],
)

cc_image(
    name = "main_image",
    binary = ":knn",
    base = "@distroless_base//image",
)

cc_image(
    name = "main_image_non_uniform_sparsity",
    binary = ":knn_non_uniform_sparsity",
    base = "@distroless_base//image",
)

container_image(
    name = "image",
    base = ":main_image",
    env = {
        "MPC_UTILS_CONFIG": "/knn.ini",
    },
    files = [
         "//private_knn/experiments/knn/two_party_phase/configs:knn.ini"
    ],
)

container_image(
    name = "non_uniform_sparsity",
    base = "main_image_non_uniform_sparsity",
    env = {
        "MPC_UTILS_CONFIG": "/knn_non_uniform_sparsity_varying_sparsity.ini",
    },
    files = [
         "//private_knn/experiments/knn/two_party_phase/configs:knn_non_uniform_sparsity_varying_sparsity.ini"
    ],
)

container_image(
    name = "non_uniform_sparsity2",
    base = "main_image_non_uniform_sparsity",
    env = {
        "MPC_UTILS_CONFIG": "/knn_non_uniform_sparsity_varying_sparsity2.ini",
    },
    files = [
         "//private_knn/experiments/knn/two_party_phase/configs:knn_non_uniform_sparsity_varying_sparsity2.ini"
    ],
)

container_image(
    name = "realworld_nonzeros",
    base = "main_image_non_uniform_sparsity",
    env = {
        "MPC_UTILS_CONFIG": "/knn_realworld_nonzeros.ini",
    },
    files = [
         "//private_knn/experiments/knn/two_party_phase/configs:knn_realworld_nonzeros.ini"
    ],
)
