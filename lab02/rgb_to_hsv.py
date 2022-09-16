from math import floor
import numpy as np
from PIL import Image

def get_hsv_pixel(pixel : np.array):
    r, g, b = pixel / 255.
    max = np.max([r, g, b])
    min = np.min([r, g, b])
    
    if(max == min):
        H = 0
    elif(max == r and g >= b):
        H = 60*(g-b)/(max-min)
    elif(max == r and g < b):
        H = 60*(g-b)/(max-min) + 360
    elif(max == g):
        H = 60*(b-r)/(max-min) + 120
    elif(max == b):
        H = 60*(r-g)/(max-min) + 240

    if(max == 0):
        S = 0
    else:
        S = 1 - min/max

    return np.array([H, S, max])

def rgb_to_hsv(_arr : np.array):
    return np.apply_along_axis(get_hsv_pixel, 2, _arr)
    
def get_rgb_pixel(pixel : np.array):
    H, S, V = pixel
    Hi = floor(H/60) % 6
    f = H/60-floor(H/60)
    p = V*(1-S)
    q = V*(1-f*S)
    t = V*(1-(1-f)*S)

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

def hsv_to_rgb(_arr):
    return np.apply_along_axis(get_rgb_pixel, 2, _arr)

#img = Image.open("1556708032_1.jpg")
img = Image.open("mono.jpg")

hsv = rgb_to_hsv(np.asarray(img))
rgb = hsv_to_rgb(hsv)

newimage = Image.fromarray((rgb * 255).astype(np.uint8))
newimage.show()


