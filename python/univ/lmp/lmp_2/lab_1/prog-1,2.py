import sys
import math
import pygame


K: float = 8.9876 * (10 ** 9)
ABS_OBJECT_CHARGE: float = 10 ** -6

BLUE: pygame.Color = pygame.Color("#7fbfff")
BLUE_2: pygame.Color = pygame.Color("#0000ff")
RED: pygame.Color = pygame.Color("#ff0000")
BLACK: pygame.Color = pygame.Color("#000000")


RESOLUTION: tuple[int, int] = (1600, 900)
FPS: int = 60


clock: pygame.time.Clock = pygame.time.Clock()
surface: pygame.Surface


objects: list[tuple[int, int, float]] = []


def main() -> None:
    init()
    run()


def init() -> None:
    global clock, surface
    pygame.init()
    clock = pygame.time.Clock()
    surface = pygame.display.set_mode(RESOLUTION)

    add_object(800, 200, ABS_OBJECT_CHARGE)
    add_object(800, 700, -ABS_OBJECT_CHARGE)


def run() -> None:
    while True:
        check_input()
        update()
        draw()


def check_input() -> None:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            x, y = event.pos
            if event.button == 1:
                add_object(x, y, ABS_OBJECT_CHARGE)
            elif event.button == 3:
                add_object(x, y, -ABS_OBJECT_CHARGE)




def add_object(x: int, y: int, q: float) -> None:
    objects.append((x, y, q))


def update() -> None:
    pygame.display.set_caption(str(round(clock.get_fps())))
    pygame.display.flip()
    clock.tick(FPS)


def draw() -> None:
    surface.fill(BLUE)
    draw_field()
    for o in objects:
        pygame.draw.circle(surface, RED if o[2] > 0 else BLACK, (o[0], o[1]), 10)


def draw_field() -> None:
    for x in range(-50, RESOLUTION[0] + 50, 50):
        for y in range(-50, RESOLUTION[1] + 50, 50):
            draw_field_vector(x, y)



def draw_field_vector(x, y) -> None:
    field_vector_x: float = 0
    field_vector_y: float = 0
    for o in objects:
        dx: int = x - o[0]
        dy: int = y - o[1]
        dist: float = math.sqrt(dx ** 2 + dy ** 2)
        if dist <= 20: return
        n: float = K * o[2] / (dist ** 2)
        field_vector_x += (dx / dist) * n
        field_vector_y += (dy / dist) * n

    force: float = math.sqrt(field_vector_x ** 2 + field_vector_y ** 2)
    if force < 10 ** -10: return

    fx: float = field_vector_x / force * 40
    fy: float = field_vector_y / force * 40
    v: float = math.sqrt(1000 * force)

    draw_vector((x - fx / 2, y - fy / 2), (fx, fy), calculate_color(v))


def calculate_color(v) -> tuple:
    if 0 <= v <= 8: return (255, 255 * v / 8, 0)
    if 8 < v <= 16: return (255 - 255 * (v - 8) / 8, 255, 255 * v / 16)
    if 16 < v <= 24: return (0, 255 - 255 * (v - 16) / 8, 255)
    if 24 < v <= 32: return (255 * (v - 24) / 8, 0, 255)
    else: return (255, 0 ,255)


def draw_vector(p, v, color):
    def deplacer_pol(point, distance, orientation):
        x, y = point
        x_2 = x + distance * math.cos(orientation)
        y_2 = y + distance * math.sin(orientation)
        return (x_2, y_2)

    A = 2
    B = 5
    C = 20

    x = 0
    y = 1
    dist_v = math.sqrt(v[x] ** 2 + v[y] ** 2)
    a = math.atan2(v[y], v[x])
    if dist_v >= C:
        p_4 = (p[x] + v[x], p[y] + v[y])
        p_1 = deplacer_pol(p, A, a - math.pi / 2)
        p_7 = deplacer_pol(p, A, a + math.pi / 2)
        p_c = deplacer_pol(p, dist_v - C, a)
        p_2 = deplacer_pol(p_c, A, a - math.pi / 2)
        p_6 = deplacer_pol(p_c, A, a + math.pi / 2)
        p_3 = deplacer_pol(p_2, B, a - math.pi / 2)
        p_5 = deplacer_pol(p_6, B, a + math.pi / 2)
        polygon = [p_1, p_2, p_3, p_4, p_5, p_6, p_7]
    else:
        p_3 = (p[x] + v[x], p[y] + v[y])
        p_1 = deplacer_pol(p_3, C, a + math.pi)
        p_2 = deplacer_pol(p_1, A + B, a - math.pi / 2)
        p_4 = deplacer_pol(p_1, A + B, a + math.pi / 2)
        polygon = [p_1, p_2, p_3, p_4]
    pygame.draw.polygon(surface, color, polygon)


if __name__ == "__main__": main()
