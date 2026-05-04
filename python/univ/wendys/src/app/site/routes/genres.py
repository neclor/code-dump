from flask import Blueprint, render_template, current_app, request

from app.core.models import *
from app.core.services import *


bp: Blueprint = Blueprint("genres", __name__, url_prefix="/genres")


@bp.route("/", methods=["GET", "POST"])
def taxonomy() -> str:
    service: MusicService = current_app.config["music_service"]

    error: str | None = None
    if request.method == "POST":
        try:
            genres: list[Genre] = []

            for line in request.form.get("text", "").splitlines():
                parts = line.split(",")
                if len(parts) == 2:
                    genres.append(Genre(genre=parts[1].strip(), subgenre=parts[0].strip()))
                elif len(parts) == 1 and parts[0].strip():
                    genres.append(Genre(genre=parts[0].strip(), subgenre=None))

            service.add_taxonomy(genres)

        except ValueError as e:
            error = str(e)

    return render_template("pages/genres.html", genres=service.get_genres_dashboard(), error=error)
