load("@room_framework//sparse_linear_algebra:deps.bzl", "sparse_linear_algebra_deps")
load("@rules_python//python:pip.bzl", "pip_import")
load(
    "@io_bazel_rules_docker//repositories:repositories.bzl",
    container_repositories = "repositories",
)
load(
    "@io_bazel_rules_docker//cc:image.bzl",
    _cc_image_repos = "repositories",
)
load(
    "@io_bazel_rules_docker//container:container.bzl",
    "container_pull",
)

def private_knn_deps():
    sparse_linear_algebra_deps()

    if "com_github_data61_mp_spdz" not in native.existing_rules():
        native.new_local_repository(
            name = "com_github_data61_mp_spdz",
            path = "../MP-SPDZ",
            build_file = "//third_party:BUILD.mp-spdz",
        )


    pip_import( 
        name = "py_deps",
        requirements = "//third_party:requirements.txt",
    )

    container_repositories()
    _cc_image_repos()

    # Something needs a recent GlibC, which is not included in standard distroless images.
    container_pull(
        name = "distroless_base",
        digest = "sha256:6d25761ba94c2b94db2dbea59390eaca65ff39596d64ec940d74140e1dc8872a",
        registry = "index.docker.io",
        repository = "schoppmp/distroless-arch",
    )

    # MP-SPDZ builder
    container_pull(
        name = "mp_spdz_builder",
        digest = "sha256:1582f132f761d5e915739bce949adcf184f4f4c675d11f24613edaf855948f13",
        registry = "index.docker.io",
        repository = "levitatingorange/mp-spdz-builder",
    )