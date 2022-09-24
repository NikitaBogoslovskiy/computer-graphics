# #2) Выделить из полноцветного изображения каждый из каналов R, G, B  и вывести результат.
# # #Построить гистограмму по цветам (3 штуки).
import os
import sys
import numpy as np
import threading
from PIL import Image, ImageDraw

task_dir = "files/rgb_channels"
task_inputs_dir = task_dir + "/inputs"

pixel_funcs = {
    "r": lambda pixel: pixel[0],
    "g": lambda pixel: pixel[1],
    "b": lambda pixel: pixel[2]
}

conversion_matrix = {
    "r": [[1, 0, 0],
          [0, 0, 0],
          [0, 0, 0]],

    "g": [[0, 0, 0],
          [0, 1, 0],
          [0, 0, 0]],

    "b": [[0, 0, 0],
          [0, 0, 0],
          [0, 0, 1]],
}

def draw_hist(hname, harr, H, Col_width = 1):
    """ saves color diagram into file
        hname - name of hist file
        H - hist height
        harr - array of color intensity frequency
        Col_width - width of each column
        (width of hist = Col_width * len(harr))
    """
    W = len(harr)
    hist = Image.new("RGB", (W * Col_width, H), "white")
    draw = ImageDraw.Draw(hist)  # "pen"
    maxx = float(max(harr))  # max value of frequency
    if maxx == 0:
        draw.rectangle(((0, 0), (W * Col_width, H)), fill="black")
    else:
        for i in range(W):
            draw.rectangle(((i * Col_width, H), ((i + 1) * Col_width, H * (1 - harr[i] / maxx))), fill="black")
    del draw
    hist.save(hname)

def async_extracting(img: Image, color: str, dirname: str, ):
    # extracting channel and saving it to file
    converted_img = np.asarray(img)
    result = converted_img @ np.array(conversion_matrix[color])
    img = Image.fromarray(result.astype(np.uint8))
    img.save(dirname + "/" + color + '_extracted.png')

    # drawing histogram and saving it to file
    W, H = 256, img.size[1]  # width and height of histogram respectively
    clrs = img.getcolors(img.size[0] * img.size[1])
    hist_columns = [0 for i in range(W)]

    # accumulating data for hist:
    # column index: [0..255] - intensity of Color (r/g/b)
    # column height: amount of pixels of that intensity
    for pixel_count, pixel in clrs:
        pixel_index = pixel_funcs[color](pixel)
        hist_columns[pixel_index] += pixel_count
    Col_width = img.size[0] // W
    draw_hist(dirname + "/" + color + "_hist.png", hist_columns, H, Col_width)


def main():
    print("In progress...")

    if len(sys.argv) < 2:
        print('No file to process specified. Exit...')
        return

    fname = task_inputs_dir + "/" + sys.argv[1]
    if not os.path.isfile(fname):
        print('No file {} found in current directory. Exit...'.format(fname))
        return

    try:
        img = Image.open(fname)

        for color in ["r", "g", "b"]:
            thread = threading.Thread(target=async_extracting, args=(img.copy(), color, task_dir,))
            thread.start()

        main_thread = threading.main_thread()
        for t in threading.enumerate():
            if t is main_thread:
                continue
            t.join()

        print("All done!")
    except:
        print("An exception occurred")


main()