#pragma once

typedef float count_t;

typedef struct {
  size_t vocabulary_size;
  const count_t *counts;
  size_t num_results;
  size_t *result_indexes;
  count_t *result_values;
} dp_idf_oblivc_args;

void dp_idf_oblivc(void *args);
