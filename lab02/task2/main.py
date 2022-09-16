# #2) Выделить из полноцветного изображения каждый из каналов R, G, B  и вывести результат.
# # #Построить гистограмму по цветам (3 штуки).
import os
from typing import List
from PIL import Image
from color_extract import r, g, b
from draw_hist import draw_hist


def extract_channels(img: Image) -> List:
    """
    extracts red, green and blue channels from img
    :param img: img to extract red, green and blue channels from
    :return: list of Images representing extracted channels
    """
    # Creating a image object, of the sample image
    # A 12-value tuple which is a transform matrix for dropping
    matrix_r = (1, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0)

    matrix_g = (0, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 0, 0)

    matrix_b = (0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 1, 0)
    return [img.convert("RGB", mat) for mat in [matrix_r, matrix_g, matrix_b]]


# histogram: 0x - [0..255] - intensity of Color (r or g or b)
# columns: amount of pixels of that intensity
def draw_channels_hist(imgs: List, dirname: str):
    """
    saves Images representing extracted channels and histogram of intensity for each
    :param imgs: List of images
    :return: void
    """
    # width and height of diagram
    W, H = 256, 100
    fnlist = [(r, "r_channel_"), (g, "g_channel_"), (b, "b_channel_")]

    for ind, img in enumerate(imgs):
        img.save(dirname + "/" + fnlist[ind][1] + 'extracted.png')
        clrs = img.getcolors(img.size[0] * img.size[1])

        hist_columns = [0 for i in range(W)]
        for pixel_count, pixel in clrs:
            index = fnlist[ind][0](pixel)
            hist_columns[index] += pixel_count
        draw_hist(dirname + "/" +  fnlist[ind][1] + "hist.png", H, hist_columns)


def main():
    print("In progress...")

    fname = "../limonokrysy.jpeg"
    img = Image.open(fname)

    dirname = "files"
    if not os.path.isdir(dirname):
        os.mkdir(dirname)

    draw_channels_hist(extract_channels(img), dirname)
    print("All done!")


if __name__ == "__main__":
    main()


