import pandas as pd
import matplotlib.pyplot as plt

# Charger les données
df = pd.read_csv("benchmark.csv")

# Afficher les données brutes (optionnel)
print(df)

# Tracer le temps d'exécution par méthode
plt.figure(figsize=(10, 6))
plt.bar(df["method"], df["time_s"], color="steelblue")
plt.xlabel("Méthode de synchronisation")
plt.ylabel("Temps (secondes)")
plt.title("Comparaison des temps d'exécution par méthode")
plt.xticks(rotation=15)
plt.tight_layout()
plt.grid(True, axis='y', linestyle='--', alpha=0.7)
plt.savefig("benchmark_plot.png")
plt.show()