import sys
import math
import pygame


K: float = 8.9876 * (10 ** 9)
A: float = 10


BLUE: pygame.Color = pygame.Color("#7fbfff")
RED: pygame.Color = pygame.Color("#ff0000")
BLACK: pygame.Color = pygame.Color("#000000")
GRAY: pygame.Color = pygame.Color("#808080")


RESOLUTION: tuple[int, int] = (1600, 900)  # (800, 600)
FPS: int = 1000


class Mobile:
    TRACE_SIZE: int = 10000

    def __init__(self: "Mobile") -> None:
        self.position: pygame.Vector2 = pygame.Vector2(RESOLUTION[0] / 2, RESOLUTION[1] / 2)
        self.charge: float = 10 ** -10
        self.mass: float = 10 ** -10

        self.velocity: pygame.Vector2 = pygame.Vector2()

        self.trace: list[pygame.Vector2] = [pygame.Vector2() for _ in range(Mobile.TRACE_SIZE)]
        self.trace_count: int = 0


    def draw(self: "Mobile") -> None:
        for position in self.trace: pygame.draw.circle(surface, GRAY, position, 4)
        pygame.draw.circle(surface, RED if self.charge > 0 else BLACK, self.position, 10, 4)


    def update(self: "Mobile", delta: float) -> None:
        coulomb_force: pygame.Vector2 = self.charge * calculate_electric_field()
        lorentz_force: pygame.Vector2 = self.charge * magnetic_field * pygame.Vector2(-self.velocity.y, self.velocity.x)
        force: pygame.Vector2 = coulomb_force + lorentz_force

        acceleration: pygame.Vector2 = force / self.mass

        self.position += self.velocity * delta + acceleration * (delta ** 2) / 2
        self.velocity += acceleration * delta

        self.add_trace()


    def add_trace(self: "Mobile") -> None:
        self.trace[self.trace_count] = self.position.copy()
        self.trace_count = (self.trace_count + 1) % Mobile.TRACE_SIZE


surface: pygame.Surface
clock: pygame.time.Clock
font: pygame.font.Font


electric_field: pygame.Vector2 = pygame.Vector2(0, 0)
magnetic_field: float = 1


cyclotron_mode: bool = False
alpha: float = 0


mobile: Mobile


def main() -> None:
    init()
    run()


def init() -> None:
    global surface, clock, font, mobile
    pygame.init()
    clock = pygame.time.Clock()
    surface = pygame.display.set_mode(RESOLUTION)
    font = pygame.font.SysFont("monospace", 16)

    mobile = Mobile()


def run() -> None:
    while True:
        check_input()
        delta: float = clock.get_time() / 1000
        if cyclotron_mode: calculate_cyclotron_field(delta)
        mobile.update(delta)
        draw()
        update()


def check_input() -> None:
    global mobile, electric_field, magnetic_field, cyclotron_mode
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_c:
                cyclotron_mode = True
            elif event.key == pygame.K_SPACE:
                cyclotron_mode = False
                mobile = Mobile()
            elif event.key == pygame.K_UP:
                cyclotron_mode = False
                electric_field.y = pygame.math.clamp(electric_field.y + 1, -100, 100)
            elif event.key == pygame.K_DOWN:
                cyclotron_mode = False
                electric_field.y = pygame.math.clamp(electric_field.y - 1, -100, 100)
            elif event.key == pygame.K_PAGEUP:
                magnetic_field = pygame.math.clamp(magnetic_field + 0.01, -1, 1)
            elif event.key == pygame.K_PAGEDOWN:
                magnetic_field = pygame.math.clamp(magnetic_field - 0.01, -1, 1)


def update() -> None:
    pygame.display.set_caption(str(round(clock.get_fps())))
    pygame.display.flip()
    clock.tick(FPS)


def draw() -> None:
    surface.fill(BLUE)
    mobile.draw()
    draw_table()


def draw_table() -> None:
    surface.blit(font.render(f"Electric field: {electric_field.y:.2f} V/m", True, BLACK), (200, 40))
    surface.blit(font.render(f"Magnetic field: {magnetic_field:.2f} T", True, BLACK), (200, 80))

    kinetic_energy: float = mobile.mass * mobile.velocity.length_squared() / 2 * 10 ** 6
    surface.blit(font.render(f"Kinetic energy: {kinetic_energy:.2f} μJ", True, BLACK), (200, 120))


def calculate_electric_field() -> pygame.Vector2:
    return pygame.Vector2(electric_field.x, -electric_field.y)


def calculate_cyclotron_field(delta: float) -> None:
    global electric_field, alpha

    PI_2: float = math.pi * 2

    if magnetic_field == 0: return

    T = (PI_2 * mobile.mass) / (mobile.charge * magnetic_field)
    alpha += PI_2 * delta / T
    alpha = math.fmod(alpha, PI_2)

    electric_field.y = A * math.sin(alpha)


if __name__ == "__main__": main()
