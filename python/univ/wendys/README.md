# Docker MySQL + phpMyAdmin + Application Flask

Une pile pédagogique complète basée sur Docker, comprenant MySQL, phpMyAdmin et une application web Python Flask avec des exemples interactifs pour apprendre les concepts des bases de données.

## Démarrage rapide

```bash
docker-compose up --build
```

Puis accédez à :
- **Application Flask** : http://localhost (port 80)
- **phpMyAdmin** : http://localhost:8080 (root / example)

## Services

### Base de données MySQL
- **Image** : mysql:8.0
- **Port** : 3306
- **Identifiants** : root / example
- **Base de données** : testdb (utilisateur / mot de passe)
- **Initialisation** : Les scripts du dossier `dump/` s'exécutent automatiquement au premier démarrage

### phpMyAdmin
- **Port** : 8080
- **Accès** : http://localhost:8080
- Gérez votre base de données MySQL via l'interface web

### Application Flask
- **Port** : 80
- **Runtime** : Python 3.11
- **Framework** : Flask
- **Pilote de base de données** : pymysql + cryptography

## Exemples pédagogiques

L'application Flask comprend 5 exemples progressifs :

### Exemple 1 : HTML basique
- **Route** : `/example-1-basic-html`
- **Concept** : Rendu HTML statique

### Exemple 2 : HTML dynamique
- **Route** : `/example-2-dynamic-html`
- **Concept** : Modèles Jinja2 avec boucles

### Exemple 3 : Traitement de formulaires
- **Route** : `/example-3-form-handling`
- **Concept** : Traitement des formulaires POST, extraction de données

### Exemple 4 : Récupération de données avec JOIN
- **Route** : `/example-4-retrieve-from-db`
- **Concepts** : 
  - Jointure SQL (employé + département)
  - Recherche LIKE avec clause WHERE
  - Paramètres de requête (GET)

### Exemple 5 : Gestion des transactions
- **Route** : `/example-5-transaction-mgmt`
- **Concepts** :
  - Transactions de base de données
  - COMMIT en cas de succès
  - ROLLBACK en cas d'échec
  - Opérations atomiques

## Schéma de la base de données

Tables pré-chargées :
- **department** (DNO, DNAME, MGR_ID, MGR_START)
- **employee** (EMP_ID, FNAME, LNAME, BDATE, ADDRESS, SALARY, DEPT_NO)

## Ajouter de nouveaux exemples

1. Créez un nouveau fichier de route dans `web-app/python_app/routes/example_X_*.py` :
   ```python
   from flask import Blueprint, render_template
   
   bp = Blueprint('example_x', __name__)
   
   @bp.route('/example-X-name')
   def route_name():
       return render_template('example_x.html')
   ```

2. Créez le modèle correspondant dans `web-app/python_app/templates/example_x.html`

3. Enregistrez le blueprint dans `web-app/python_app/routes/__init__.py` :
   ```python
   from . import example_x_name
   # ... dans register_blueprints():
   app.register_blueprint(example_x_name.bp)
   ```

4. Reconstruisez et redémarrez :
   ```bash
   docker-compose up --build
   ```

## Dépannage

**Erreur "Table doesn't exist":**
- Assurez-vous que `dump/dump.sql` a `USE testdb;` comme première instruction
- Exécutez `docker-compose down -v && docker-compose up --build`

**Port déjà utilisé:**
- Modifiez les ports dans `docker-compose.yml`
- Exemple : `"3307:3306"` pour MySQL, `"8081:80"` pour phpMyAdmin

**Base de données non initialisée:**
- Vérifiez les logs : `docker-compose logs db`
- Vérifiez que les fichiers du dossier `dump/` sont présents

## Prérequis

- Docker & Docker Compose
- Aucun Python local requis (entièrement containerisé)

---

## À propos

Ce projet a été généré avec **Claude** comme port Flask du modèle PHP original disponible sur [docker-lamp](https://github.com/chrdebru/docker-lamp). Il conserve la même structure pédagogique et les mêmes concepts d'enseignement, adaptés à l'écosystème Python et Flask.
