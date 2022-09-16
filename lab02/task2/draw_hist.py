from PIL import Image, ImageDraw

def draw_hist(hname, H, harr):
	""" Рисуем диаграмму, сохраняем в файл в текущую папку
	hname - имя файла
	H - высота рисунка
	harr - массив с высотами столбиков в гистограмме
	"""
	W = len(harr) #кол-во элементов массива
	hist = Image.new("RGB", (W, H), "white") #создаем рисунок в памяти
	draw = ImageDraw.Draw(hist) #объект для рисования на рисунке
	maxx = float(max(harr)) #высота самого высокого столбика
	if maxx == 0: #столбики равны 0
		draw.rectangle(((0, 0), (W, H)), fill="black")
	else:
		for i in range(W):
			draw.line(((i, H),(i, H-harr[i]/maxx*H)), fill="black") #рисуем столбики
	del draw #удаляем объект
	hist.save(hname) #сохраняем рисунок в файл