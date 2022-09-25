import tkinter
import tkinter as tk
from tkinter import ttk
from tkinter import colorchooser

import numpy as np
from utils import constants, styles, util_funcs

import enum


class PainterStatus(enum.IntEnum):
    pen = 0
    bucket = 1
    bresenham_line = 2
    wu_line = 3
    magic_wand = 4


class Window(tk.Tk):
    filename: str
    data: np.ndarray
    colors: np.array = np.asarray([constants.DEFAULT_COLOR, constants.DEFAULT_COLOR2],
                                  dtype=np.uint8)  # .astype(np.uint8) #np.zeros((2, 3), dtype=np.uint8)
    str_colors: np.array = np.array([util_funcs.rgb_tuple_to_str(colors[0]),
                                     util_funcs.rgb_tuple_to_str(colors[1])])
    mod: PainterStatus = PainterStatus.pen

    def __init__(self):
        super().__init__()
        self.config(bg=styles.win["darky-darky"])
        self.resizable(False, False)
        self.create_widgets()
        self.pen_width = 1
        self.data = np.full((constants.CANV_HEIGHT, constants.CANV_WIDTH, 3), 255, np.uint8)
        self.style = ttk.Style(self)
        self.define_styles()
        self.mainloop()

    def create_widgets(self):
        self.canvas = tk.Canvas(self, width=constants.CANV_WIDTH, height=constants.CANV_HEIGHT, bg='white', highlightthickness=0, borderwidth=0, relief='flat') #remove the attributes starting with highlightthickness to restore border

        self.f_toolbar = ttk.Frame(self, width=100, style="Toolbar.TFrame")
        ## color pickers for colors[0] and colors[1]
        self.f_draw_color = ttk.Frame(self.f_toolbar, width=100, height=50, style="ColorPicker0.TFrame", cursor="hand2")
        self.f_fill_color = ttk.Frame(self.f_toolbar, width=100, height=50, style="ColorPicker1.TFrame", cursor="hand2")

        self.l_pen_width = ttk.Label(self.f_toolbar, text="Pen width", style="Instr.TLabel")
        self.sc_pen_width = ttk.Scale(self.f_toolbar, from_=1, to=100, length=100,  orient=tk.HORIZONTAL, command=self.change_pen_width)

        self.b_chsmod1 = ttk.Button(self.f_toolbar, text="Pen", command=lambda: self.set_mode(PainterStatus.pen),
                                    style="WTF.TButton", cursor="hand2")
        self.b_chsmod2 = ttk.Button(self.f_toolbar, text="Bucket", command=lambda: self.set_mode(PainterStatus.bucket),
                                    style="WTF.TButton", cursor="hand2")
        self.b_bresenham_line = ttk.Button(self.f_toolbar, text="Bresenham",
                                           command=lambda: self.set_mode(PainterStatus.bresenham_line),
                                           style="WTF.TButton", cursor="hand2")
        self.b_wu_line = ttk.Button(self.f_toolbar, text="Wu", command=lambda: self.set_mode(PainterStatus.wu_line),
                                    style="WTF.TButton", cursor="hand2")
        self.b_magic_wand = ttk.Button(self.f_toolbar, text="Magic wand",
                                       command=lambda: self.set_mode(PainterStatus.magic_wand),
                                       style="WTF.TButton", cursor="hand2")

        # well the name of current instrument will be inserted here
        # but current stylesheet highlights the button of picked mode as well
        # so this label has no use at all...
        self.l_current_instrument = ttk.Label(self.f_toolbar, text="<no instrument>", style="Instr.TLabel")

        self.canvas.grid(row=0, column=0, padx=constants.WINDOW_BORDER, pady=constants.WINDOW_BORDER, sticky="w")
        self.f_toolbar.grid(row=0, column=1, padx=constants.WINDOW_BORDER, pady=0, sticky="ns")

        toolbar_elems = [self.f_draw_color,
                         self.f_fill_color,

                         self.l_pen_width,
                         self.sc_pen_width,

                         self.b_chsmod1,
                         self.b_chsmod2,
                         self.b_bresenham_line,
                         self.b_wu_line,
                         self.b_magic_wand,
                         self.l_current_instrument]
        for i, node in enumerate(toolbar_elems):
            if i == 2: #self.l_pen_width
                node.grid(row=i, column=0, padx=0, pady=0, sticky="e")
                continue
            node.grid(row=i, column=0, padx=0, pady=constants.WINDOW_BORDER, sticky="e")
        self.l_current_instrument.grid(row=len(toolbar_elems) - 1, column=0, padx=0, pady=constants.WINDOW_BORDER,
                                       sticky="s")
        # self.canvas.bind('<ButtonRelease-1>', self.mouse_release)

        self.f_draw_color.bind("<Button-1>", lambda _: self.pick_color(PainterStatus.pen))
        self.f_fill_color.bind("<Button-1>", lambda _: self.pick_color(PainterStatus.bucket))
        self.canvas.bind("<Button-1>", self.savePosition)
        self.canvas.bind("<B1-Motion>", self.mouse_click_handler)
        # self.canvas.bind('<Button-1>', self.mouse_fill_handler)
        # self.canvas.bind('<Motion>', self.mouse_draw_handler)

    def change_pen_width(self, new_width):
        self.pen_width = new_width

    def define_styles(self):
        self.style.theme_settings("clam", {
            "WTF.TButton": {
                "configure": {"relief": "flat",
                              "anchor": "w",
                              },
            },
            "Toolbar.TFrame": {
                "configure": {"background": styles.win["darky-darky"],
                              "borderwidth": 0},
            },
            "TFrame": {
                "configure": {"relief": "sunken",
                              "borderwidth": 1},
            },
            "Instr.TLabel": {
                "configure": {
                    "foreground": styles.win["darky-lighty"],
                    "background": styles.win["darky-darky"]},
            }
        })
        self.style.theme_use("clam")
        for i in range(2):
            self.style.configure("ColorPicker" + str(i) + ".TFrame", background=self.str_colors[i])

    def pick_color(self, instrument_type: PainterStatus):
        color = colorchooser.askcolor(title="Choose a color")
        if (color == (None, None)):
            return
        self.colors[instrument_type] = np.asarray(color[0])
        self.str_colors[instrument_type] = util_funcs.rgb_tuple_to_str(self.colors[instrument_type])
        self.style.configure("ColorPicker" + str(int(instrument_type)) + ".TFrame", background=color[1])
        # print('new color = ', color, 'reset self colors = ', self.colors, 'reset self str_colors = ', self.str_colors)

    def set_mode(self, ps: PainterStatus):
        self.mod = ps
        #print(self.l_current_instrument['text'])
        self.l_current_instrument['text'] = str(ps.name)
        #print(self.mod)

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

    # last position of cursor
    prev: (tuple[int, int] | None) = None

    def savePosition(self, event):
        self.prev = (event.x, event.y)

    def mouse_fill_handler(self, event):
        if (self.mod != PainterStatus.bucket):
            return

        self.fill(event.x, event.y)

    def mouse_click_handler(self, event):
        # print('event = ', event, 'self.colors = ', self.colors[PainterStatus.draw],'event.state = ', event.state, 'self.prev = ', self.prev, 'self.mode = ',self.mod)
        if (event.x >= constants.CANV_WIDTH or event.x < 0 or event.y >= constants.CANV_HEIGHT or event.y < 0):
            self.prev = None
            return

        # if (event.state != 264):  # 264: left button is down and moving ## sorry it does  not work
        #    self.prev = None
        #    return

        if (self.prev == None):
            self.prev = (event.x, event.y)

        # dont know yet if it would be convenient to use match case instead of picking right fn from like fn_array
        match self.mod:
            case PainterStatus.pen:
                self.data[event.x][event.y] = self.colors[self.mod].copy()
                self.canvas.create_line(self.prev[0], self.prev[1], event.x, event.y,
                                        fill=self.str_colors[PainterStatus.pen], width=self.pen_width)
                self.savePosition(event)
            case PainterStatus.bucket:
                self.mouse_fill_handler(event)
            case PainterStatus.bresenham_line:
                pass
            case PainterStatus.wu_line:
                pass
            case PainterStatus.magic_wand:
                pass
            case _:
                return

        # self.canvas.create_line(event.x, event.y, self.prev[0], self.prev[1], fill=self.color[1], width=1)
        # self.line(event.x, event.y, self.prev[0], self.prev[1])

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
