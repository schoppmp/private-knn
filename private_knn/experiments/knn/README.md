# Private KNN Experiments
These are the KNN experiments that are mentioned in [the paper](TODO) that is
accompanying this repository. More specifically, these are the experiments of
the first and second phase. In the first phase, oblivc is used to calculate topk results with each server indivdually. The results are then fed into the second phase, which caluclates the global topk. For this phase, mp-spdz is used as it offers, among others, semi-honest n-party MPC.

<!-- You can build docker images via the bazel rules in BUILD. These
will execute one of the contained configs respectively. -->