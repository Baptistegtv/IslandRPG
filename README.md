# IslandRPG (3DSX)

Base technique de départ pour un action-RPG façon anime : île de départ, personnage
3D contrôlable, caméra 3e personne (façon Zelda). C'est la première brique du jeu
complet (PNJ, quêtes, slimes, autres îles, bateaux viendront ensuite en couches).

## Contrôles

- **Circle Pad** : déplacer le personnage (relatif à la caméra)
- **D-Pad Gauche/Droite** : faire tourner la caméra autour du personnage
- **START** : quitter

## Ce qui est fait dans cette étape

- Rendu 3D via citro3d (PICA200)
- Sol de l'île (grand plan d'herbe verte)
- Personnage (cube coloré temporaire, à remplacer par un vrai modèle plus tard)
- Caméra 3e personne qui suit et tourne autour du joueur
- Écran du bas : debug texte (position du joueur)

## Compilation

Ce projet compile automatiquement via GitHub Actions (voir `.github/workflows/build.yml`).
À chaque push sur `main`, le `.3dsx` est généré et disponible dans l'onglet
**Actions** du repo → dernier run → section **Artifacts**.

Aucune installation locale n'est nécessaire.
