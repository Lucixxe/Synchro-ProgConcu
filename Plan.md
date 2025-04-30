# 4 Idées de projet 

## Visualisation des solutions de synchronisation
- Créer un simulateur interactif qui montre comment les algorithmes de synchronisation fonctionnent en temps réel.
- Représenter graphiquement les files d’attente, les sections critiques et l'accès aux ressources partagées.
- Utiliser une interface web ou une application desktop serait le plus adapté.

## Comparaison des performances
- Mettre en place des **benchmarks** entre différentes solutions de synchronisation (**mutex, sémaphores, etc.**).
- Tester l’efficacité sur différentes charges (**nombre croissant de threads/processus**).
- Évaluer l’impact sur l’utilisation du **CPU** et la **latence**.

## Intégration matérielle
- Expérimenter des solutions hybrides combinant du logiciel et du matériel (ex : utiliser des **instructions spécifiques** à certaines architectures comme les **verrous atomiques** des processeurs modernes --> Pas de changement de contexte, ne nécéssitent pas d'appels systèmes, intégré en C, C++ et Rust).
- Tester l’impact des **architectures multi-cœurs** sur la synchronisation.

## Mise en pratique avec un cas concret
- Appliquer ces concepts à un **mini-projet** comme une **implémentation simplifiée** d’un système de transaction concurrente (ex : **mini-blockchain** ou **base de données en mémoire**).
- Tester les solutions dans un **environnement distribué** (via **Docker ou Kubernetes**) pour simuler une charge réseau.


# Organisation du travail

### Recherche bibliographique
- Comprendre les concepts de base et les **algorithmes** de synchronisation (ça sera plus une révision qu'autre chose).

### Développement
- Implémenter et tester les solutions.

### Visualisation/benchmarking
- Comparer les performances et documenter les résultats.

### Rédaction de la documentation
- Expliquer les **choix techniques** et les **conclusions** (faire une doc complète de notre logiciel en gros).
