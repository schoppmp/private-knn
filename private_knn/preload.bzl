load(
    "@bazel_tools//tools/build_defs/repo:http.bzl",
    "http_archive",
)

# Dependencies that need to be defined before :deps.bzl can be loaded.
# Copy those in a similar preload.bzl file in any workspace that depends on
# this one.
def private_knn_deps_preload():

    if "io_bazel_rules_go" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_go",
            urls = [
                "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/rules_go/releases/download/v0.20.3/rules_go-v0.20.3.tar.gz",
                "https://github.com/bazelbuild/rules_go/releases/download/v0.20.3/rules_go-v0.20.3.tar.gz",
            ],
            sha256 = "e88471aea3a3a4f19ec1310a55ba94772d087e9ce46e41ae38ecebe17935de7b",
        )
    if "bazel_gazelle" not in native.existing_rules():
        http_archive(
            name = "bazel_gazelle",
            urls = [
                "https://storage.googleapis.com/bazel-mirror/github.com/bazelbuild/bazel-gazelle/releases/download/v0.19.1/bazel-gazelle-v0.19.1.tar.gz",
                "https://github.com/bazelbuild/bazel-gazelle/releases/download/v0.19.1/bazel-gazelle-v0.19.1.tar.gz",
            ],
            sha256 = "86c6d481b3f7aedc1d60c1c211c6f76da282ae197c3b3160f54bd3a8f847896f",
        )
    # Transitive dependencies of sparse_linear_algebra.
    if "com_github_nelhage_rules_boost" not in native.existing_rules():
        http_archive(
            name = "com_github_nelhage_rules_boost",
            url = "https://github.com/nelhage/rules_boost/archive/691a53dd7dc4fb8ab70f61acad9b750a1bf10dc6.zip",
            sha256 = "5837d6bcf96c60dc1407126e828287098f91a8c69d8c2ccf8ebb0282ed35b401",
            strip_prefix = "rules_boost-691a53dd7dc4fb8ab70f61acad9b750a1bf10dc6",
        )
    if "com_github_schoppmp_rules_oblivc" not in native.existing_rules():
        http_archive(
            name = "com_github_schoppmp_rules_oblivc",
            sha256 = "86a4e546eecc06532ee2b0df19d3e8ab9a9ca32d65b7b3e4348da8c09ed4aa1a",
            url = "https://github.com/schoppmp/rules_oblivc/archive/dfc8c1fd3d6b5c6a4be12aa34f65d628230d2e87.zip",
            strip_prefix = "rules_oblivc-dfc8c1fd3d6b5c6a4be12aa34f65d628230d2e87",
        )
    if "rules_foreign_cc" not in native.existing_rules():
        http_archive(
            name = "rules_foreign_cc",
            url = "https://github.com/bazelbuild/rules_foreign_cc/archive/6bb0536452eaca3bad20c21ba6e7968d2eda004d.zip",
            strip_prefix = "rules_foreign_cc-6bb0536452eaca3bad20c21ba6e7968d2eda004d",
            sha256 = "ab266a13f5f695c898052271af860bf4928fb2ef6a333f7b63076b81271e4342",
        )
    if "bazel_skylib" not in native.existing_rules():
        http_archive(
            name = "bazel_skylib",
            sha256 = "c33a54ef16961e48df7d306a67ccb92c0c4627d0549df519e07533a6f3d270aa",
            strip_prefix = "bazel-skylib-9b85311ab47548ec051171213a1b3b4b3b3c9dc8",
            url = "https://github.com/bazelbuild/bazel-skylib/archive/9b85311ab47548ec051171213a1b3b4b3b3c9dc8.zip",
        )
    if "com_github_schoppmp_mpc_utils" not in native.existing_rules():
        http_archive(
            name = "mpc_utils",
            url = "https://github.com/schoppmp/mpc-utils/archive/1b97fad04b78d9d3e0b6780a4b782b4c017ebe5c.zip",
            sha256 = "8d81c56cfdbd757d5e4b4885bc0dff9c7b57f44c8b439bc340e61ccf24c48a87",
            strip_prefix = "mpc-utils-1b97fad04b78d9d3e0b6780a4b782b4c017ebe5c",
        )
    # if "io_bazel_rules_docker" not in native.existing_rules():
    #     http_archive(
    #         name = "io_bazel_rules_docker",
    #         sha256 = "ed884a780b82b0586b9c5454a2163b4133b2c50788f9334de4b6008780e26032",
    #         strip_prefix = "rules_docker-170335d284991ecc9fa5a6682c46bd32f167daa9",
    #         url = "https://github.com/bazelbuild/rules_docker/archive/170335d284991ecc9fa5a6682c46bd32f167daa9.zip",
    #     )
    if "io_bazel_rules_docker" not in native.existing_rules():
        http_archive(
            name = "io_bazel_rules_docker",
            sha256 = "14ac30773fdb393ddec90e158c9ec7ebb3f8a4fd533ec2abbfd8789ad81a284b",
            strip_prefix = "rules_docker-0.12.1",
            urls = ["https://github.com/bazelbuild/rules_docker/releases/download/v0.12.1/rules_docker-v0.12.1.tar.gz"],
        )
            


    if "com_github_schoppmp_room_framework" not in native.existing_rules():
        http_archive(
            name = "room_framework",
            sha256 = "50d436be1ae525ae556d315cda253c6ba5fa3061d5492c245fc0def918418415",
            strip_prefix = "room-framework-fddcb654a8a9d3af4adbdb0684c986bcc5b40fde",
            url = "https://github.com/schoppmp/room-framework/archive/fddcb654a8a9d3af4adbdb0684c986bcc5b40fde.zip",
            # TODO: remove this simple visibility patch by merging the small change into the room repo
            patch_args = ["-p1"],
            patches = ["//third_party:util_visibility.patch"],
        )
    http_archive(
        name = "rules_python",
        url = "https://github.com/bazelbuild/rules_python/releases/download/0.0.1/rules_python-0.0.1.tar.gz",
        sha256 = "aa96a691d3a8177f3215b14b0edc9641787abaaa30363a080165d06ab65e1161",
    )