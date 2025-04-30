import pandas as pd
import matplotlib.pyplot as plt

# Charger les résultats
df = pd.read_csv("benchmark_scaling.csv")

# Regrouper par méthode
methods = df["method"].unique()

# Tracer la courbe de performance pour chaque méthode
plt.figure(figsize=(10, 6))

for method in methods:
    subset = df[df["method"] == method]
    plt.plot(subset["num_threads"], subset["time_s"], marker='o', label=method)

plt.title("Temps d'exécution en fonction du nombre de threads")
plt.xlabel("Nombre de threads")
plt.ylabel("Temps (secondes)")
plt.grid(True, linestyle="--", alpha=0.5)
plt.legend()
plt.tight_layout()
plt.savefig("scaling_plot.png")
plt.show()