import numpy as np


def rgb_tuple_to_str(pixel: np.ndarray[np.uint8]):
    return '#%02x%02x%02x' % (pixel[0], pixel[1], pixel[2])
