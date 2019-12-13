#pragma once
#include <random>

#include "private_knn/applications/knn/two_party_phase/knn_protocol.hpp"
#include "sparse_linear_algebra/util/get_ceil.hpp"
#include "sparse_linear_algebra/util/randomize_matrix.hpp"
#include "sparse_linear_algebra/util/reservoir_sampling.hpp"
#include "sparse_linear_algebra/util/time.h"

namespace private_knn {
namespace experiments {
namespace knn {
namespace two_party_phase {

using private_knn::applications::knn::two_party_phase::MulType;
using private_knn::applications::knn::two_party_phase::PirType;

class KNNConfig : public virtual mpc_config {
 protected:
  void validate();

 public:
  std::map<std::string, PirType> pir_type_converison_table{
      {"basic", PirType::basic},
      {"poly", PirType::poly},
      {"scs", PirType::scs},
  };
  std::map<std::string, MulType> mul_type_converison_table{
      {"dense", MulType::dense},
      {"sparse", MulType::sparse},
  };

  std::vector<ssize_t> num_documents;
  std::vector<ssize_t> num_selected;
  std::vector<ssize_t> chunk_size;
  std::vector<ssize_t> vocabulary_size;
  std::vector<ssize_t> nonzeros_server;
  std::vector<ssize_t> nonzeros_client;
  std::vector<std::string> multiplication_types_raw;
  std::vector<std::string> pir_types_raw;
  std::vector<MulType> multiplication_types;
  std::vector<PirType> pir_types;

  int16_t statistical_security;
  ssize_t max_runs;
  KNNConfig();
  bool measure_communication;
};
}  // namespace two_party_phase
}  // namespace knn
}  // namespace experiments
}  // namespace private_knn