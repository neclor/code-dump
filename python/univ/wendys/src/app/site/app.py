from flask import Flask, render_template

from app.site.routes import *


def create_app() -> Flask:
    app: Flask = Flask(__name__)

    _register_blueprints(app)   
    app.register_error_handler(404, _page_not_found)

    return app


def _register_blueprints(app: Flask) -> None:
    app.register_blueprint(index.bp)
    app.register_blueprint(events.bp)
    app.register_blueprint(clients.bp)
    app.register_blueprint(locations.bp)
    app.register_blueprint(cds.bp)
    app.register_blueprint(genres.bp)


def _page_not_found(_error) -> tuple: 
    return render_template("page-404.html"), 404
