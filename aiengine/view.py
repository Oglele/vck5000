import matplotlib.pyplot as plt

def plot_signals_subplots():
    # Fonction de lecture flexible pour fichiers ASCII
    def read_signal(filename):
        with open(filename, "r") as f:
            content = f.read()
            return [int(val) for val in content.split() if val.strip()]

    # 1. Chargement des données
    input_signal = read_signal("input_signal.txt")
    smoothed_signal = read_signal("smoothed_signal.txt")
    final_signal = read_signal("final_signal.txt")

    indices = list(range(len(input_signal)))

    # 2. Création de la figure avec 3 sous-graphiques (3 lignes, 1 colonne)
    fig, axes = plt.subplots(3, 1, figsize=(10, 8), sharex=True)

    # Subplot 1 : Signal d'entrée bruité
    axes[0].plot(indices, input_signal, 'r-o', markersize=3, label="Entrée")
    axes[0].set_title("1. Signal d'entrée (Sinusoïde bruitée)")
    axes[0].set_ylabel("Valeur")
    axes[0].grid(True, linestyle="--", alpha=0.6)
    axes[0].legend(loc="upper right")

    # Subplot 2 : Kernel 1 (ReLU / Redressement)
    axes[1].plot(indices, smoothed_signal, 'b-s', markersize=3, label="Kernel 1")
    axes[1].set_title("2. Kernel 1 : Redressement (Partie négative coupée à 0)")
    axes[1].set_ylabel("Valeur")
    axes[1].grid(True, linestyle="--", alpha=0.6)
    axes[1].legend(loc="upper right")

    # Subplot 3 : Kernel 2 (Inversion & Amplification)
    axes[2].plot(indices, final_signal, 'g-^', markersize=3, label="Kernel 2")
    axes[2].set_title("3. Kernel 2 : Inversion et Amplification finale")
    axes[2].set_xlabel("Index de l'échantillon")
    axes[2].set_ylabel("Valeur")
    axes[2].grid(True, linestyle="--", alpha=0.6)
    axes[2].legend(loc="upper right")

    # Ajustement de l'espacement pour éviter les chevauchements
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    plot_signals_subplots()