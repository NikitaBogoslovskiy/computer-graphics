import math
import tkinter as tk
from tkinter import ttk, colorchooser, filedialog as fd
import numpy as np
from utils import constants, styles, util_funcs
from utils.pixel_comparable_by_coords import Detour, PixelComparableByCoords as UberPix, ClockwiseTurn90
from PIL import Image, ImageGrab, ImageTk as itk
from numba import njit, prange
import enum
from collections import deque
from numba import njit
import heapq


class PainterStatus(enum.IntEnum):
    pen = 0
    bucket = 1
    bresenham_line = 2
    bresenham_line2 = 22
    wu_line = 3
    wu_line2 = 33
    magic_wand = 4
    stamp = 5


class Window(tk.Tk):
    filename: str
    data: np.ndarray
    colors: np.array = np.asarray([constants.DEFAULT_COLOR, constants.DEFAULT_COLOR2],
                                  dtype=np.uint8)  # .astype(np.uint8) #np.zeros((2, 3), dtype=np.uint8)
    str_colors: np.array = np.array([util_funcs.rgb_tuple_to_str(colors[0]),
                                     util_funcs.rgb_tuple_to_str(colors[1])])
    mod: PainterStatus = PainterStatus.pen
    pen_width: int = 1
    loaded_image: np.ndarray = None
    new_image: np.ndarray
    stamp: tk.PhotoImage
    is_stamp: bool = False

    def __init__(self):
        super().__init__()
        self.config(bg=styles.win["darky-darky"])
        self.resizable(False, False)
        self.create_widgets()
        self.data = np.full((constants.CANV_WIDTH, constants.CANV_HEIGHT, 3), 255, np.uint8)
        self.style = ttk.Style(self)
        self.define_styles()
        self.mainloop()

    def create_widgets(self):
        self.canvas = tk.Canvas(self, width=constants.CANV_WIDTH, height=constants.CANV_HEIGHT, bg='white',
                                highlightthickness=0, borderwidth=0,
                                relief='ridge')  # remove the attributes starting with highlightthickness to restore border
        self.f_sidebar = ttk.Frame(self, width=100, style="Sidebar.TFrame")

        self.f_toolbar = ttk.Frame(self.f_sidebar, width=100, style="Toolbar.TFrame")

        ## color pickers for colors[0] and colors[1]
        self.f_draw_color = ttk.Frame(self.f_toolbar, width=100, height=50, style="ColorPicker0.TFrame", cursor="hand2")
        self.f_fill_color = ttk.Frame(self.f_toolbar, width=100, height=50, style="ColorPicker1.TFrame", cursor="hand2")

        self.l_pen_width = ttk.Label(self.f_toolbar, text="Pen width: " + str(self.pen_width), style="Instr.TLabel")
        self.sc_pen_width = ttk.Scale(self.f_toolbar, from_=1, to=100, length=100, orient=tk.HORIZONTAL,
                                      command=self.change_pen_width)

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
        self.b_stamp = ttk.Button(self.f_toolbar, text="Stamp",
                                       command=lambda: self.set_mode(PainterStatus.stamp),
                                       style="WTF.TButton", cursor="hand2")

        self.f_bottombar = ttk.Frame(self.f_sidebar, width=100, style="Bottombar.TFrame")
        # well the name of current instrument will be inserted here
        # but current stylesheet highlights the button of picked mode as well
        # so this label has no use at all...
        self.b_clear_canvas = ttk.Button(self.f_bottombar, text='Clear canvas', style="WTF.TButton", command=self.clear_canvas,
                                      cursor="hand2")
        self.b_open_file = ttk.Button(self.f_bottombar, text='Load image', style="WTF.TButton", command=self.open_file,
                                      cursor="hand2")
        self.b_save_file = ttk.Button(self.f_bottombar, text='Save image', style="WTF.TButton",
                                      command=self.save_img_from_canvas, cursor="hand2")
        # self.l_current_instrument = ttk.Label(self.f_bottombar, text="<no instrument>", style="Instr.TLabel")

        self.canvas.grid(row=0, column=0, padx=constants.WINDOW_BORDER, pady=constants.WINDOW_BORDER, sticky="w")

        self.f_sidebar.grid(row=0, column=1, padx=constants.WINDOW_BORDER, pady=0, sticky="ns")
        self.f_sidebar.rowconfigure(0, weight=3)
        self.f_sidebar.rowconfigure(1, weight=0)

        self.f_toolbar.grid(row=0, column=0, padx=0, pady=0, sticky="n")
        self.f_bottombar.grid(row=1, column=0, padx=0, pady=0, sticky="s")

        # think there is a way to just get the children list but whatever
        toolbar_elems = [self.f_draw_color,
                         self.f_fill_color,
                         self.l_pen_width,
                         self.sc_pen_width,
                         self.b_chsmod1,
                         self.b_chsmod2,
                         self.b_bresenham_line,
                         self.b_wu_line,
                         self.b_magic_wand,
                         self.b_stamp,
                         ]
        for i, node in enumerate(toolbar_elems):
            if i == 2:  # self.l_pen_width # well maybe there is something like
                node.grid(row=i, column=0, padx=0, pady=0, sticky="e")
                continue
            node.grid(row=i, column=0, padx=0, pady=constants.WINDOW_BORDER, sticky="e")

        # think there is a way to just get the children list but whatever
        bottombar_elems = [
            self.b_clear_canvas,
            self.b_open_file,
            self.b_save_file,
            # self.l_current_instrument,
        ]
        for i, node in enumerate(bottombar_elems):
            node.grid(row=i, column=0, padx=0, pady=constants.WINDOW_BORDER, sticky="e")

        # self.canvas.bind('<ButtonRelease-1>', self.mouse_release)
        self.f_draw_color.bind("<Button-1>", lambda _: self.pick_color(PainterStatus.pen))
        self.f_fill_color.bind("<Button-1>", lambda _: self.pick_color(PainterStatus.bucket))
        self.canvas.bind("<Button-1>", self.mouse_click_handler)
        self.canvas.bind("<ButtonRelease-1>", self.mouse_b1_release_handler)
        self.canvas.bind("<B1-Motion>", self.mouse_move_handler)

    def clear_canvas(self):
        self.data = np.full((constants.CANV_WIDTH, constants.CANV_HEIGHT, 3), 255, np.uint8)
        self.update_image(itk.PhotoImage(Image.fromarray(self.data)))

    def open_file(self):
        self.filename = fd.askopenfilename()
        if not self.filename:
            return

        img = Image.open(self.filename)
        # self.data = np.asarray(img).copy()
        self.loaded_image = np.asarray(img).copy()
        # self.canvas.config(width=img.width, height=img.height)
        # self.title(self.filename)
        # self.update_image()

    def save_file(self):
        Image.fromarray(self.data).save(fd.asksaveasfilename())

    # well another option to save processed image
    # to set another func use callback for 'self.b_save_file' widget
    def save_img_from_canvas(self):
        x = self.winfo_rootx() + self.canvas.winfo_x()
        y = self.winfo_rooty() + self.canvas.winfo_y()
        x1 = x + self.canvas.winfo_width()
        y1 = y + self.canvas.winfo_height()
        ImageGrab.grab().crop((x, y, x1, y1)).save(fd.asksaveasfilename())

    def update_image(self, new_image: Image):
        self.image = new_image
        self.canvas.create_image(0, 0, anchor=tk.NW, image=self.image)

    def show_stamp(self, x, y):
        self.stamp = itk.PhotoImage(Image.fromarray(self.new_image).convert("RGBA"))
        self.canvas.create_image(x, y, anchor=tk.NW, image=self.stamp)

    def change_pen_width(self, new_width):
        self.pen_width = int(float(new_width))
        self.l_pen_width['text'] = "Pen width: " + str(self.pen_width)

    def define_styles(self):
        self.style.theme_settings("clam", {
            "WTF.TButton": {
                "configure": {"relief": "raised",
                              "anchor": "w",
                              },
            },
            "FileBtn.TButton": {
                "configure": {"relief": "raised",
                              "anchor": "we",
                              },
            },
            "ColorPicker0.TFrame": {
                "configure": {"borderwidth": 1},
            },
            "ColorPicker1.TFrame": {
                "configure": {"borderwidth": 1},
            },
            "TFrame": {
                "configure": {
                    "background": styles.win["darky-darky"],
                    "relief": "sunken",
                    "borderwidth": 0},
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

    def set_mode(self, ps: PainterStatus):
        self.mod = ps
        # print(self.l_current_instrument['text'])
        # self.l_current_instrument['text'] = ps.name

    # last position of cursor
    prev: (tuple[int, int] | None) = None

    def savePosition(self, event):
        self.prev = (event.x, event.y)

    def mouse_click_handler(self, event):
        if event.x >= constants.CANV_WIDTH or event.x < 0 or event.y >= constants.CANV_HEIGHT or event.y < 0:
            self.prev = None
            return
        # print('hello from mouse_click_handler ', event, self.mod)
        #self.savePosition(event)
        #if self.prev == None:
        #    self.savePosition(event)

        match self.mod:
            case PainterStatus.pen:
                self.prev = None
                self.plot(event.x, event.y, 0, True)
            case PainterStatus.stamp:
                self.prev = None
                self.plot_stamp((event.x, event.y))
            case PainterStatus.bucket:
                self.fill(event.x, event.y)
            case PainterStatus.bresenham_line:
                self.savePosition(event)
                self.set_mode(PainterStatus.bresenham_line2)
            case PainterStatus.bresenham_line2:
                print(event)
                self.bresenham_line(self.prev[0], self.prev[1], event.x, event.y)
                self.set_mode(PainterStatus.bresenham_line)
                self.prev = None
            case PainterStatus.wu_line:
                print(event)
                self.savePosition(event)
                self.set_mode(PainterStatus.wu_line2)
            case PainterStatus.wu_line2:
                self.wu_line(self.prev[0], self.prev[1], event.x, event.y)
                self.set_mode(PainterStatus.wu_line)
                self.prev = None
            case PainterStatus.magic_wand:
                self.magic_wand(event.x, event.y)
            case _:
                return

    def mouse_move_handler(self, event):
        if event.x >= constants.CANV_WIDTH or event.x < 0 or event.y >= constants.CANV_HEIGHT or event.y < 0:
            # self.prev = None
            return

        if self.prev == None:
            self.savePosition(event)

        # dont know yet if it would be convenient to use match case instead of picking right fn from like fn_array
        match self.mod:
            case PainterStatus.pen:
                self.data[event.x][event.y] = self.colors[self.mod].copy()
                self.bresenham_line(self.prev[0], self.prev[1], event.x, event.y, True)
                self.savePosition(event)
            case PainterStatus.bresenham_line:
                self.set_mode(PainterStatus.bresenham_line2)
            case PainterStatus.bresenham_line2:
                self.update_image(itk.PhotoImage(Image.fromarray(self.data.transpose((1, 0, 2)))))
                self.bresenham_line(self.prev[0], self.prev[1], event.x, event.y, False)
            case PainterStatus.wu_line:
                pass
            case _:
                return

    def mouse_b1_release_handler(self, event):
        if event.x >= constants.CANV_WIDTH or event.x < 0 or event.y >= constants.CANV_HEIGHT or event.y < 0:
            self.prev = None
            return

        match self.mod:
            case PainterStatus.pen:
                self.prev = None
            case PainterStatus.bresenham_line2:
                self.update_image(itk.PhotoImage(Image.fromarray(self.data.transpose((1, 0, 2)))))
                self.bresenham_line(self.prev[0], self.prev[1], event.x, event.y, True)
                self.set_mode(PainterStatus.bresenham_line)
                self.prev = None
            case _:
                return

    def bresenham_line(self, x0: int, y0: int, x1: int, y1: int, save_to_data: bool):
        alpha = 666  # gradient
        if x1 - x0 != 0:
            alpha = abs((y1 - y0) / (x1 - x0))
        if alpha > 1:  # if gradient is >1, we use the same algorithm, but axis Ox and Oy exchange roles
            x0, y0, x1, y1 = y0, x0, y1, x1
        dx = x1 - x0
        if dx < 0:  # if x1 < x0, we use the same algorithm, but for (x1, y1) -> (x0, y0) pair of points, so from now on we assume x0 < x1
            x0, y0, x1, y1 = x1, y1, x0, y0
            dx = abs(dx)
        dy = y1 - y0
        sign_dy = 1 if dy > 0 else - 1  # line direction: north or south
        di = sign_dy * 2 * dy - dx
        yi = 0  # assuming the first pixel is (0, 0)
        for xi in range(dx + 1):
            if alpha > 1:
                self.plot(y0 + yi, x0 + xi, 0, save_to_data)  # parallel translation by (y0, x0) vector
                # print(y0 + yi, x0 + xi)
            else:
                self.plot(x0 + xi, y0 + yi, 0, save_to_data)  # parallel translation by (x0, y0) vector
                # print(x0 + xi, y0 + yi)
            if di > 0:
                yi += sign_dy
                di += 2 * dy * sign_dy - 2 * dx
            else:
                # yi does not change
                di += 2 * dy * sign_dy

    def magic_wand(self, x: int, y: int):
        pix_list = []
        event_col = self.data[x][y].copy()

        while rgb_equal(self.data[x][y], event_col):
            x += 1
            if x >= constants.CANV_WIDTH:
                return
        pix_list.append(UberPix(x, y))
        first_pix_coords = (x, y)
        outline_col = self.data[x][y].copy()

        new_dir = 6
        new_dir, x, y = self.get_new_pix(new_dir, x, y, outline_col)

        while x != first_pix_coords[0] or y != first_pix_coords[1]:
            pix_list.append(UberPix(x, y))
            self.plot(x, y, 0, False)
            new_dir, x, y = self.get_new_pix(new_dir, x, y, outline_col)

        #pix_list.sort(key=lambda x: x)

    def get_new_pix(self, new_dir, x, y, outline_col):
        for i in [j for j in range(new_dir, 8)] + [j for j in range(new_dir)]:  # lol
            direction = Detour[i]
            new_x, new_y = x + direction.dx, y + direction.dy
            if constants.CANV_WIDTH > new_x >= 0 and constants.CANV_HEIGHT > new_y >= 0 and rgb_equal(self.data[new_x][new_y], outline_col):
                new_dir = ClockwiseTurn90[i]
                x, y = new_x, new_y
                return new_dir, x, y

    def plot(self, x: int, y: int, color_ind: int, save_to_data: bool):
        if save_to_data:
            self.data[x][y] = self.colors[color_ind].copy()
        self.canvas.create_line(x, y, x + 1, y, fill=self.str_colors[color_ind], width=self.pen_width)

    def plot_stamp(self, start_point):
        if self.loaded_image is None:
            return
        coordinates = np.column_stack(np.nonzero(np.sum(self.data, axis=2) != 765))
        filled_pixels = set()
        for p in coordinates.tolist():
            filled_pixels.add((p[0], p[1]))
        coordinates_set = filled_pixels.copy()
        new_set = self.fill_within_area(coordinates, start_point, filled_pixels)
        new_set = new_set.difference(coordinates_set)
        indices = np.array(list(new_set))
        min_x = np.min(indices[:, 0])
        min_y = np.min(indices[:, 1])
        max_x = np.max(indices[:, 0])
        max_y = np.max(indices[:, 1])
        indices -= np.array([min_x, min_y])
        width = max_x - min_x + 1
        height = max_y - min_y + 1
        new_image = self.loaded_image.copy()
        if new_image.shape[0] < height:
            new_image = np.vstack([new_image] * (height // new_image.shape[0] + 1))
        if new_image.shape[1] < width:
            new_image = np.hstack([new_image] * (width // new_image.shape[1] + 1))
        new_image = new_image[:height, :width, :]
        new_channel = np.zeros((height, width, 1))
        new_channel[indices[:, 1], indices[:, 0]] = 255
        new_image = np.concatenate([new_image, new_channel], axis=2)
        self.new_image = new_image.astype(np.uint8)
        self.show_stamp(min_x, min_y)

    def fill_within_area(self, coordinates, point, filled_pixels):
        q = deque()
        q.append(point)
        while True:
            if len(q) == 0:
                break
            start_point = q.popleft()
            if start_point in filled_pixels or start_point[1] < 0 or start_point[1] >= constants.CANV_HEIGHT:
                continue
            points_in_line = coordinates[np.where(coordinates[:, 1] == start_point[1])]
            all_left_points = points_in_line[points_in_line[:, 0] < start_point[0]]
            if len(all_left_points) == 0:
                left_x = -1
            else:
                left_x = np.max(all_left_points[:, 0])
            all_right_points = points_in_line[points_in_line[:, 0] > start_point[0]]
            if len(all_right_points) == 0:
                right_x = constants.CANV_WIDTH
            else:
                right_x = np.min(all_right_points[:, 0])
            for i in range(left_x + 1, right_x):
                filled_pixels.add((i, start_point[1]))
                q.append((i, start_point[1] + 1))
                q.append((i, start_point[1] - 1))
        return filled_pixels

    def mouse_fill_handler(self, event):
        pass

    def wu_line(self, x0 : int, y0 : int, x1 : int, y1 : int):
        dx = abs(x1 - x0)
        dy = abs(y1 - y0)
        
        if(dx == 0 or dy == 0):
            self.canvas.create_line(x0, y0, x1, y1)
            return
        
        err = 0.
        
        if(dx == 0 or dy/dx > 1):
            derr = (dx + 1.)
            x = x0
            
            dirx = x1 - x0
            if(dirx > 0):
                dirx = 1
            if(dirx < 0):
                dirx = -1

            step = 1
            if(y0 > y1):
                step = -1

            dy1 = dy + 1.
            for y in range(y0, y1 + step, step):
                div = err/dy1
                self.plotA(x, y, 1. - div)    
                self.plotA(x + dirx, y, div)
                err = err + derr
                if(err >= dy1):
                    x = x + dirx
                    err = err - dy1

        else:
            derr = (dy + 1.)
            y = y0
            
            diry = y1 - y0
            if(diry > 0):
                diry = 1
            if(diry < 0):
                diry = -1

            step = 1
            if(x0 > x1):
                step = -1

            dx1 = dx + 1.
            for x in range(x0, x1 + step, step):
                div = err/dx1
                self.plotA(x, y, 1. - div)
                self.plotA(x, y + diry, div)
                err = err + derr
                if(err >= dx1):
                    y = y + diry
                    err = err - dx1

    def plotA(self, x : int, y : int, alpha : float = 1.):
        combine_rgbas(self.colors[0], alpha, self.data[x][y], 1., self.data[x][y])
        self.canvas.create_line(x, y, x+1, y, fill=('#%02x%02x%02x' %  (self.data[x][y][0], self.data[x][y][1], self.data[x][y][2])))

    def fill(self, x, y, filled_color=None):
        if(np.array_equal(filled_color, None)):
            filled_color = self.data[x][y].copy()

        rdx = 0
        while(x+rdx+1 < constants.CANV_WIDTH and rgb_equal(self.data[x+rdx+1][y], filled_color)):
            rdx += 1
        
        ldx = 0
        while(x-ldx-1 >= 0 and rgb_equal(self.data[x-ldx-1][y], filled_color)):
            ldx += 1
        
        self.canvas.create_line(x - ldx, y, x + rdx + 1, y, fill=self.str_colors[1])
        for i in np.arange(x - ldx, x + rdx + 1):
            self.data[i][y] = self.colors[1]
        # self.line(x - ldx, y, x + rdx + 1, y)
        for i in np.arange(x-ldx, x+rdx+1):
            if(y + 1 < constants.CANV_HEIGHT and rgb_equal(filled_color, self.data[i][y + 1])):
                self.fill(i, y + 1, filled_color)
            if(y - 1 >= 0 and rgb_equal(filled_color, self.data[i][y - 1])):    
                self.fill(i, y - 1, filled_color)


@njit(cache=True, inline='always')
def rgb_equal(a, b):
        if a[0] == b[0] and a[1] == b[1] and a[2] == b[2]:
            return True
        return False


@njit(parallel=True, cache='always')
def combine_rgbas(S, Sa, D, Da, out):
    Ra = Da + Sa*(1. - Da)
    for i in prange(0, 3):
        out[i] = (S[i] * Sa + D[i]*Da*(1. - Sa))


@njit(cache=True, inline='always')
def dist(p1, p2):
    return math.sqrt(pow(p1[0] - p2[0], 2) + pow(p1[1] - p2[1], 2))


if __name__ == '__main__':
    Window()
