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
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "dp_idf_timing",
    srcs = [
        "dp_idf_timing.cpp",
    ],
    deps = [
        ":dp_idf",
        "@boost//:range",
        "@mpc_utils//mpc_utils:comm_channel_oblivc",
        "@mpc_utils//mpc_utils:mpc_config",
        "@mpc_utils//mpc_utils:benchmarker",
        "//private_knn/util:get_ceil",
    ]
)