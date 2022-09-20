from PIL import Image, ImageDraw


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
            # draw.line(((i, H), (i, H-harr[i]/maxx*H)), fill="black") #рисуем столбики
            draw.rectangle(((i * Col_width, H), ((i + 1) * Col_width, H * (1 - harr[i] / maxx))), fill="black")
    del draw
    hist.save(hname)
