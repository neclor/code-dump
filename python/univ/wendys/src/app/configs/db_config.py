import os


USER: str = os.environ.get("MYSQL_USER", "user")
PASSWORD: str = os.environ.get("MYSQL_PASSWORD", "user")
HOST: str = os.environ.get("MYSQL_HOST", "host")
DATABASE: str = os.environ.get("MYSQL_DATABASE", "database")
