
# Projet Pong en 2D

## Description
Ce projet est une implémentation du jeu classique Pong en 2D, développé en C++. Le jeu permet à deux joueurs de s'affronter en utilisant des raquettes pour renvoyer une balle d'un côté à l'autre de l'écran.

## Fonctionnalités
- Mode deux joueurs
- Contrôles de la raquette via le clavier
- Détection des collisions avec les raquettes et les murs
- Système de score
- Fonctionnalité client-serveur pour le jeu en réseau

## Prérequis
- Un compilateur C++ (par exemple, g++)
- CMake pour la gestion de la construction

## Installation
1. Clonez le dépôt :
   ```sh
   git clone https://github.com/votre-utilisateur/votre-repo.git
   ```
2. Accédez au répertoire du projet :
   ```sh
   cd votre-repo
   ```
3. Créez un répertoire de construction et accédez-y :
   ```sh
   mkdir build && cd build
   ```
4. Générez les fichiers de construction avec CMake :
   ```sh
   cmake ..
   ```
5. Compilez le projet :
   ```sh
   make
   ```
## Fonctionnement du serveur et du client
### Serveur
Le serveur est responsable de :
- Gérer les connexions des clients.
- Maintenir l'état du jeu (positions des raquettes, balle, score).
- Envoyer les mises à jour de l'état du jeu aux clients.

### Client
Le client est responsable de :
- Envoyer les commandes de l'utilisateur (mouvements de la raquette) au serveur.
- Recevoir les mises à jour de l'état du jeu depuis le serveur.
- Afficher l'état du jeu à l'utilisateur.

## Structure du projet
- `src/` : Contient les fichiers source du projet.
    - `main.cpp` : Point d'entrée du programme.
    - `pong_server.cpp` : Implémentation du serveur.
    - `pong_client.cpp` : Implémentation du client.
    - `game.cpp` : Logique du jeu.
- `include/` : Contient les fichiers d'en-tête.
    - `game.h` : Déclarations des fonctions et classes du jeu.
- `CMakeLists.txt` : Fichier de configuration pour CMake.
- `README.md` : Ce fichier.
- `LICENSE` : Informations sur la licence.