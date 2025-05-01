# 🔧 Synchro-ProgConcu

Projet de benchmark et comparaison de différentes méthodes de synchronisation en C++ multithread dans le cadre de l'UE "Histoire de l'Informatique".

---

## 🧠 Objectif

Ce programme compare les performances (temps d'exécution, scalabilité, correction) de plusieurs stratégies de synchronisation :
- 🔒 Mutex (`std::mutex`)
- 🔁 Spinlock (basé sur `atomic_flag`)
- 🧩 CAS (lock-free avec `compare_exchange_weak`)
- 🚫 Naive (incrémentation sans synchronisation, pour référence)

Chaque méthode est testée dans deux scénarios :
- Variation du nombre de **threads**,
- Variation du nombre de **cycles (itérations)**.

Une vérification automatique est effectuée pour détecter les erreurs liées aux conditions de course.

---

## ▶️ Compilation

```bash
g++ -std=c++17 sync_benchmark.cpp -o benchmark.exe -pthread
```

---

## 🚀 Exécution

Le programme propose **2 modes** :

### 🔁 Mode 1 : Scalabilité (threads variables)

```bash
./benchmark.exe threads <iterations_par_thread>
```

Exemple :

```bash
./benchmark.exe threads 500000
```

➡️ Fixe 500 000 itérations par thread  
➡️ Threads testés : 1, 2, 4, 8, 16  
➡️ Résultats dans `benchmark_scaling.csv`

---

### 🔁 Mode 2 : Charge variable (cycles)

```bash
./benchmark.exe iterations <nombre_de_threads>
```

Exemple :

```bash
./benchmark.exe iterations 8
```

➡️ Threads fixés à 8  
➡️ Cycles testés : 100k, 500k, 1M, 5M, 10M  
➡️ Résultats dans `benchmark_iterations.csv`

---

## ⚠️ Vérification de correction

Chaque méthode est automatiquement validée à l'exécution :
- ✅ Si `counter final == nb_threads × itérations`
- ❌ Sinon → un message d'erreur s'affiche dans la console et la méthode est marquée comme incorrecte dans le CSV.

---

## 📊 Visualisation

Trois scripts Python sont fournis pour générer des graphiques à partir des fichiers CSV.

### 1. `plot_scaling.py`

➡️ Affiche le **temps d'exécution** selon le nombre de threads

```bash
python plot_scaling.py
```

🖼️ Génère `scaling_plot.png`

---

### 2. `plot_speedup.py`

➡️ Calcule et affiche le **speedup = T(1) / T(n)** pour chaque méthode

```bash
python plot_speedup.py
```

🖼️ Génère `speedup_plot.png`

---

### 3. `plot_iterations.py`

➡️ Affiche l'impact du **nombre d'itérations** sur le temps d'exécution pour un thread fixé

```bash
python plot_iterations.py
```

🖼️ Génère `iterations_plot.png`

---

## 📁 Fichiers générés

| Fichier | Description |
|--------|-------------|
| `benchmark_scaling.csv` | Résultats pour différentes tailles de thread |
| `benchmark_iterations.csv` | Résultats pour différentes tailles de charge |
| `scaling_plot.png` | Temps vs threads |
| `speedup_plot.png` | Speedup vs threads |
| `iterations_plot.png` | Temps vs itérations |

Chaque CSV contient aussi une colonne `correct` indiquant si le résultat est fiable (`1`) ou non (`0`).

---

## 📚 Références

Ce projet repose sur une synthèse théorique réalisée dans le cadre d’un travail universitaire sur :
- L’évolution historique des méthodes de synchronisation,
- Les différences entre approches logicielles et matérielles,
- Les notions de cohérence mémoire, sections critiques, structures de synchronisation,
- Les critères de performance : temps, scalabilité, correction et simplicité.
