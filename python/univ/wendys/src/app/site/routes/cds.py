from datetime import time as Time
from typing import cast
from flask import Blueprint, render_template, current_app, request, abort, redirect, url_for
from werkzeug.wrappers import Response

from app.core.models import *
from app.core.db.repositories import *
from app.core.services import *


bp: Blueprint = Blueprint("cds", __name__, url_prefix="/cds")


@bp.route("/stats")
def stats() -> str:
    service: MusicService = current_app.config["music_service"]

    stats: list[CdStat] = service.get_cd_statistics(CdStatFilter(
        sort_column=cast(CdStatColumn, request.args.get("sort", "cd_number")),
        order=cast(SortOrder, request.args.get("order", "asc")),
    ))

    return render_template("pages/cds/stats.html", stats=stats)


@bp.route("/dashboard")
def dashboard() -> str:
    service: MusicService = current_app.config["music_service"]

    cds: list[CdDashboard] = service.get_cd_dashboard(CdDashboardFilter(
        sort_column=cast(CdDashboardColumn, request.args.get("sort", "date")),
        order=cast(SortOrder, request.args.get("order", "desc")),
    ))

    return render_template("pages/cds/dashboard.html", cds=cds)


@bp.route("/new", methods=["GET", "POST"])
@bp.route("/<int:cd_number>/edit", methods=["GET", "POST"])
def cd_edit(cd_number: int | None = None) -> str | Response:
    service: MusicService = current_app.config["music_service"]

    cd: Cd | None = None
    songs: list[Song] = []
    next_track: int = 1

    if cd_number is not None:
        cd = service.get_cd_by_number(cd_number)
        if cd is None: abort(404)
        songs = service.get_songs_by_cd(cd_number)
        next_track = max((s.track_number for s in songs), default=0) + 1

    error: str | None = None
    if request.method == "POST":
        try:
            updated: Cd = Cd(
                cd_number=cd.cd_number if cd else None,
                title=request.form.get("title", ""),
                producer=request.form.get("producer", ""),
                year=int(request.form.get("year", 0)),
                copies=int(request.form.get("copies", 0)),
            )
            new_number: int = service.save_cd(updated)

            if cd is None: return redirect(url_for("cds.cd_edit", cd_number=new_number))

            cd = updated
        except ValueError as e:
            error = str(e)

    return render_template("pages/cds/cd_edit.html", cd=cd, songs=songs, next_track=next_track, error=error)


@bp.route("/<int:cd_number>/delete", methods=["POST"])
def cd_delete(cd_number: int) -> str | Response:
    service: MusicService = current_app.config["music_service"]

    try:
        service.delete_cd(cd_number)
        return redirect(url_for("cds.stats"))

    except ValueError as e:
        cd: Cd | None = service.get_cd_by_number(cd_number)
        if cd is None: abort(404)

        songs: list[Song] = service.get_songs_by_cd(cd_number)
        next_track: int = max((s.track_number for s in songs), default=0) + 1

        return render_template("pages/cds/cd_edit.html", cd=cd, songs=songs, next_track=next_track, error=str(e))


@bp.route("/<int:cd_number>/songs/<int:track_number>/edit", methods=["GET", "POST"])
def song_edit(cd_number: int, track_number: int) -> str | Response:
    service: MusicService = current_app.config["music_service"]

    cd: Cd | None = service.get_cd_by_number(cd_number)
    if cd is None: abort(404)

    song: Song | None = service.get_song(cd_number, track_number)

    error: str | None = None
    if request.method == "POST":
        try:
            duration_raw: str = request.form.get("duration", "")
            new_track_number: int = int(request.form.get("track_number", track_number))
            updated: Song = Song(
                cd_number=cd_number,
                track_number=new_track_number,
                title=request.form.get("title", ""),
                artist=request.form.get("artist", ""),
                duration=Time.fromisoformat(duration_raw),
                genre=request.form.get("genre", ""),
            )
            service.save_song(updated, old_track_number=track_number)

            if new_track_number != track_number:
                return redirect(url_for("cds.song_edit", cd_number=cd_number, track_number=new_track_number))

            song = updated
        except ValueError as e:
            error = str(e)

    return render_template("pages/cds/song_edit.html", cd_number=cd_number, track_number=track_number, song=song, error=error)


@bp.route("/<int:cd_number>/songs/<int:track_number>/delete", methods=["POST"])
def song_delete(cd_number: int, track_number: int) -> str | Response:
    service: MusicService = current_app.config["music_service"]

    try:
        service.delete_song(cd_number, track_number)
        return redirect(url_for("cds.cd_edit", cd_number=cd_number))

    except ValueError as e:
        song: Song | None = service.get_song(cd_number, track_number)
        if song is None: abort(404)

        return render_template("pages/cds/song_edit.html", cd_number=cd_number, track_number=track_number, song=song, error=str(e))
