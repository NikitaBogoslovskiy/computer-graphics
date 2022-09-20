# #2) Выделить из полноцветного изображения каждый из каналов R, G, B  и вывести результат.
# # #Построить гистограмму по цветам (3 штуки).
import os
import sys
import threading
from draw_hist import draw_hist
from PIL import Image

pixel_funcs = {
    "r": lambda pixel: pixel[0],
    "g": lambda pixel: pixel[1],
    "b": lambda pixel: pixel[2]
}

conversion_matrix = {
    "r": (1, 0, 0, 0,
          0, 0, 0, 0,
          0, 0, 0, 0),

    "g": (0, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 0, 0),

    "b": (0, 0, 0, 0,
          0, 0, 0, 0,
          0, 0, 1, 0)
}


def async_extracting(img: Image, color: str, dirname: str, ):
    # extracting channel and saving it to file
    converted_img = img.convert("RGB", conversion_matrix[color])
    converted_img.save(dirname + "/" + color + '_extracted.png')

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

    # fname = "./kotyk.jpg"
    fname = sys.argv[1]
    if not os.path.isfile(fname):
        print('No file {} found in current directory.'.format(fname))
        return
    try:
        img = Image.open(fname)

        dirname = "files"
        if not os.path.isdir(dirname):
            os.mkdir(dirname)

        for color in ["r", "g", "b"]:
            thread = threading.Thread(target=async_extracting, args=(img.copy(), color, dirname,))
            thread.start()

        main_thread = threading.main_thread()
        for t in threading.enumerate():
            if t is main_thread:
                continue
            t.join()

        print("All done!")
    except:
        print("An exception occurred")


if __name__ == "__main__":
    main()
