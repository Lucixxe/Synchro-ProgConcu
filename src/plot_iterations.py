import pandas as pd
import matplotlib.pyplot as plt

# Charger le fichier CSV
df = pd.read_csv("benchmark_iterations.csv")

# Tracer
plt.figure(figsize=(10, 6))

methods = df["method"].unique()

for method in methods:
    subset = df[df["method"] == method].sort_values("iterations")
    plt.plot(subset["iterations"], subset["time_s"], marker="o", label=method)

plt.title("Temps d'exécution selon le nombre d'itérations")
plt.xlabel("Nombre d'itérations par thread")
plt.ylabel("Temps (secondes)")
plt.xscale("log")  # échelle logarithmique pour mieux voir la tendance
plt.grid(True, linestyle="--", alpha=0.5)
plt.legend()
plt.tight_layout()
plt.savefig("iterations_plot.png")
plt.show()