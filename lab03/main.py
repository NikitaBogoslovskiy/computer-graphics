import tkinter as tk
from tkinter import ttk
from tkinter import colorchooser

from numba import njit, prange
import numpy as np
from PIL import Image
from PIL import ImageTk as itk
import constants
import styles

import enum
class PainterStatus(enum.Enum):
    draw = 0
    fill = 1
    bresenham_line = 2
    wu_line = 3

class Window(tk.Tk):
    filename: str
    data: np.ndarray
    colors: np.array = np.zeros((2, 3), dtype=np.uint8)
    str_colors: np.array = np.array([constants.DEFAULT_COLOR, constants.DEFAULT_COLOR])
    mod: PainterStatus = PainterStatus.draw  # 0 - draw, 1 - fill

    def __init__(self):
        super().__init__()
        self.config(bg=styles.win["darky-darky"])
        self.resizable(False, False)
        self.create_widgets()
        self.data = np.full((constants.CANV_HEIGHT, constants.CANV_WIDTH, 3), 255, np.uint8)
        self.style = ttk.Style(self)
        self.define_styles()
        self.mainloop()

    def create_widgets(self):
        self.canvas = tk.Canvas(self, width=constants.CANV_WIDTH, height=constants.CANV_HEIGHT, bg='white')
        self.f_toolbar = ttk.Frame(self, width=100, style="Toolbar.TFrame")

        self.f_current_color = ttk.Frame(self.f_toolbar, width=100,  height=50, style="ColorPickerSample.TFrame")
        self.b_chscolor = ttk.Button(self.f_toolbar, text="Pick color", command=self.pick_color, style="WTF.TButton", cursor="hand2")
        self.b_chsmod1 = ttk.Button(self.f_toolbar, text="Draw", command=self.mod_draw, style="WTF.TButton", cursor="hand2")
        self.b_chsmod2 = ttk.Button(self.f_toolbar, text="Fill", command=self.mod_fill, style="WTF.TButton", cursor="hand2")

        self.canvas.grid(row=0, column=0, padx=constants.WINDOW_BORDER, pady=constants.WINDOW_BORDER, sticky="w")
        self.f_toolbar.grid(row=0, column=1, padx=0, pady=0, sticky="ne")
        for i, node in enumerate([self.f_current_color, self.f_current_color, self.b_chscolor, self.b_chsmod1, self.b_chsmod2]):
            node.grid(row=i, column=0, padx=0, pady=constants.WINDOW_BORDER, sticky="e")

        # self.canvas.bind('<ButtonRelease-1>', self.mouse_release)
        self.canvas.bind('<Button-1>', self.mouse_fill_handler)
        self.canvas.bind('<Motion>', self.mouse_draw_handler)

    def define_styles(self):
        self.style.theme_settings("clam", {
            "WTF.TButton": {
                "configure": { "cursor": "hand2",
                               "relief": "flat"},
            },
            "Toolbar.TFrame": {
                "configure": {"background": styles.win["darky-darky"]},
            }
        })
        self.style.theme_use("clam")
        self.style.configure("ColorPickerSample.TFrame", background=constants.DEFAULT_COLOR)


    def pick_color(self):
        color = colorchooser.askcolor(title="Choose a color")
        print(color)
        if (color == (None, None)):
            return

        self.style.configure("ColorPickerSample.TFrame", background=color[1])
        self.colors[self.mod] = np.asarray(color[0])
        self.str_colors[self.mod] = '#%02x%02x%02x' % (
        self.colors[self.mod][0], self.colors[self.mod][1], self.colors[self.mod][2])

    def mod_draw(self):
        self.mod = 0
        #self.b_chsmod1.config(font=('Times', 14, 'bold'))
        #self.b_chsmod2.config(font=('Times', 14, 'normal'))

    def mod_fill(self):
        self.mod = 1
        #self.b_chsmod2.config(font=('Times', 14, 'bold'))
        #self.b_chsmod1.config(font=('Times', 14, 'normal'))

    def fill(self, x, y, filled_color=None):
        if (np.array_equal(filled_color, None)):
            filled_color = self.data[x][y].copy()

        rdx = 0
        while (x + rdx + 1 < 800 and rgb_equal(self.data[x + rdx + 1][y], filled_color)):
            rdx += 1

        ldx = 0
        while (x - ldx - 1 >= 0 and rgb_equal(self.data[x - ldx - 1][y], filled_color)):
            ldx += 1

        self.line(x - ldx, y, x + rdx + 1, y)
        for i in range(x - ldx, x + rdx + 1):
            if (y + 1 < 800 and rgb_equal(filled_color, self.data[i][y + 1])):
                self.fill(i, y + 1, filled_color)
            if (y - 1 >= 0 and rgb_equal(filled_color, self.data[i][y - 1])):
                self.fill(i, y - 1, filled_color)

    prev: (tuple[int, int] | None) = None

    def mouse_fill_handler(self, event):
        if (self.mod != 1):
            return

        self.fill(event.x, event.y)

    def mouse_draw_handler(self, event):
        if (event.x >= 800 or event.x < 0 or event.y >= 800 or event.y < 0):
            self.prev = None
            return

        if (event.state != 264):  # 264: left button is down and moving
            self.prev = None
            return

        if (self.prev == None):
            self.prev = (event.x, event.y)

        if (self.mod != 0):
            return

        # self.canvas.create_line(event.x, event.y, self.prev[0], self.prev[1], fill=self.color[1], width=1)
        self.line(event.x, event.y, self.prev[0], self.prev[1])
        self.prev = (event.x, event.y)

    def plot(self, x: int, y: int):
        self.data[x][y] = self.colors[self.mod].copy()
        self.canvas.create_line(x, y, x + 1, y + 1, fill=self.str_colors[self.mod])

    def line(self, x0: int, y0: int, x1: int, y1: int):
        dx: int = abs(x1 - x0)
        dy: int = abs(y1 - y0)
        err: int = 0

        if (dx == 0 or dy / dx > 1):
            derr: int = (dx + 1)
            x: int = x0

            dirx: int = x1 - x0
            if (dirx > 0):
                dirx = 1
            if (dirx < 0):
                dirx = -1

            step = 1
            if (y0 > y1):
                step = -1

            for y in range(y0, y1, step):
                self.plot(x, y)
                err = err + derr
                if (err >= (dy + 1)):
                    x = x + dirx
                    err = err - (dy + 1)
        else:
            derr: int = (dy + 1)
            y: int = y0

            diry: int = y1 - y0
            if (diry > 0):
                diry = 1
            if (diry < 0):
                diry = -1

            step = 1
            if (x0 > x1):
                step = -1

            for x in range(x0, x1, step):
                self.plot(x, y)
                err = err + derr
                if (err >= (dx + 1)):
                    y = y + diry
                    err = err - (dx + 1)


def rgb_equal(a, b):
    if (a[0] == b[0] and a[1] == b[1] and a[2] == b[2]):
        return True
    return False


if (__name__ == '__main__'):
    Window()