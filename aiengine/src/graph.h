#include <adf.h>
#include "lissage.h"

using namespace adf;

class simpleGraph : public graph {
private:
  kernel lissage_kernel;
public:
  input_plio in, num_elements;
  output_plio out;

  simpleGraph(){
    lissage_kernel = kernel::create(lissage_kernel_fn_stream);

    in = input_plio::create("s0", plio_128_bits, "data/input_signal.txt");
    num_elements  = input_plio::create("numels", plio_128_bits, "data/num_elements.txt");
    out = output_plio::create("s1", plio_128_bits, "data/output.txt");

    connect<stream>(in.out[0], lissage_kernel.in[0]);
    connect<stream>(num_elements.out[0], lissage_kernel.in[1]);
    connect<stream>(lissage_kernel.out[0], out.in[0]);

    source(lissage_kernel) = "lissage.cpp";

    runtime<ratio>(lissage_kernel) = 0.9;
  }
};
