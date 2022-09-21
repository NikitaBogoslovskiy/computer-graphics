import tkinter as tk
from math import floor
from tkinter import filedialog as fd

from numba import njit, prange
import numpy as np
from PIL import Image
from PIL import ImageTk as itk


class Window(tk.Tk):
    image_filename: str
    data: np.ndarray
    image: 'tk._CanvasItemId'
    hsv_data: np.ndarray
    H: int = 0
    S: int = 0
    V: int = 0

    def __init__(self):
        super().__init__()
        self.resizable(False, False)
        self.create_widgets()
        self.mainloop()

    def create_widgets(self):
        self.f_scales = tk.Frame(self)
        self.f_buttons = tk.Frame(self.f_scales)
        self.b_open_file = tk.Button(self.f_buttons, text='Open an image', command=self.open_file)
        self.b_save = tk.Button(self.f_buttons, text='Save', command=self.save_file)
        self.canvas = tk.Canvas(self, width=800, height=450, bg='white')
        self.sc_h = tk.Scale(self.f_scales, from_=0, to=359, orient=tk.HORIZONTAL, command=self.change_hue, label='Hue')
        self.sc_s = tk.Scale(self.f_scales, from_=0, to=99, orient=tk.HORIZONTAL, command=self.change_saturation, label='Saturation')
        self.sc_v = tk.Scale(self.f_scales, from_=0, to=99, orient=tk.HORIZONTAL, command=self.change_value, label='Value')
        
        self.canvas.create_text(75, 20, text='No image', anchor=tk.NW)
        self.canvas.pack(side=tk.LEFT)
        
        self.f_scales.pack(side=tk.RIGHT)
        self.sc_h.pack(fill=tk.X)
        self.sc_s.pack(fill=tk.X)
        self.sc_v.pack(fill=tk.X)

        self.f_buttons.pack()
        self.b_open_file.pack(padx=5, pady=5)
        self.b_save.pack(padx=5, pady=5)

        # self.sc_s.set(0)
        # self.sc_v.set(0)

    def open_file(self):
        self.image_filename = fd.askopenfilename()
        if not self.image_filename:
            return

        img = Image.open(self.image_filename)
        self.data = np.asarray(img).copy()
        self.canvas.config(width=img.width, height=img.height)
        self.title(self.image_filename)
        self.hsv_data = rgb_to_hsv(self.data, np.empty_like(self.data).astype(np.float64))
        self.update_image()

    def save_file(self):
        Image.fromarray(self.data).save(fd.asksaveasfilename())

    def update_image(self):
        self.image = itk.PhotoImage(Image.fromarray(self.data))
        self.canvas.create_image(0, 0, anchor=tk.NW, image=self.image)

    def change_hue(self, hue):
        self.H = int(hue)
        self.data = hsv_to_rgb(self.hsv_data, self.data, self.H, self.S, self.V)
        self.update_image()

    def change_saturation(self, saturation):
        self.S = int(saturation)
        self.data = hsv_to_rgb(self.hsv_data, self.data, self.H, self.S, self.V)
        self.update_image()

    def change_value(self, value):
        self.V = int(value)
        self.data = hsv_to_rgb(self.hsv_data, self.data, self.H, self.S, self.V)
        self.update_image()

@njit(cache=True)
def get_hsv_pixel(pixel : np.array) -> np.array:
    r, g, b = pixel / 255.

    mx = max(r, g, b)
    mn = min(r, g, b)
    
    H, S, V = [0., 0., 0.]

    if(mx == mn):
        H = 0.
    elif(mx == r and g >= b):
        H = 60.*(g-b)/(mx-mn)
    elif(mx == r and g < b):
        H = 60.*(g-b)/(mx-mn) + 360.
    elif(mx == g):
        H = 60.*(b-r)/(mx-mn) + 120.
    elif(mx == b):
        H = 60.*(r-g)/(mx-mn) + 240.

    if(mx == 0):
        S = 0.
    else:
        S = 1. - mn/mx

    V = mx

    return np.array([H, S, V])

@njit(parallel=True, cache=True)
def rgb_to_hsv(_in : np.array, _out : np.array) -> np.array:
    for j in prange(0, _in.shape[0]):
        for i in prange(0, _in.shape[1]):
            _out[j][i] = get_hsv_pixel(_in[j][i])
    return _out

@njit(cache=True)    
def get_rgb_pixel(pixel : np.array) -> np.array:
    H, S, V = pixel
    Hi = floor(H/60) % 6
    f = H/60-floor(H/60)
    p = V*(1.-S)
    q = V*(1.-f*S)
    t = V*(1.-(1.-f)*S)

    match Hi:
        case 0:
            return np.array([V, t, p])
        case 1:
            return np.array([q, V, p])
        case 2:
            return np.array([p, V, t])
        case 3:
            return np.array([p, q, V])
        case 4:
            return np.array([t, p, V])
        case 5:
            return np.array([V, p, q])

    return np.empty((3))

@njit(cache=True, inline='always')  
def save_in_bound(el, minv, maxv) -> float:
    if(el > maxv):
        return maxv
    if(el < minv):
        return minv
    return el

@njit(parallel=True, cache=True)
def hsv_to_rgb(_in : np.array, _out : np.array, h, s, v) -> np.array:
    for j in prange(0, _in.shape[0]):
        for i in prange(0, _in.shape[1]):
            
            H = (_in[j][i][0] + h) % 360.
            S = (_in[j][i][1] + s/100.) % 1.
            V = (_in[j][i][2] + v/100.) % 1.
            
            # H = save_in_bound(_in[j][i][0] + h, 0., 360.)
            # S = save_in_bound(_in[j][i][1] + s/100., 0., 1.)
            # V = save_in_bound(_in[j][i][2] + v/100., 0., 1.)
            
            _out[j][i] = get_rgb_pixel(np.array([H, S, V])) * 255
    return _out


if(__name__ == '__main__'):
    Window()