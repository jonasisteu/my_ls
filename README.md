# 📂 My LS

Bienvenue dans **My LS**, une implémentation en C de la commande `ls` du shell Bash. Ce projet permet de lister le contenu de répertoires avec plusieurs options de formatage et de filtrage.

## ✨ Fonctionnalités

- **`-l` (Long listing)** : Affiche les informations détaillées des fichiers et répertoires (permissions, nombre de liens, propriétaire, groupe, taille, date de modification, nom).
- **`-R` (Recursive)** : Liste récursivement le contenu des sous-répertoires rencontrés.
- **`-r` (Reverse)** : Inverse l'ordre de tri de la liste (alphabétique par défaut).
- **`-t` (Time sort)** : Trie la liste par date de dernière modification (du plus récent au plus ancien).
- **`-a` (All)** : Affiche tous les fichiers, y compris ceux dont le nom commence par un point (`.`).
- **`-A` (Almost all)** : Affiche tous les fichiers, à l'exception des entrées spéciales `.` (répertoire courant) et `..` (répertoire parent).

---

## 🛠️ Installation

### 1. Cloner ou Télécharger le Projet
- Clonez ce dépôt :
  ```bash
  git clone git@github.com:jonasisteu/my_ls.git
  ```
  **OU**
- Téléchargez le fichier `.zip` et extrayez-le dans le dossier de votre choix.

### 2. Compiler le Projet
- Ouvrez votre terminal et naviguez jusqu'au dossier **MyLS**.
- Assurez-vous que `gcc` (GNU Compiler Collection) est installé sur votre système.
- Utilisez la commande `make` pour compiler le projet en vous basant sur le `Makefile` fourni :
  ```bash
  make
  ```
  Cette commande créera un exécutable nommé (probablement) `my_ls` dans le répertoire courant.

### 3. Exécuter l'Application
- Après une compilation réussie, vous pouvez exécuter la commande `my_ls` avec les options souhaitées. Par exemple, pour lister le contenu du répertoire courant avec les détails longs :
  ```bash
  ./my_ls -l
  ```
- Pour lister le contenu de manière récursive :
  ```bash
  ./my_ls -R
  ```
- Vous pouvez combiner plusieurs options :
  ```bash
  ./my_ls -l -a -t
  ```

---

## 🧪 Utilisation

Une fois compilé, l'exécutable `my_ls` peut être utilisé de la même manière que la commande `ls` de Bash, en passant les options disponibles :

1.  Ouvrez votre terminal.
2.  Naviguez vers le répertoire où l'exécutable `my_ls` a été créé.
3.  Exécutez la commande `./my_ls` suivie des options que vous souhaitez utiliser et du ou des chemins de répertoire à lister (si aucun chemin n'est spécifié, le répertoire courant est utilisé).

    Exemples :
    ```bash
    ./my_ls
    ./my_ls -l dossier_exemple
    ./my_ls -R /home/utilisateur
    ./my_ls -a -r
    ```

---

## 💻 Environnement

Ce projet a été développé pour fonctionner principalement sous **Debian 12**. Cependant, il est indiqué qu'il fonctionne également sous **macOS**, en supposant que les librairies standard du C utilisées soient compatibles entre les systèmes.
```