import sys
from typing import Tuple
import math
import pygame


NOIR: Tuple[int, int, int] = (0, 0, 0)
RED: Tuple[int, int, int] = (255, 0, 0)
ORANGE: Tuple[int, int, int] = (255, 128, 0)
YELLOW: Tuple[int, int, int] = (255, 255, 0)

WINDOW_SIZE: Tuple[int, int] = (800, 600)

fps: int = 30
background_color: Tuple[int, int, int] = NOIR

clock: pygame.time.Clock
previous_time: int
window: pygame.Surface

ship: dict = {
	"position": [WINDOW_SIZE[0] / 2.0, WINDOW_SIZE[1] / 2.0],
	"rotation": 0.0,
	"velocity": [0.0, 0.0],
	"force": 0.0003,
	"rotation_speed": math.pi / 20,
	"max_engine_time": 3,
	"engine_time": 0,
	"weight": 1,
}
planet: dict = {
	"exist": False,
	"position": [0.0, 0.0],
	"radius": 40,
	"weight": 160,
	"G": 0.001,
}


def main() -> None:
	init()
	run()


def init() -> None:
	pygame.init()

	global clock, previous_time, window
	clock = pygame.time.Clock()
	previous_time = pygame.time.get_ticks()
	window = pygame.display.set_mode(WINDOW_SIZE)


def run() -> None:
	while True:
		handle_input()
		update_ship()
		draw()
		update()
		check_exit()


def handle_input() -> None:
	keys = pygame.key.get_pressed()
	buttons = pygame.mouse.get_pressed()

	if keys[pygame.K_LEFT]:
		ship["rotation"] -= ship["rotation_speed"]
	if keys[pygame.K_RIGHT]:
		ship["rotation"] += ship["rotation_speed"]
	if keys[pygame.K_UP]:
		ship["engine_time"] = ship["max_engine_time"]

	if buttons[0]:
		planet["exist"] = True
		planet["position"] = pygame.mouse.get_pos()


def update_ship() -> None:
	def update_ship_position() -> None:
		global previous_time

		time = pygame.time.get_ticks()
		delta = time - previous_time
		previous_time = time

		if ship["engine_time"] > 0:
			ship["velocity"][0] += ship["force"] * math.cos(ship["rotation"]) / ship["weight"] * delta
			ship["velocity"][1] += ship["force"] * math.sin(ship["rotation"]) / ship["weight"] * delta

		if planet["exist"]:
			vector_to_planet: list[float] = [planet["position"][0] - ship["position"][0], planet["position"][1] - ship["position"][1]]
			distance_to_planet_pow_2: float = vector_to_planet[0] ** 2 + vector_to_planet[1] ** 2
			distance_to_planet: float = math.sqrt(distance_to_planet_pow_2)

			gravity = planet["G"] * planet["weight"] * ship["weight"] / distance_to_planet_pow_2

			ship["velocity"][0] += gravity * vector_to_planet[0] / distance_to_planet * delta
			ship["velocity"][1] += gravity * vector_to_planet[1] / distance_to_planet * delta

		ship["position"][0] += ship["velocity"][0] * delta
		ship["position"][1] += ship["velocity"][1] * delta

		ship["position"][0] = (ship["position"][0] + WINDOW_SIZE[0]) % WINDOW_SIZE[0]
		ship["position"][1] = (ship["position"][1] + WINDOW_SIZE[1]) % WINDOW_SIZE[1]


	if ship["engine_time"] > 0:
		ship["engine_time"] -= 1

	update_ship_position()


def draw() -> None:
	def draw_planet() -> None:
		if planet["exist"]:
			pygame.draw.circle(window, ORANGE, planet["position"], planet["radius"])


	def draw_ship() -> None: # afficher_vaisseau()
		def draw_triangle(color: Tuple[int, int, int], p: list[float], r: int, a: float, b: float) -> None:
			p1: list[float] = [p[0] + r * math.cos(a + b), p[1] + r * math.sin(a + b)]
			p2: list[float] = [p[0] + r * math.cos(a - b), p[1] + r * math.sin(a - b)]
			pygame.draw.polygon(window, ORANGE, [p, p1, p2])


		if ship["engine_time"] > 0:
			draw_triangle(YELLOW, ship["position"], 38, ship["rotation"] + 21 / 20 * math.pi, math.pi / 30)
			draw_triangle(YELLOW, ship["position"], 38, ship["rotation"] + 19 / 20 * math.pi, math.pi / 30)
		draw_triangle(ORANGE, ship["position"], 23, ship["rotation"] + math.pi, math.pi / 7)
		pygame.draw.circle(window, RED, ship["position"], 15)



	window.fill(background_color)
	draw_planet()
	draw_ship()


def update() -> None:
	pygame.display.set_caption(str(round(clock.get_fps())))
	pygame.display.flip()
	clock.tick(fps)


def check_exit():
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			pygame.quit()
			sys.exit()


if __name__ == "__main__":
   main()
