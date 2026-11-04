#include "kernel.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include <aie_api/utils.hpp>

void sum_kernel_fn_stream(input_stream<float> * in, output_stream<float> * out, input_stream<int32>* number_elements) {
  float val=0;
  int32 numels=readincr(number_elements);
  int32 raw_addv=readincr(number_elements);

  printf("numels : %d\n", numels);
  printf("raw_addv : %d\n", raw_addv);

  //float add_val=as_float(raw_addv); //or can use *((float*) &(raw_addv))
  float add_val = as_float(raw_addv);
  printf("addvalue : %f\n", add_val);
  aie::vector<float,8> rhs_numbers = aie::broadcast<float,8>(add_val);

  printf("%f %f %f %f %f %f %f %f\n", rhs_numbers[0], rhs_numbers[1], rhs_numbers[2], rhs_numbers[3], rhs_numbers[4], rhs_numbers[5], rhs_numbers[6], rhs_numbers[7]);

  aie::vector<float,8> lhs_numbers;
  for (int i=0;i<numels/2;i++) {
    lhs_numbers.insert(0,readincr_v<4>(in));
    lhs_numbers.insert(1,readincr_v<4>(in));

    aie::vector<float,8> vadd=aie::add(lhs_numbers,rhs_numbers);

    // Printing useful for debugging
    float* print_ptr =(float*)&vadd;
    printf("%f %f %f %f %f %f %f %f\n", print_ptr[0], print_ptr[1], print_ptr[2], print_ptr[3], print_ptr[4], print_ptr[5], print_ptr[6], print_ptr[7]);

    writeincr(out, vadd.extract<4>(0));
    writeincr(out, vadd.extract<4>(1));
  }
}
