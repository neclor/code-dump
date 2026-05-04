import logging
import pymysql

from app.core.services.transaction import Transaction
from app.core.models import *
from app.core.db.repositories import *


logger: logging.Logger = logging.getLogger(__name__)


class MusicService:
    def __init__(self, db: pymysql.connections.Connection, music: CdRepository, genres: GenreRepository) -> None:
        self._db = db
        self._cd = music
        self._genres = genres


    def get_cd_by_number(self, cd_number: int) -> Cd | None:
        return self._cd.get_cd_by_number(cd_number)


    def save_cd(self, cd: Cd) -> int:
        with Transaction(self._db):
            if cd.cd_number is not None and not self._cd.cd_exists(cd.cd_number, lock=True): raise ValueError("CD does not exist.")
            new_number: int = self._cd.save_cd(cd)

            if cd.cd_number is None: logger.info("CD created: cd_number=%d title=%r", new_number, cd.title)
            else: logger.info("CD updated: cd_number=%d title=%r", cd.cd_number, cd.title)

            return new_number


    def delete_cd(self, cd_number: int) -> None:
        with Transaction(self._db):
            self._cd.delete_cd(cd_number)

            logger.info("CD deleted: cd_number=%d", cd_number)


    def get_cd_statistics(self, filter: CdStatFilter = CdStatFilter()) -> list[CdStat]:
        return self._cd.get_cd_statistics(filter)


    def get_cd_dashboard(self, filter: CdDashboardFilter = CdDashboardFilter()) -> list[CdDashboard]:
        return self._cd.get_cd_dashboard(filter)


    def get_song(self, cd_number: int, track_number: int) -> Song | None:
        return self._cd.get_song(cd_number, track_number)


    def save_song(self, song: Song, old_track_number: int | None = None) -> None:
        with Transaction(self._db):
            if old_track_number is not None and old_track_number <= 0: raise ValueError("old_track_number must be greater than 0.")

            if not self._cd.cd_exists(song.cd_number, lock=True): raise ValueError("Cd does not exist.")
            if not self._genres.genre_exists(song.genre, lock=True): raise ValueError("Genre does not exist.")

            if old_track_number is not None and old_track_number != song.track_number:
                if self._cd.get_song(song.cd_number, song.track_number, lock=True) is not None: raise ValueError("Track position is already taken.")
                self._cd.delete_song(song.cd_number, old_track_number)

            self._cd.save_song(song)

            logger.info("Song saved: cd_number=%d track=%d title=%r", song.cd_number, song.track_number, song.title)


    def delete_song(self, cd_number: int, track_number: int) -> None:
        with Transaction(self._db):
            self._cd.delete_song(cd_number, track_number)

            logger.info("Song deleted: cd_number=%d track=%d", cd_number, track_number)


    def get_songs_by_cd(self, cd_number: int) -> list[Song]:
        return self._cd.get_songs_by_cd(cd_number)


    def add_taxonomy(self, genres: list[Genre]) -> None:
        names: list[str] = [g for genre in genres for g in (genre.genre, genre.subgenre) if g is not None]

        with Transaction(self._db):
            self._genres.insert_ignore_genres(*names)
            for genre in genres:
                if self._genres.genre_create_cycle(genre): raise ValueError(f"Cycle: {genre.subgenre} -> {genre.genre}")
                self._genres.insert_ignore_specializes(genre)

            logger.info("Taxonomy added: %d genre(s)", len(genres))


    def get_genres_dashboard(self) -> list[GenreDashboard]:
        return self._genres.get_dashboard()
