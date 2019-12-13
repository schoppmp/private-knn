#include <random>
#include <thread>

#include "gtest/gtest.h"
#include "mpc_utils/comm_channel.hpp"
#include "mpc_utils/comm_channel_oblivc_adapter.hpp"
#include "mpc_utils/status_matchers.h"
#include "mpc_utils/testing/comm_channel_test_helper.hpp"
#include "sparse_linear_algebra/util/serialize_le.hpp"

extern "C" {
#include "../top_k.h"
#include "obliv_common.h"
}

namespace private_knn {
namespace applications {
namespace knn {
namespace two_party_phase {
namespace {

class TopKTest : public ::testing::Test {
 protected:
  TopKTest() : helper_(false) {}

  void SetUp() {
    comm_channel *chan0 = helper_.GetChannel(0);
    comm_channel *chan1 = helper_.GetChannel(1);
    memset(&pd_0_, 0, sizeof(ProtocolDesc));
    memset(&pd_1_, 0, sizeof(ProtocolDesc));
    std::thread thread1([this, chan1] {
      ASSERT_OK_AND_ASSIGN(pd_1_, adapter_.Connect(*chan1));
      setCurrentParty(&pd_1_, 2);
    });
    ASSERT_OK_AND_ASSIGN(pd_0_, adapter_.Connect(*chan0));
    setCurrentParty(&pd_0_, 1);
    thread1.join();
  }

  mpc_utils::CommChannelOblivCAdapter adapter_;
  mpc_utils::testing::CommChannelTestHelper helper_;
  ProtocolDesc pd_0_;
  ProtocolDesc pd_1_;
};

TEST_F(TopKTest, Test1) {
  // Define inputs and desired outputs.
  std::vector<uint64_t> inner_products = {10000,  20000,  30000,  40000, 50000, 60000, 70000, 80000, 90000, 100000};
  std::vector<uint64_t> norms_0 = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
  uint64_t norm_1 = 2;
  std::vector<int> top_k_indices = {1, 2, 3, 0, 5};

  // Create secret shares.
  int num_selected(top_k_indices.size());
  int num_elements(inner_products.size());
  std::mt19937 rd(12345);
  std::uniform_int_distribution<uint64_t> dist;
  std::vector<uint64_t> inner_products_0(num_elements);
  std::vector<uint64_t> inner_products_1(num_elements);
  for (int i = 0; i < num_elements; i++) {
    inner_products_0[i] = dist(rd);
    inner_products_1[i] = inner_products[i] - inner_products_0[i];
  }

  std::thread thread1([this, num_elements, num_selected, &norm_1, &inner_products_1] {
    std::vector<uint8_t> inner_products_1_serialized(sizeof(uint64_t) * num_elements);
    std::vector<uint8_t> norm_1_serialized(sizeof(uint64_t) * num_elements);
    std::vector<int> result(num_selected);
    serialize_le(inner_products_1_serialized.begin(), inner_products_1.begin(), num_elements);
    serialize_le(norm_1_serialized.begin(), &norm_1, 1);
    knn_oblivc_args args = {
        num_elements: num_elements,
        num_selected: num_selected,
        value_size: sizeof(uint64_t),
        serialized_inputs: inner_products_1_serialized.data(),
        serialized_norms: norm_1_serialized.data(),
        result: result.data(),
    };
    execYaoProtocol(&pd_1_, top_k_oblivc, &args);
    cleanupProtocol(&pd_1_);
  });

  std::vector<uint8_t> inner_products_0_serialized(sizeof(uint64_t) * num_elements);
  std::vector<uint8_t> norms_0_serialized(sizeof(uint64_t) * num_elements);
  std::vector<int> result(num_selected);
  serialize_le(inner_products_0_serialized.begin(), inner_products_0.begin(), num_elements);
  serialize_le(norms_0_serialized.begin(), norms_0.begin(), num_elements);
  knn_oblivc_args args = {
      num_elements: num_elements,
      num_selected: num_selected,
      value_size: sizeof(uint64_t),
      serialized_inputs: inner_products_0_serialized.data(),
      serialized_norms: norms_0_serialized.data(),
      result: result.data(),
  };
  execYaoProtocol(&pd_0_, top_k_oblivc, &args);
  cleanupProtocol(&pd_0_);
  thread1.join();
  EXPECT_EQ(top_k_indices, result);
}

}  // namespace
}  // namespace two_party_phase
}  // namespace knn
}  // namespace applications
}  // namespace private_knn