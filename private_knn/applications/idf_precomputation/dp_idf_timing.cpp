#include <boost/range/algorithm/sort.hpp>
#include <cmath>
#include <limits>

#include "absl/strings/str_cat.h"
#include "mpc_utils/benchmarker.hpp"
#include "mpc_utils/comm_channel.hpp"
#include "mpc_utils/comm_channel_oblivc_adapter.hpp"
#include "mpc_utils/mpc_config.hpp"
#include "mpc_utils/party.hpp"
#include "private_knn/util/get_ceil.hpp"
extern "C" {
#include "obliv.h"
#include "private_knn/applications/idf_precomputation/dp_idf.h"
}

namespace private_knn {
namespace applications {
namespace idf_precomputation {

class dp_idf_config : public virtual mpc_config {
 protected:
  void validate() {
    namespace po = boost::program_options;
    if (party_id < 0 || party_id > 1) {
      BOOST_THROW_EXCEPTION(po::error("'party' must be 0 or 1"));
    }
    if (vocabulary_size.size() == 0) {
      BOOST_THROW_EXCEPTION(
          po::error("'vocabulary_size' must be passed at least once"));
    }
    if (num_samples.size() == 0) {
      BOOST_THROW_EXCEPTION(
          po::error("'num_samples' must be passed at least once"));
    }
    for (auto V : vocabulary_size) {
      if (V <= 0) {
        BOOST_THROW_EXCEPTION(po::error("'vocabulary_size' must be positive"));
      }
    }
    for (auto L : num_samples) {
      if (L <= 0) {
        BOOST_THROW_EXCEPTION(po::error("'num_samples' must be positive"));
      }
    }
    if (vocabulary_size < num_samples) {
      BOOST_THROW_EXCEPTION(
          po::error("'vocabulary_size' must be at least 'num_samples'"));
    }
    if (epsilon <= 0) {
      BOOST_THROW_EXCEPTION(po::error("'epsilon' must be positive"));
    }
    mpc_config::validate();
  }

 public:
  std::vector<ssize_t> vocabulary_size;
  std::vector<ssize_t> num_samples;
  ssize_t max_runs;
  double epsilon;

  dp_idf_config() {
    namespace po = boost::program_options;
    add_options()("vocabulary_size,V", po::value(&vocabulary_size)->composing(),
                  "Number of words in the vocabulary")(
        "num_samples,L", po::value(&num_samples)->composing(),
        "Number words to sample")(
        "max_runs", po::value(&max_runs)->default_value(-1),
        "Maximum number of runs. Default is unlimited")(
        "epsilon,e", po::value(&epsilon)->required(), "Privacy budget");
    set_default_filename("config/experiments/dp_idf_timing.ini");
  }
};

}  // namespace idf_precomputation
}  // namespace applications
}  // namespace private_knn

int main(int argc, const char *argv[]) {
  using private_knn::applications::idf_precomputation::dp_idf_config;
  // parse config
  dp_idf_config conf;
  try {
    conf.parse(argc, argv);
  } catch (boost::program_options::error &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  // connect to other party
  party p(conf);
  auto channel = p.connect_to(1 - p.get_id());

  size_t num_experiments =
      std::max({conf.vocabulary_size.size(), conf.num_samples.size()});

  for (size_t num_runs = 0; conf.max_runs < 0 || num_runs < conf.max_runs;
       num_runs++) {
    std::cout << "Run " << num_runs << "\n";
    for (size_t experiment = 0; experiment < num_experiments; experiment++) {
      mpc_utils::Benchmarker benchmarker;
      // run yao's protocol using Obliv-C
      mpc_utils::Benchmarker::MaybeBenchmarkFunction(
          &benchmarker, "dp_idf_timing", [&] {
            size_t vocabulary_size = get_ceil(conf.vocabulary_size, experiment);
            size_t num_samples = get_ceil(conf.num_samples, experiment);
            std::cout << "vocabulary_size = " << vocabulary_size
                      << "\nnum_samples = " << num_samples << "\n";
            std::vector<double> probabilities(vocabulary_size, 0);
            double count_max = 1000., epsilon = conf.epsilon, normalizer = 0.;
            for (size_t i = 0; i < vocabulary_size; i++) {
              probabilities[i] =
                  std::exp(count_max / (i + 1) * epsilon / (2 * num_samples));
              // sum += probabilities[i];
              // std::cout << probabilities[i] <<"\n";
            }
            std::vector<count_t> input(vocabulary_size);
            normalizer =
                (std::numeric_limits<count_t>::max() / vocabulary_size) /
                probabilities[0];
            for (size_t i = 0; i < vocabulary_size; i++) {
              probabilities[i] *= normalizer;
              input[i] = probabilities[i];
            }
            std::vector<size_t> result_indexes(num_samples);
            std::vector<count_t> result_values(num_samples);
            dp_idf_oblivc_args args = {.vocabulary_size = vocabulary_size,
                                       .counts = input.data(),
                                       .num_results = num_samples,
                                       .result_indexes = result_indexes.data(),
                                       .result_values = result_values.data()};

            auto status = mpc_utils::CommChannelOblivCAdapter::Connect(
                channel, /*sleep_time=*/10);
            if (!status.ok()) {
              std::string error =
                  absl::StrCat("dp_idf_timing: connection failed: ",
                               status.status().message());
              BOOST_THROW_EXCEPTION(std::runtime_error(error));
            }
            ProtocolDesc pd = status.ValueOrDie();
            setCurrentParty(&pd, p.get_id() + 1);
            execYaoProtocol(&pd, dp_idf_oblivc, &args);
            cleanupProtocol(&pd);

            // boost::sort(result_indexes);
            std::cout << "result: ";
            for (size_t i = 0; i < num_samples; i++) {
              std::cout << result_indexes[i] << " ";
            }
            // std::cout << "\n";
            // benchmarker.AddSecondsSinceStart("total_time:", start);
            // std::cout << "total_time: " << benchmarker.Get("total_time")
            //           << "\n";
          });
    }
  }

  return 0;
}
