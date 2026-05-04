from datetime import date as Date
from decimal import Decimal
from typing import cast
from flask import Blueprint, render_template, current_app, request, abort, redirect, url_for
from werkzeug.wrappers import Response

from app.core.models import *
from app.core.db.repositories import *
from app.core.services.event_service import EventService


bp: Blueprint = Blueprint("events", __name__, url_prefix="/events")


_FILTERS: list[dict] = [
    {"name": "id",            "label": "Id",            "type": "number"},
    {"name": "name",          "label": "Name",          "type": "text"},
    {"name": "date",          "label": "Date",          "type": "date"},
    {"name": "description",   "label": "Description",   "type": "text"},
    {"name": "client",        "label": "Client id",     "type": "number"},
    {"name": "manager",       "label": "Manager id",    "type": "number"},
    {"name": "event_planner", "label": "Planner id",    "type": "number"},
    {"name": "dj",            "label": "DJ id",         "type": "number"},
    {"name": "theme",         "label": "Theme",         "type": "text"},
    {"name": "type",          "label": "Type",          "type": "text"},
    {"name": "location",      "label": "Location id",   "type": "number"},
    {"name": "rental_fee",    "label": "Rental fee",    "type": "number"},
    {"name": "playlist",      "label": "Playlist",      "type": "text"},
]


@bp.route("/", methods=["GET"])
def events() -> str:
    service: EventService = current_app.config["event_service"]

    date_raw: str | None = request.args.get("date")
    rental_fee_raw: str | None = request.args.get("rental_fee")

    events: list[Event] = service.get_all(EventFilter(
        id=request.args.get("id", type=int),
        name=request.args.get("name"),
        date=Date.fromisoformat(date_raw) if date_raw else None,
        description=request.args.get("description"),
        client=request.args.get("client", type=int),
        manager=request.args.get("manager", type=int),
        event_planner=request.args.get("event_planner", type=int),
        dj=request.args.get("dj", type=int),
        theme=request.args.get("theme"),
        type=request.args.get("type"),
        location=request.args.get("location", type=int),
        rental_fee=Decimal(rental_fee_raw) if rental_fee_raw else None,
        playlist=request.args.get("playlist"),
        sort_column=cast(EventColumn, request.args.get("sort", "date")),
        order=cast(SortOrder, request.args.get("order", "desc")),
    ))

    return render_template("pages/events/events.html", filters=_FILTERS, events=events)


@bp.route("/dashboard")
def dashboard() -> str:
    service: EventService = current_app.config["event_service"]

    events: list[EventDashboard] = service.get_dashboard(EventDashboardFilter(
        sort_column=cast(EventDashboardColumn, request.args.get("sort", "date")),
        order=cast(SortOrder, request.args.get("order", "desc")),
    ))

    return render_template("pages/events/dashboard.html", events=events)


@bp.route("/new", methods=["GET", "POST"])
@bp.route("/<int:event_id>/edit", methods=["GET", "POST"])
def edit(event_id: int | None = None) -> str | Response:
    service: EventService = current_app.config["event_service"]

    event: Event | None = None
    if event_id is not None:
        event = service.get_by_id(event_id)
        if event is None: abort(404)

    error: str | None = None
    if request.method == "POST":
        try:
            date_raw: str = request.form.get("date", "")
            rental_fee_raw: str = request.form.get("rental_fee", "")
            updated: Event = Event(
                id=event.id if event else None,
                client=request.form.get("client", -1, type=int),
                name=request.form.get("name", ""),
                date=Date.fromisoformat(date_raw),
                theme=request.form.get("theme", ""),
                type=request.form.get("type", ""),
                description=request.form.get("description", "").strip() or None,
                manager=request.form.get("manager", type=int),
                event_planner=request.form.get("event_planner", type=int),
                dj=request.form.get("dj", type=int),
                location=request.form.get("location", type=int),
                rental_fee=Decimal(rental_fee_raw) if rental_fee_raw else None,
                playlist=request.form.get("playlist") or None,
            )
            new_id: int = service.save(updated)

            if event is None: return redirect(url_for("events.edit", event_id=new_id))

            event = updated
        except ValueError as e:
            error = str(e)

    return render_template("pages/events/edit.html", event=event, error=error)


@bp.route("/<int:event_id>/delete", methods=["POST"])
def delete(event_id: int) -> str | Response:
    service: EventService = current_app.config["event_service"]

    try:
        service.delete(event_id)
        return redirect(url_for("events.events"))

    except ValueError as e:
        event: Event | None = service.get_by_id(event_id)
        if event is None: abort(404)

        return render_template("pages/events/edit.html", event=event, error=str(e))
