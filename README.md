# 🔧 Synchro-ProgConcu

Projet de benchmark et comparaison de différentes méthodes de synchronisation en C++ multithread.

## 🧠 Objectif

Ce programme compare les performances (temps d'exécution, scalabilité) de plusieurs stratégies de synchronisation :
- Mutex (`std::mutex`)
- Spinlock (à base de `atomic_flag`)
- CAS (lock-free `compare_exchange_weak`)
- Incrémentation naïve (sans synchronisation, pour référence)

Les résultats sont mesurés pour différents nombres de threads et exportés dans un fichier CSV.

---

## ▶️ Utilisation

### 🔨 Compilation

```bash
g++ -std=c++17 sync_benchmark.cpp -o benchmark.exe -pthread
```

### 🚀 Exécution

Benchmark avec **X itérations par thread** (valeur par défaut : 1 million) :

```bash
./benchmark.exe 500000
```

Cela génère automatiquement :
- `benchmark_scaling.csv` : résultats de benchmark pour {1, 2, 4, 8, 16} threads

---

## 📊 Visualisation

Deux scripts Python sont fournis :

### 1. `plot_scaling.py`

Trace le **temps d'exécution** selon le nombre de threads.

```bash
python plot_scaling.py
```

→ génère `scaling_plot.png`

### 2. `plot_speedup.py`

Trace le **speedup** (accélération) de chaque méthode.

```bash
python plot_speedup.py
```

→ génère `speedup_plot.png`

---

## 📁 Fichiers générés

- `benchmark_scaling.csv` : données brutes
- `scaling_plot.png` : temps d'exécution
- `speedup_plot.png` : accélération

---

## 📚 Références

Ce projet s’appuie sur une synthèse théorique réalisée dans le cadre d’un travail universitaire sur :
- Les primitives de synchronisation logicielle & matérielle,
- Leur évolution historique,
- Les critères de performance, correction, et scalabilité.
