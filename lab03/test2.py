from utils import constants, styles, util_funcs
from utils.pixel_comparable_by_coords import Detour, PixelComparableByCoords as UberPix, ClockwiseTurn90
import heapq
import numpy as np

def magic_wand(x: int, y: int, data):
    data = np.asarray(data).transpose((1, 0))

    canvasmx = data.shape[0]
    canvasmy = data.shape[1]
    print(data)
    pix_list = []
    # get color of data[x][y] -> event_col
    event_col = data[x][y]
    print(event_col)
    while data[x][y] == event_col:
        x += 1
        if x >= data.shape[0]:
            print('no border found!')
            return
    print('found border', x, y)
    pix_list.append(UberPix(x, y))
    first_pix_coords = (x, y)

    outline_col = data[x][y]
    print('outline_col =', outline_col)

    new_dir = 6
    new_dir, x, y = get_new_pix(new_dir, x, y, data, canvasmx, canvasmy, outline_col)

    while x != first_pix_coords[0] or y != first_pix_coords[1]:
        pix_list.append(UberPix(x, y))
        new_dir, x, y = get_new_pix(new_dir, x, y, data, canvasmx, canvasmy, outline_col)

    a = sorted(pix_list, key=lambda x: x)
    print(len(a))
    for elem in a:
        print('(',elem.x,elem.y,')')


def get_new_pix(new_dir, x, y, data, canvasmx, canvasmy, outline_col):
    for i in [j for j in range(new_dir, 8)] + [j for j in range(new_dir)]:  # lol
        direction = Detour[i]
        new_x, new_y = x + direction.dx, y + direction.dy
        if canvasmx > new_x >= 0 and canvasmy > new_y >= 0 and data[new_x][new_y] == outline_col:
            new_dir = ClockwiseTurn90[i]
            x, y = new_x, new_y
            return (new_dir, x, y)


magic_wand(2, 1, [
    [0, 0, 1, 1, 1, 0, 0, 0],
    [0, 1, 0, 0, 1, 1, 0, 0],
    [1, 1, 0, 0, 1, 1, 0, 0],
    [0, 1, 1, 1, 1, 0, 0, 0],
])
