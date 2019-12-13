from numpy.random import seed
from numpy.random import randint
import numpy as np
import csv

UINT64_MIN = 0
UINT64_MAX = 18446744073709551615

def create_test_case(fpath, seed_val, num_elements, num_selected, min_val, max_val, min_val_norms, max_val_norms):
    seed(seed_val)

    # generate some uint64 values
    values = randint(min_val, max_val, num_elements, dtype=np.uint64)

    # generate shares
    share_values_server = randint(min_val, np.min(values), num_elements, dtype=np.uint64)
    share_values_client = values - share_values_server

    norm_server = randint(min_val_norms, max_val_norms, 1, dtype=np.uint64)
    norms_client = randint(min_val_norms, max_val_norms, num_elements, dtype=np.uint64)

    # sanity check
    assert(np.array_equal(values, share_values_server + share_values_client))

    # calculate wanted result
    wanted_result = values // norms_client
    wanted_result = wanted_result // norm_server[0]

    topk_indices = wanted_result.argsort()[-num_selected:][::-1]

    with open(fpath, "w") as f:
        w = csv.writer(f)
        #w.writerow([num_elements, num_selected])
        w.writerow(share_values_server)
        w.writerow(share_values_client)
        w.writerow(norm_server)
        w.writerow(norms_client)
        w.writerow(topk_indices)

create_test_case("test1.csv", 1337, 20, 10, 10, 200, 1, 4)
create_test_case("test2.csv", 1338, 1000, 20, 10, 200000, 1, 10)
create_test_case("test3.csv", 1339, 300, 300, 10, UINT64_MAX, 1, 100)
create_test_case("test4.csv", 1340, 1, 1, 10, UINT64_MAX, 1, 10)
create_test_case("test5.csv", 1341, 100000, 10, 10, UINT64_MAX, 1, 4)
