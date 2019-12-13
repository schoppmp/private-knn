load("@py_deps//:requirements.bzl", "pip_install")

def private_knn_deps_postload():
    pip_install()