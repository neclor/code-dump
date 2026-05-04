from typing import get_args
import pymysql

from app.core.db.repositories.repository import *
from app.core.models import *


class ClientRepository(Repository):

    def client_exists(self, client_number: int, lock: bool = False) -> bool:
        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT COUNT(*) AS n FROM client WHERE client_number = %s {'FOR UPDATE' if lock else ''}", [client_number])
            row: dict | None = cursor.fetchone()
        return row["n"] == 1 if row else False


    def get_all(self, filter: ClientFilter = ClientFilter(), lock: bool = False) -> list[Client]:
        conditions: list[str] = []
        params: list[object] = []

        if filter.client_number is not None: conditions.append("client_number = %s");    params.append(filter.client_number)
        if filter.first_name:                conditions.append("first_name LIKE %s");    params.append(f"%{filter.first_name}%")
        if filter.last_name:                 conditions.append("last_name LIKE %s");     params.append(f"%{filter.last_name}%")
        if filter.email_address:             conditions.append("email_address LIKE %s"); params.append(f"%{filter.email_address}%")
        if filter.phone_number:              conditions.append("phone_number LIKE %s");  params.append(f"%{filter.phone_number}%")

        where: str = f"WHERE {' AND '.join(conditions)}" if conditions else ""
        column: ClientColumn = filter.sort_column if filter.sort_column in get_args(ClientColumn) else "client_number"
        order_by: str = f"ORDER BY {column} {'DESC' if filter.order == 'desc' else 'ASC'}"

        with self._db.cursor(pymysql.cursors.DictCursor) as cursor:
            cursor.execute(f"SELECT * FROM client {where} {order_by} {'FOR UPDATE' if lock else ''}", params)
            rows: tuple[dict, ...] = cursor.fetchall()

        return [Client(**row) for row in rows]



