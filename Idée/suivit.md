# 1) Prototype

conception d'un prototype client server simple (deplacmeent de cube) afin d'avoir une base de depart

# 2) Conception de l'architecture du server

## 2.1) Mise en place de la structure
    
mise en place de thread
diagramme de classe minimale + transformation du prototype
debut de plateau

## 2.2) Mise en place du jeu

### 2.2.1) Boucle de jeu

role
pacgomme
timer
eat

### 2.2.2) Plateau

L’algorithme utilisé ici est Prim pour labyrinthe (une variante de l’algorithme de Prim pour graphe).

Principe :

On part d’une case initiale (ici (1,1)).

On marque cette case comme « Floor ».

On ajoute les murs voisins dans une liste frontier.

Tant que la frontier n’est pas vide :

On choisit un mur aléatoire dans la frontier.

Si ce mur a au moins un voisin déjà « Floor », on casse le mur entre eux.

On marque ce mur comme « Floor ».

On ajoute ses voisins encore murs à la frontier.


Ensuite, la fonction placeHut() place une cabane 3×3 au centre et protège ses murs avec isHutWall() pour que l’algorithme de Prim ne touche pas à cette zone.

connectHut() : garantit qu’il y a au moins une sortie de la cabane vers le labyrinthe existant.

Puis on s'assure que les coins du plateau sont accessibles pour les joueurs et on ouvre éventuellement des murs adjacents pour relier les coins à l’intérieur.

Après on parcourt toutes les cellules et ajoute des passages aléatoires pour créer des cycles.

On supprime aussi les cul-de-sacs en cassant aléatoirement un mur adjacent.

Il y a egalement des ouverture dans le plateau qui servent de portails

### 2.2.3) Bot

L’intelligence artificielle des fantômes repose sur une architecture hybride combinant :

- Pathfinding classique
- Système d’influence inspiré du comportement des colonies de fourmis
- Prise de décision hiérarchique

L’objectif est d’obtenir un comportement à la fois réactif, cohérent et émergent.

Le système fonctionne par priorités :

Priorité 1 — Fuite stratégique
Si Pac-Man est en mode chasseur :
- Recherche du plus court chemin vers la cabane centrale
- Utilisation de **Breadth First Search (BFS)** pour obtenir un chemin optimal non pondéré

Priorité 2 — Poursuite ciblée
Si Pac-Man est détecté dans un rayon de vision :
- BFS à profondeur limitée
- Sélection du voisin rapprochant le plus rapidement de la cible

Priorité 3 — Comportement autonome
En absence de menace ou cible visible :
- Choix du meilleur voisin selon un score d’influence
- Décision locale de type greedy

Le déplacement repose sur un mécanisme de traces comparable aux phéromones :

- Traces de Pac-Man -> Attraction
- Traces de fantômes alliés -> Répulsion
- Traces de fantômes adverses -> Répulsion modérée

Chaque case du graphe reçoit un score :

Score = Attraction − Répulsions

Le voisin présentant le score maximal est sélectionné.

Ce mécanisme s’apparente à :

- Influence Maps
- Champs de potentiel
- Approche locale inspirée de l’Ant Colony Optimization

Il permet l’émergence d’un comportement collectif naturel :
- Dispersion automatique des fantômes
- Convergence progressive vers la cible
- Réduction des regroupements inutiles

Techniques utilisées :

- **BFS (Breadth First Search)** -> plus court chemin
- **BFS limité en profondeur** -> simulation de vision
- **Recherche gloutonne locale (Greedy search)** -> sélection du voisin optimal
- **Distance euclidienne au carré** -> heuristique rapide
- **Bruit aléatoire contrôlé** -> prévention des cycles et oscillations


### 2.2.4) Room

mise en place des rooms avec parametre, selection de role, et information sur la futur parties.

### 2.2.5) regle du jeu

on a comme condition de victoire du pacman : i a manger toutes les pacgommes ou a survit jusqua la fin du timer

pour les fantome, cest si le pacman a epuiser toutes cest vie.

Il y a des pacgomme speciale qui permette au pacman de manger les famtome pendant une courtes periode.

Les fantome ne voit que autour d'eux tandis que le pacman voit tout le plateau


