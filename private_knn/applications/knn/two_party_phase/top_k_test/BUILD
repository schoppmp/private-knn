load("@py_deps//:requirements.bzl", "requirement")

cc_test(
    name = "top_k_test_generated",
    timeout = "short",
    srcs = ["top_k_test_generated.cpp"],
    data = [
        ":test1.csv",
        ":test2.csv",
        ":test3.csv",
        ":test4.csv",
        ":test5.csv",
    ],
    deps = [
        "//private_knn/applications/knn/two_party_phase:top_k",
        "@boost//:tokenizer",
        "@googletest//:gtest_main",
        "@room_framework//sparse_linear_algebra/util:serialize_le",
    ],
)

cc_test(
    name = "top_k_test",
    timeout = "short",
    srcs = [
        "top_k_test.cpp",
    ],
    deps = [
        "//private_knn/applications/knn/two_party_phase:top_k",
        "@googletest//:gtest_main",
        "@mpc_utils//mpc_utils:comm_channel_oblivc_adapter",
        "@mpc_utils//mpc_utils:status_matchers",
        "@mpc_utils//mpc_utils:statusor",
        "@mpc_utils//mpc_utils/testing:comm_channel_test_helper",
        "@mpc_utils//mpc_utils/testing:test_deps",
        "@room_framework//sparse_linear_algebra/util:serialize_le",
    ],
)

genrule(
    name = "gen_test_inputs",
    outs = [
        "test1.csv",
        "test2.csv",
        "test3.csv",
        "test4.csv",
        "test5.csv",
    ],
    cmd = "$(location :test_input_generator) && cp *.csv $(@D)/",
    tools = [":test_input_generator"],
)

py_binary(
    name = "test_input_generator",
    srcs = ["test_input_generator.py"],
    deps = [
        requirement("numpy"),
    ],
)
