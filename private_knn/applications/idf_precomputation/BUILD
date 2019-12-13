load("@com_github_schoppmp_rules_oblivc//oblivc:oblivc.bzl", "oblivc_library")

oblivc_library(
    name = "dp_idf",
    srcs = [
        "dp_idf.oc",
    ],
    hdrs = [
        "dp_idf.h",
    ],
    deps = [
        "@ack//:oram",
    ],
)

cc_binary(
    name = "dp_idf_timing",
    srcs = [
        "dp_idf_timing.cpp",
    ],
    visibility = ["//visibility:public"],
    deps = [
        ":dp_idf",
        "@abseil//absl/strings",
        "//private_knn/util:get_ceil",
        "@boost//:range",
        "@mpc_utils//mpc_utils:benchmarker",
        "@mpc_utils//mpc_utils:comm_channel_oblivc_adapter",
        "@mpc_utils//mpc_utils:mpc_config",
    ],
)
