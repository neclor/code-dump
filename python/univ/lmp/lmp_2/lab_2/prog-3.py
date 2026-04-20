import sys
import math
import pygame


K: float = 8.9876 * (10 ** 9)
ABS_OBJECT_CHARGE: float = 10 ** -7
ABS_MOBILE_CHARGE: float = ABS_OBJECT_CHARGE


BLUE: pygame.Color = pygame.Color("#7fbfff")
RED: pygame.Color = pygame.Color("#ff0000")
BLACK: pygame.Color = pygame.Color("#000000")


RESOLUTION: tuple[int, int] = (1600, 900)  # (800, 600)
FPS: int = 60


class Obj:
    def __init__(self: "Obj", new_position: pygame.Vector2, new_charge: float) -> None:
        self.position: pygame.Vector2 = new_position
        self.charge: float = new_charge


    def draw(self: "Obj") -> None:
        pygame.draw.circle(surface, RED if self.charge > 0 else BLACK, self.position, 10)


class Mobile(Obj):
    def __init__(self: "Mobile", new_position: pygame.Vector2, new_charge: float) -> None:
        super().__init__(new_position, new_charge)
        self.velocity: pygame.Vector2 = pygame.Vector2()
        self.mass: float = 10 ** -10


    def draw(self: "Mobile") -> None:
        pygame.draw.circle(surface, RED if self.charge > 0 else BLACK, self.position, 10, 4)


    def update(self: "Mobile", delta: float) -> None:
        coulomb_force: pygame.Vector2 = self.charge * calculate_field_strength(self.position)
        acceleration: pygame.Vector2 = coulomb_force / self.mass

        self.position += self.velocity * delta + acceleration * (delta ** 2) / 2
        self.velocity += acceleration * delta


surface: pygame.Surface
clock: pygame.time.Clock
font: pygame.font.Font


objects: list[Obj] = []
mobile: Mobile | None = None


def main() -> None:
    init()
    run()


def init() -> None:
    global surface, clock, font
    pygame.init()
    clock = pygame.time.Clock()
    surface = pygame.display.set_mode(RESOLUTION)
    font = pygame.font.SysFont("monospace", 16)


def run() -> None:
    while True:
        check_input()
        if mobile is not None: mobile.update(clock.get_time() / 1000)
        draw()
        update()


def check_input() -> None:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit()
        elif event.type == pygame.MOUSEBUTTONDOWN:
            if event.button == 1:
                objects.append(Obj(pygame.Vector2(event.pos), ABS_OBJECT_CHARGE))
            elif event.button == 2:
                if len(objects) != 0: objects.pop(0)
            elif event.button == 3:
                objects.append(Obj(pygame.Vector2(event.pos), -ABS_OBJECT_CHARGE))
        elif event.type == pygame.KEYDOWN:
            global mobile
            if event.key == pygame.K_p:
                mobile = Mobile(pygame.Vector2(pygame.mouse.get_pos()), ABS_MOBILE_CHARGE)
            elif event.key == pygame.K_n:
                mobile = Mobile(pygame.Vector2(pygame.mouse.get_pos()), -ABS_MOBILE_CHARGE)


def update() -> None:
    pygame.display.set_caption(str(round(clock.get_fps())))
    pygame.display.flip()
    clock.tick(FPS)


def draw() -> None:
    surface.fill(BLUE)
    for obj in objects: obj.draw()
    if mobile is not None: mobile.draw()
    draw_table()


def draw_table() -> None:
    electric_potential: float = calculate_electric_potential(pygame.Vector2(pygame.mouse.get_pos()))
    surface.blit(font.render(f"Electric potential: {electric_potential:.2f} V", True, BLACK), (200, 160))
    if mobile is None: return
    kinetic_energy: float = mobile.mass * mobile.velocity.length_squared() / 2 * 10 ** 6
    surface.blit(font.render(f"Kinetic energy: {kinetic_energy:.2f} μJ", True, BLACK), (200, 40))
    potential_energy: float = calculate_potential_energy(mobile.position, mobile.charge) * 10 ** 6
    surface.blit(font.render(f"Potential energy: {potential_energy:.2f} μJ", True, BLACK), (200, 80))
    total_energy: float = kinetic_energy + potential_energy
    surface.blit(font.render(f"Total energy: {total_energy:.2f} μJ", True, BLACK), (200, 120))


def calculate_field_strength(position: pygame.Vector2) -> pygame.Vector2:
    field_strength_vector: pygame.Vector2 = pygame.Vector2()
    for obj in objects:
        delta_vector: pygame.Vector2 = position - obj.position
        distance: float = delta_vector.length()
        if distance < 10: continue
        normalized_delta_vector: pygame.Vector2 = delta_vector.normalize()
        field_strength: float = K * obj.charge / (distance ** 2)
        field_strength_vector += normalized_delta_vector * field_strength

    return field_strength_vector


def calculate_potential_energy(position: pygame.Vector2, charge: float) -> float:
    potential_energy: float = 0
    for obj in objects:
        delta_vector: pygame.Vector2 = position - obj.position
        distance: float = delta_vector.length()
        if distance < 10: continue
        potential_energy += obj.charge / distance

    return K * charge * potential_energy


def calculate_electric_potential(position: pygame.Vector2) -> float:
    electric_potential: float = calculate_potential_energy(position, 1)
    if mobile is None: return electric_potential
    delta_vector: pygame.Vector2 = position - mobile.position
    distance: float = delta_vector.length()
    if distance == 0: return math.inf if mobile.charge > 0 else -math.inf
    electric_potential += K * mobile.charge / distance
    return electric_potential


if __name__ == "__main__": main()
