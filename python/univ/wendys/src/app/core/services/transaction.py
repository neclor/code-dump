import pymysql


class Transaction:
    def __init__(self, connection: pymysql.connections.Connection) -> None:
        self._connection = connection


    def __enter__(self):
        self._connection.begin()
        return self


    def __exit__(self, exc_type, _exc_val, _exc_tb) -> None:
        if exc_type:
            self._connection.rollback()
        else:
            self._connection.commit()
