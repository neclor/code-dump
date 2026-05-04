import pymysql


class Repository:
    def __init__(self, db: pymysql.connections.Connection) -> None:
        self._db = db
