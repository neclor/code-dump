from typing import cast
from flask import Blueprint, render_template, current_app, request

from app.core.models import *
from app.core.db.repositories import *


bp: Blueprint = Blueprint("locations", __name__, url_prefix="/locations")


_FILTERS: list[dict] = [
    {"name": "id",          "label": "Id",          "type": "number"},
    {"name": "street",      "label": "Street",      "type": "text"},
    {"name": "city",        "label": "City",        "type": "text"},
    {"name": "postal_code", "label": "Postal code", "type": "text"},
    {"name": "country",     "label": "Country",     "type": "text"},
    {"name": "comment",     "label": "Comment",     "type": "text"},
]


@bp.route("/")
def locations() -> str:
    repo: LocationRepository = current_app.config["location_repository"]

    locations: list[Location] = repo.get_all(LocationFilter(
        id=request.args.get("id", type=int),
        street=request.args.get("street"),
        city=request.args.get("city"),
        postal_code=request.args.get("postal_code"),
        country=request.args.get("country"),
        comment=request.args.get("comment"),
        sort_column=cast(LocationColumn, request.args.get("sort", "id")),
        order=cast(SortOrder, request.args.get("order", "asc")),
    ))

    return render_template("pages/locations.html", filters=_FILTERS, locations=locations)
