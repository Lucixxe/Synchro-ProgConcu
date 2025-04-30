import pandas as pd
import matplotlib.pyplot as plt

# Charger le fichier CSV
df = pd.read_csv("benchmark_scaling.csv")

# Créer le graphe
plt.figure(figsize=(10, 6))

# Identifier toutes les méthodes
methods = df["method"].unique()

# Calculer et tracer le speedup par méthode
for method in methods:
    sub = df[df["method"] == method].sort_values("num_threads")
    base_time = sub[sub["num_threads"] == 1]["time_s"].values[0]
    speedup = base_time / sub["time_s"]
    plt.plot(sub["num_threads"], speedup, marker='o', label=method)

plt.title("Speedup par méthode en fonction du nombre de threads")
plt.xlabel("Nombre de threads")
plt.ylabel("Speedup (T1 / Tn)")
plt.grid(True, linestyle='--', alpha=0.5)
plt.legend()
plt.tight_layout()
plt.savefig("speedup_plot.png")
plt.show()
