#ifndef LISSAGE_KERNEL_H
#define LISSAGE_KERNEL_H

#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"

void lissage_kernel_fn_stream(input_stream<int32_t>*, output_stream<int32_t>*, input_stream<int32_t>*);

#endif
