from typing import cast
from flask import Blueprint, render_template, current_app, request

from app.core.models import *
from app.core.db.repositories import *


bp: Blueprint = Blueprint("clients", __name__, url_prefix="/clients")


_FILTERS: list[dict] = [
    {"name": "first_name",    "label": "First name",    "type": "text"},
    {"name": "last_name",     "label": "Last name",     "type": "text"},
    {"name": "email_address", "label": "Email",         "type": "text"},
    {"name": "phone_number",  "label": "Phone number",  "type": "text"},
    {"name": "client_number", "label": "Client number", "type": "number"},
]


@bp.route("/")
def clients() -> str:
    repo: ClientRepository = current_app.config["client_repository"]

    clients: list[Client] = repo.get_all(ClientFilter(
        client_number=request.args.get("client_number", type=int),
        first_name=request.args.get("first_name"),
        last_name=request.args.get("last_name"),
        email_address=request.args.get("email_address"),
        phone_number=request.args.get("phone_number"),
        sort_column=cast(ClientColumn, request.args.get("sort", "client_number")),
        order=cast(SortOrder, request.args.get("order", "asc")),
    ))

    return render_template("pages/clients.html", filters=_FILTERS, clients=clients)
