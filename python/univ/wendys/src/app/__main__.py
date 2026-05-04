import logging
import pymysql
from flask import Flask

from app.configs import *
from app.core.db import database

from app.core.db.repositories import *
from app.core.services import *

from app.site.app import create_app


logger: logging.Logger = logging.getLogger(__name__)


def main() -> None:
    db: pymysql.connections.Connection = database.connect_db(db_config.USER, db_config.PASSWORD, db_config.HOST, db_config.DATABASE)

    app: Flask = create_app()
    add_dependencies(app, db)

    app.run(host="0.0.0.0", port=5000, debug=app_config.DEBUG)
    db.close()


def add_dependencies(app: Flask, db: pymysql.connections.Connection) -> None:
    genre_repo: GenreRepository = GenreRepository(db)
    theme_repo: ThemeRepository = ThemeRepository(db)
    employee_repo: EmployeeRepository =  EmployeeRepository(db)
    event_repo: EventRepository = EventRepository(db)
    cd_repo: CdRepository = CdRepository(db)

    client_repo: ClientRepository = ClientRepository(db)
    location_repo: LocationRepository = LocationRepository(db)

    event_service: EventService = EventService(db, event_repo, employee_repo, cd_repo, client_repo, location_repo, theme_repo)
    music_service: MusicService = MusicService(db, cd_repo, genre_repo)

    app.config["client_repository"] = client_repo
    app.config["location_repository"] = location_repo
    app.config["event_service"] = event_service
    app.config["music_service"] = music_service


if __name__ == "__main__": main()
