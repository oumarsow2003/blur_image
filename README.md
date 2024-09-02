# Blur Image

## Description

Ce projet consiste à flouter une image BMP en utilisant une interface graphique simple. L'utilisateur peut charger une image BMP, puis ajuster le niveau de flou à l'aide d'une barre de défilement (scale). Le flou est appliqué en fonction du niveau choisi, offrant un contrôle précis sur la dégradation de l'image.

## Prérequis

Avant de pouvoir utiliser ce projet, vous devez installer les dépendances suivantes à l'aide de `brew` :

- **GTK+ 3** : Une bibliothèque graphique pour créer des interfaces utilisateur.
- **GLib 2.0** : Une bibliothèque de bas niveau utilisée par GTK+ pour des tâches comme la gestion de la mémoire et des threads.
- **pkg-config** : Un outil qui permet de gérer les drapeaux de compilation pour les bibliothèques.

### Installation des dépendances avec `brew`

Si vous n'avez pas encore installé `brew`, suivez les instructions sur le [site officiel de Homebrew](https://brew.sh/).

Une fois `brew` installé, exécutez les commandes suivantes pour installer les dépendances nécessaires :

```bash
brew install gtk+3
brew install glib
brew install pkg-config
