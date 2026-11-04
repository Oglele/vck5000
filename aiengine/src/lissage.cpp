#include "lissage.h"
#include "aie_api/aie.hpp"
#include "aie_api/aie_adf.hpp"
#include <aie_api/utils.hpp>

void lissage_kernel_fn_stream(input_stream<int32> *in, output_stream<int32> *out, input_stream<int32> *number_elements)
{

  int32 numels = readincr(number_elements);

  printf("nombre d'elements : %d\n", numels);

  aie::vector<int32, 16> curr_vec;
  int32 last_val = 0;

  for (int i = 0; i < numels / 16; i++)
  {

    curr_vec = readincr_v<16>(in);

    // 1. Voisin de gauche (Précédent) avec shuffle_up_fill
    // On décale vers le haut de 1 position et on remplit la case vide à gauche avec le dernier élément du bloc précédent
    aie::vector<int32, 16> fill_left = aie::broadcast<int32, 16>(last_val);
    aie::vector<int32, 16> left_vec = aie::shuffle_up_fill(curr_vec, fill_left, 1);

    // 2. Voisin de droite (Suivant) avec shuffle_down_fill
    // On décale vers le bas de 1 position et on remplit la case vide à droite avec le premier élément de la suite
    aie::vector<int32, 16> fill_right = aie::broadcast<int32, 16>(curr_vec[15]);
    aie::vector<int32, 16> right_vec = aie::shuffle_down_fill(curr_vec, fill_right, 1);

    last_val = curr_vec[15]; // Mise à jour de last_val pour le prochain bloc

    // 3. Addition vectorielle simultanée des 3 flux (Gauche + Courant + Droite) en 1 seul cycle SIMD
    aie::vector<int32, 16> sum_vec = aie::add(aie::add(left_vec, curr_vec), right_vec);

    // 4. Division par 3 pour obtenir la moyenne
    aie::vector<int32, 16> output_vec;
    for (int j = 0; j < 16; j++)
    {
      output_vec[j] = sum_vec[j] / 3;
    }

    writeincr(out, output_vec);
  }
}
