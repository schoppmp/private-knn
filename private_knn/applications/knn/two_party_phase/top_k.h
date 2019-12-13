#pragma once
#include <stdint.h>

typedef struct {
  const int num_elements;
  const int num_selected;
  const int value_size;
  const uint8_t* serialized_inputs;
  // Party 0 has `num_elements` norms, party 1 only one.
  const uint8_t* serialized_norms;
  int* result;
} knn_oblivc_args;

void top_k_oblivc(void* vargs);
