load("@io_bazel_rules_docker//cc:image.bzl", "cc_image")
load("@io_bazel_rules_docker//container:image.bzl", "container_image")

# genrule(
#     name = "compile_topk",
#     outs = ["programs.zip"],
#     srcs = [
#         "topk.mpc"
#     ],
#     cmd = """mkdir -p Programs/Source &&\ 
#     cp $< @D/Programs/Source &&\
#     $(location @com_github_data61_mp_spdz//:compile) -F 64 topk.mpc &&\
#     zip -r programs.zip Programs
#     """,
#     tools = ["@com_github_data61_mp_spdz//:compile"],
# )

# cc_image(
#     name = "main_image",
#     binary = "@com_github_data61_mp_spdz//:semi_party.x",
#     base = "@distroless_base//image",
# )

container_image(
    name = "topk",
    base = "@mp_spdz_builder//image",
    directory = "/MP-SPDZ/Programs/Source",
    files = [
         ":topk.mpc",
         ":run_topk.sh"
    ],
    entrypoint = "/MP-SPDZ/Programs/Source/run_topk.sh",
    cmd = ""
)

