#include <stdint.h>

#include <boost/tokenizer.hpp>
#include <fstream>

#include "gtest/gtest.h"
#include "sparse_linear_algebra/util/serialize_le.hpp"

extern "C" {
#include "obliv.h"
#include "private_knn/applications/knn/two_party_phase/top_k.h"
}

namespace test {
namespace knn {
namespace two_party_phase {

const char* remote_host = "localhost";
// const char *port = "10312";

// const int num_selected = 2;
// const int input_len = 4;
// static uint64_t client_vals[] = {55, 10, 200, 30};
// static uint64_t client_norms[] = {1, 1, 1, 1};
// static uint64_t server_vals[] = {150, 300, 1, 10};
// static uint64_t server_norm[] = {1, 1, 1, 1};

// static int client_output[] = {0, 0};
// static int server_output[] = {0, 0};

struct ThreadInput {
  char* port;
  size_t num_selected;
  size_t input_len;
  std::vector<uint8_t> server_values;
  std::vector<uint8_t> client_values;
  std::vector<uint8_t> server_norm;
  std::vector<uint8_t> client_norms;
  std::vector<uint64_t> expected_indices;
};

// std::vector<uint8_t> serialize(std::vector<uint64_t> input) {
//   std::vector<uint8_t> output(input.size() * sizeof(uint64_t), 0);
//   memcpy(output.data(), input.data(), input.size() * sizeof(uint64_t));
//   for (size_t i = 0; i < output.size(); i++) {
//     std::cout << static_cast<int>(output[i]) << std::endl;
//     //ASSERT_EQ(received_indices[i], (int) ti->expected_indices[i]) << "Index
//     at position " << i << " did not match on server side.";
//   }
//   return output;
// }
std::vector<uint8_t> Serialize(std::vector<uint64_t> input) {
  std::vector<uint8_t> output(input.size() * sizeof(uint64_t), 0);
  serialize_le(output.begin(), input.begin(), input.size());
  return output;
}

std::vector<uint64_t> ReadCsvLine(std::string line) {
  using boost::tokenizer;
  using boost::escaped_list_separator;
  std::vector<uint64_t> vec;

  tokenizer<escaped_list_separator<char>> tk(
      line, escaped_list_separator<char>('\\', ',', '\"'));

  for (tokenizer<escaped_list_separator<char>>::iterator i(tk.begin());
       i != tk.end(); ++i) {
    vec.push_back(strtoul(i->c_str(), nullptr, 10));
  }
  return vec;
}

ThreadInput ReadCsv(std::string path) {
  std::ifstream in(path);
  if (!in.is_open()) {
    std::cout << "File \"" << path << "\" not found";
    exit(1);
  }

  ThreadInput result;

  std::string line;
  std::getline(in, line);
  auto server_values = ReadCsvLine(line);
  result.server_values = Serialize(server_values);
  std::getline(in, line);
  result.client_values = Serialize(ReadCsvLine(line));
  std::getline(in, line);
  result.server_norm = Serialize(ReadCsvLine(line));
  std::getline(in, line);
  result.client_norms = Serialize(ReadCsvLine(line));
  std::getline(in, line);
  result.expected_indices = ReadCsvLine(line);

  result.input_len = server_values.size();
  result.num_selected = result.expected_indices.size();

  return result;
}

void _Server(ThreadInput* ti) {
  ProtocolDesc pd;

  int* received_indices = (int*)calloc(ti->num_selected, sizeof(int));

  knn_oblivc_args args = {ti->input_len,          ti->num_selected,
                          sizeof(uint64_t),       ti->server_values.data(),
                          ti->server_norm.data(), received_indices};

  ASSERT_EQ(protocolAcceptTcp2P(&pd, ti->port), 0)
      << "Sever could not create tcp socket";

  setCurrentParty(&pd, 2);
  execYaoProtocol(&pd, top_k_oblivc, &args);

  cleanupProtocol(&pd);

  for (size_t i = 0; i < ti->num_selected; i++) {
    // std::cout << "client " << i << ": " << args.result[i] << std::endl;
    ASSERT_EQ(received_indices[i], (int)ti->expected_indices[i])
        << "Index at position " << i << " did not match on server side.";
  }

}

void* Server(void* arg) {
  _Server((ThreadInput*)arg);
  return 0;
}

void _Client(ThreadInput* ti) {
  ProtocolDesc pd;

  int* received_indices = (int*)calloc(ti->num_selected, sizeof(int));

  knn_oblivc_args args = {ti->input_len,           ti->num_selected,
                          sizeof(uint64_t),        ti->client_values.data(),
                          ti->client_norms.data(), received_indices};

  ASSERT_EQ(protocolConnectTcp2P(&pd, remote_host, ti->port), 0)
      << "Client could not connect to server";

  setCurrentParty(&pd, 1);
  execYaoProtocol(&pd, top_k_oblivc, &args);

  cleanupProtocol(&pd);

  for (size_t i = 0; i < ti->num_selected; i++) {
    // std::cout << "client " << i << ": " << args.result[i] << std::endl;
    ASSERT_EQ(received_indices[i], (int)ti->expected_indices[i])
        << "Index at position " << i << " did not match on client side.";
  }
}

void* Client(void* arg) {
  _Client((ThreadInput*)arg);
  return 0;
}

void CreateTest(std::string filename, char* port) {
  std::string base("private_knn/applications/knn/two_party_phase/top_k_test/");

  base += filename;

  ThreadInput ti = ReadCsv(base);

  // TODO use 0 port to get a free one and give this information to the client
  // somehow
  ti.port = port;

  pthread_t mThreadID1, mThreadID2;
  ::pthread_create(&mThreadID1, nullptr, Server, &ti);
  ::sleep(1);
  ::pthread_create(&mThreadID2, nullptr, Client, &ti);
  ::pthread_join(mThreadID1, nullptr);
  ::pthread_join(mThreadID2, nullptr);
}

TEST(topk_two_party_phase, CompleteTest1) {
  CreateTest(std::string("test1.csv"), "11337");
}

// TEST(topk_two_party_phase, CompleteTest2) {
//   CreateTest(std::string("test2.csv"), "11338");
// }

// TEST(topk_two_party_phase, CompleteTest3) {
//   CreateTest(std::string("test3.csv"), "11339");
// }

// TEST(topk_two_party_phase, CompleteTest4) {
//   CreateTest(std::string("test4.csv"), "11340");
// }

// TEST(topk_two_party_phase, CompleteTest5) {
//   CreateTest(std::string("test5.csv"), "11341");
// }

} // namespace two_party_phase
} // namespace knn
} // namespace test