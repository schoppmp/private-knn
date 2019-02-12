load("@sparse_linear_algebra//sparse_linear_algebra:deps.bzl", "sparse_linear_algebra_deps")

def private_knn_deps():
    sparse_linear_algebra_deps()