# #2) Выделить из полноцветного изображения каждый из каналов R, G, B  и вывести результат.
# # #Построить гистограмму по цветам (3 штуки).

from PIL import Image, ImageDraw

# Creating a image object, of the sample image
fname = 'limonokrysy.jpeg'
img = Image.open(fname)

# A 12-value tuple which is a transform matrix for dropping
# green channel (in this case)
matrix_r = (1, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 0)

matrix_g = (0, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0)

matrix_b = (0, 0, 0, 0,
            0, 0, 0, 0,
            0, 0, 1, 0)

imgs = []
for mat in [matrix_r, matrix_g, matrix_b]:
    imgs.append(img.convert("RGB", mat))


# gistogramm: 0x - [0..255] - intensity of Color (r or g or b)
# columns: amount of pixels of that intensity


def r(c):
    return c[0]


def g(c):
    return c[1]


def b(c):
    return c[2]


def drawhist(hname, H, harr):
    W = len(harr)
    hist = Image.new("RGB", (W, H), "white")
    draw = ImageDraw.Draw(hist)
    maxx = float(max(harr))
    if maxx == 0:
        draw.rectangle(((0, 0), (W, H)), fill="black")
    else:
        for i in range(W):
            draw.line(((i, H), (i, H - harr[i] / maxx * H)), fill="black")
    del draw
    hist.save(hname)


W, H = img.size[0], img.size[1]
fnlist = [(r, "r_channel_"), (g, "g_channel_"), (b, "b_channel_")]
for ind, img in enumerate(imgs):

    img.save(fnlist[ind][1]+'color.png')
    print(img.size)
    clrs = img.getcolors(W * H)
    harr = [0 for i in range(W)]
    for n, c in clrs:
        index = fnlist[ind][0](c)
        harr[index] += n
    drawhist(fnlist[ind][1] + "hist.png", H, harr)
