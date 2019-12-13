load("@py_deps//:requirements.bzl", "requirement")

cc_test(
    name = "test",
    timeout = "short",
    srcs = ["top_k_isolated.cpp"],
    data = [
        ":test1.csv",
        ":test2.csv",
        ":test3.csv",
        ":test4.csv",
        ":test5.csv",
    ],
    deps = [
        "//private_knn/applications/knn/two_party_phase:knn",
        "@boost//:tokenizer",
        "@googletest//:gtest_main",
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
        requirement("numpy")
    ]
)
