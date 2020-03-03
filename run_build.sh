docker ps
eval $(opam config env)
cd /private-knn
bazel build //...
bazel run //private_knn/experiments/idf_precomputation:idf_precomputation -- --norun
bazel run //private_knn/experiments/knn/two_party_phase:image -- --norun
bazel run //private_knn/experiments/knn/two_party_phase:non_uniform_sparsity -- --norun
bazel run //private_knn/experiments/knn/two_party_phase:realworld_nonzeros -- --norun
bazel run //private_knn/experiments/knn/multi_party_phase:topk -- --norun
