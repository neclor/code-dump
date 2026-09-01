from casioplot import *
from casioplot.types import Color
from time import *

WHITE: Color = (255, 255, 255)

BLACK: Color = (0, 0, 0)

def render() -> None:
    show_screen()
    clear_screen()

def draw_rect(x, y, width, height, color: Color = BLACK):
    for i in range(x, x + width):
        for j in range(y, y + height):
            set_pixel(i, j, color)

def main() -> None:
    while True:
        draw_rect(32, 16, 64, 32)
        render()
        sleep(0.016)

        draw_rect(0, 0, 64, 32)
        render()
        sleep(0.016)

main()
