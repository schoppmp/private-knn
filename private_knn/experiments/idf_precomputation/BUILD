load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")
load("@io_bazel_rules_docker//container:image.bzl", "container_image")


cc_image(
    name = "main_image",
    binary = "//private_knn/applications/idf_precomputation:dp_idf_timing",
    base = "@distroless_base//image",
)

container_image(
    name = "idf_precomputation",
    base = ":main_image",
    env = {
        "MPC_UTILS_CONFIG": "/idf_precomputation.ini",
    },
    files = [
         "//private_knn/experiments/idf_precomputation:idf_precomputation.ini"
    ],
)
