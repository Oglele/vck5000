#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define M_PI  3.14159265358979323846
#define NUM_SAMPLES 128     
#define AMPLITUDE 50.0f   
#define FREQ 5.0f          // Fréquence principale (basse fréquence)
#define FREQ_2 20.0f       // Harmonique / Haute fréquence à filtrer
#define AMPLITUDE_2 20.0f   

// 1. Génération du signal complexe (Sinusoïde 5Hz + Harmonique 20Hz)
void generate_noisy_sine(int32_t *buffer, int size) {
    for (int i = 0; i < size; i++) {
        double x = (double)i / (double)size;
        double y = sin(2.0 * M_PI * x * FREQ);
        double second = sin(2.0 * M_PI * x * FREQ_2);
        int32_t noise = (rand() % 21) - 10; // Bruit aléatoire entre -10 et +10
        buffer[i] = (int32_t)(y * AMPLITUDE) + (int32_t)(second * AMPLITUDE_2) + noise;
    }
}

// 2. Kernel 1 : Lissage / Moyenne glissante simple
void kernel_smooth_sim(const int32_t *input, int32_t *output, int size) {
    for (int i = 0; i < size; i++) {
        int prev = (i > 0) ? input[i - 1] : input[size - 1];
        int curr = input[i];
        int next = (i < size - 1) ? input[i + 1] : input[0];
        output[i] = (prev + curr + next) / 3;
    }
}

// 3. Simulation du Kernel 2 : Filtre FIR Passe-Bas large (15 coefficients)
void kernel_fir_sim(const int32_t *input, int32_t *output, int size) {
    // 15 coefficients symétriques formant une cloche (filtre passe-bas puissant)
    const int coeffs[15] = {1, 2, 4, 7, 10, 13, 15, 16, 15, 13, 10, 7, 4, 2, 1};
    int sum_coeffs = 130; // Somme totale des coefficients pour la normalisation

    for (int i = 0; i < size; i++) {
        int32_t res = 0;
        // Convolution sur une fenêtre de 15 échantillons gérant les bords de manière circulaire
        for (int j = -7; j <= 7; j++) {
            int idx = i + j;
            // Gestion des bords du tableau (boucle circulaire)
            if (idx < 0) idx += size;
            if (idx >= size) idx -= size;

            res += input[idx] * coeffs[j + 7];
        }
        // Normalisation
        output[i] = res / sum_coeffs;
    }
}

int main() {
    srand((unsigned int)time(NULL));

    int32_t input_signal[NUM_SAMPLES];
    int32_t smoothed_signal[NUM_SAMPLES];
    int32_t final_signal[NUM_SAMPLES];

    // Exécution du pipeline
    generate_noisy_sine(input_signal, NUM_SAMPLES);
    kernel_smooth_sim(input_signal, smoothed_signal, NUM_SAMPLES);
    kernel_fir_sim(smoothed_signal, final_signal, NUM_SAMPLES);

    // Écriture des fichiers ASCII (séparés par un espace comme souhaité)
    FILE *f_in = fopen("data/input_signal.txt", "w");
    FILE *f_smooth = fopen("smoothed_signal.txt", "w");
    FILE *f_out = fopen("final_signal.txt", "w");

    FILE *f_config = fopen("data/num_elements.txt", "w");

    if (f_in == NULL || f_smooth == NULL || f_out == NULL || f_config == NULL) {
        perror("Erreur lors de l'ouverture des fichiers");
        return 1;
    }

    fprintf(f_config, "%d\n", NUM_SAMPLES);

    for (int i = 0; i < NUM_SAMPLES; i++) {
        fprintf(f_in, "%d", input_signal[i]);
        fprintf(f_smooth, "%d", smoothed_signal[i]);
        fprintf(f_out, "%d", final_signal[i]);
        if (((i + 1) % 4 == 0) && (i < NUM_SAMPLES - 1)) { // Ajout d'un retour à la ligne tous les 4 échantillons demander par le plio_128_bits = 4 int32_t
            fprintf(f_in, "\n");
            fprintf(f_smooth, "\n");
            fprintf(f_out, "\n");
        } else {
            fprintf(f_in, " ");
            fprintf(f_smooth, " ");
            fprintf(f_out, " ");
        }
    }

    // Fermeture propre des fichiers
    fclose(f_in);
    fclose(f_smooth);
    fclose(f_out);
    fclose(f_config);
    printf("Pipeline exécuté et fichiers ASCII générés avec succès !\n");
    printf("Size of input_signal: %zu bytes type %ld\n", sizeof(input_signal), sizeof(input_signal[0]));
    return 0;
}