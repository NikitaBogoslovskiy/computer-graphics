import cv2
from typing import List
import os
from PIL import Image, ImageDraw
import numpy as np
import argparse

coefficients_dict = {
    "PAL": [0.299, 0.587, 0.114],
    "HDTV": [0.2126, 0.7152, 0.0722]
}

input_path = "files/rgb_to_grayscale/inputs"
output_path = "files/rgb_to_grayscale/outputs"
image_ext = ".jpg"
image_width_limit = 1500.


def gen_input_path(filename: str):
    result = input_path + "/" + filename + image_ext
    return result


def gen_output_path(filename: str):
    result = output_path + "/" + filename + image_ext
    return result


def compute_new_size(width, height):
    c = image_width_limit / width
    new_width = int(width * c)
    new_height = int(height * c)
    return new_width, new_height


def rgb_to_grayscale(image_name: str,
                     grayscale_type: str,
                     create_histogram: bool,
                     need_show: bool):
    coefficients = coefficients_dict[grayscale_type]
    im = cv2.imread(gen_input_path(image_name))
    c_array = np.array(coefficients).reshape(-1, 1)
    grayscale = np.tile((im @ c_array).astype(int), (1, 1, 3))
    result_image = np.hstack((im, grayscale))
    if create_histogram:
        hist_array = get_hist(grayscale)
        result_image = np.hstack((result_image, hist_array))
    normalized_result_image = result_image / 255
    cv2.imwrite(gen_output_path(image_name + "_" + grayscale_type), result_image)
    if need_show:
        if normalized_result_image.shape[1] > image_width_limit:
            height, width = normalized_result_image.shape[0], normalized_result_image.shape[1]
            normalized_result_image = cv2.resize(normalized_result_image, compute_new_size(width, height))
        cv2.imshow(f"{image_name}: {grayscale_type} grayscale", normalized_result_image)
        cv2.waitKey(0)
    return np.array(grayscale, dtype=np.int16)


def get_hist(input_image: np.array):
    image_height, image_width = input_image.shape[0], input_image.shape[1]
    unique, counts = np.unique(input_image[:, :, 0], return_counts=True)
    hist_dict = dict(zip(unique, counts))
    column_width = image_width // 256 + 1
    hist_width = 256 * column_width
    hist = Image.new("RGB", (hist_width, image_height), "white")
    column_height_c = float(image_height) / max(counts)
    draw = ImageDraw.Draw(hist)
    hist_index = 0
    for i in range(0, hist_width, column_width):
        if hist_index in hist_dict:
            value = hist_dict[hist_index]
        else:
            value = 0
        draw.rectangle(((i, image_height),
                        (i+column_width, image_height-int(value*column_height_c))), fill="black")
        hist_index += 1
    del draw
    return np.array(hist)


def analyze_images(image_name: str,
                   create_histogram: bool,
                   need_show: bool):
    type1, type2 = coefficients_dict.keys()
    grayscale_type1 = rgb_to_grayscale(image_name, type1, create_histogram, need_show)
    grayscale_type2 = rgb_to_grayscale(image_name, type2, create_histogram, need_show)
    result = np.abs(grayscale_type1 - grayscale_type2)
    result_image = np.hstack((grayscale_type1, grayscale_type2, result))
    normalized_result_image = result_image / 255
    cv2.imwrite(gen_output_path(f"{image_name}_{type1} and {type2} comparison"), result_image)
    if need_show:
        if normalized_result_image.shape[1] > image_width_limit:
            height, width = normalized_result_image.shape[0], normalized_result_image.shape[1]
            normalized_result_image = cv2.resize(normalized_result_image, compute_new_size(width, height))
        cv2.imshow(f"{image_name}: {type1} and {type2} comparison", normalized_result_image)
        cv2.waitKey(0)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--image-name', dest='image_name', required=True,
                        help='Name of the input image')
    parser.add_argument('--show', dest='show', required=False, action='store_true',
                        help='Enables show mode')
    parser.add_argument('--no-show', dest='show', required=False, action='store_false',
                        help='Disables show mode')
    parser.add_argument('--hist', dest='hist', required=False, action='store_true',
                        help='Enables histogram creation')
    parser.add_argument('--no-hist', dest='hist', required=False, action='store_false',
                        help='Disables histogram creation')
    parser.set_defaults(show=True)
    parser.set_defaults(hist=True)
    args = parser.parse_args()
    analyze_images(image_name=args.image_name,
                   create_histogram=args.hist,
                   need_show=args.show)
