import cv2
from typing import List
import os
from PIL import Image, ImageDraw
import numpy as np

coefficients_type = {
    "PAL": [0.299, 0.587, 0.114],
    "HDTV": [0.2126, 0.7152, 0.0722]
}


def rgb_to_grayscale(image_path: str,
                     output_name: str,
                     coefficients: List[float],
                     create_histogram: bool = False):
    im = cv2.imread(image_path)
    c_array = np.array(coefficients).reshape(-1, 1)
    grayscale = np.tile((im @ c_array).astype(int), (1, 1, 3))
    normalized_grayscale = grayscale / 256
    directory = os.path.split(image_path)[0] + "/"
    cv2.imwrite(directory + output_name, grayscale)
    cv2.imshow("Resulted grayscale", normalized_grayscale)
    cv2.waitKey(0)
    if create_histogram:
        hist_arr = [0] * 256
        for i in range(grayscale.shape[0]):
            hist_arr[grayscale[i, i, 0]] += 1
        draw_hist(directory + "hist_" + output_name, hist_arr)


def find_difference(image_path_1: str,
                    image_path_2: str,
                    output_name: str):
    im1 = cv2.imread(image_path_1)
    im2 = cv2.imread(image_path_2)
    im1_np = np.array(im1, dtype=np.int16)
    im2_np = np.array(im2, dtype=np.int16)
    result = np.abs(im1_np - im2_np)
    normalized_result = result / 255
    cv2.imwrite(os.path.split(image_path_1)[0] + "/" + output_name, result)
    cv2.imshow("Resulted difference", normalized_result)
    cv2.waitKey(0)


def draw_hist(hist_path, hist_arr):
    width = len(hist_arr)
    height = len(hist_arr)
    hist = Image.new("RGB", (width, height), "white")
    draw = ImageDraw.Draw(hist)
    max_column = float(max(hist_arr))
    if max_column == 0:
        draw.rectangle(((0, 0), (width, height)), fill="black")
    else:
        for i in range(width):
            draw.line(((i, height), (i, height-hist_arr[i]/max_column * height)), fill="black")
    del draw
    hist.save(hist_path)


if __name__ == "__main__":
    rgb_to_grayscale(image_path="files/input.jpg",
                     coefficients=coefficients_type["PAL"],
                     output_name="pal_grayscale.jpg",
                     create_histogram=True)
    rgb_to_grayscale(image_path="files/input.jpg",
                     coefficients=coefficients_type["HDTV"],
                     output_name="hdtv_grayscale.jpg",
                     create_histogram=True)
    find_difference(image_path_1="files/pal_grayscale.jpg",
                    image_path_2="files/hdtv_grayscale.jpg",
                    output_name="grayscale_difference.jpg")
