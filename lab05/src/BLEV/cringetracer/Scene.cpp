#include "../headers/cringetracer/Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{

}

void Scene::SubRender(Image& img, const size_t start, const size_t end, const size_t xSize, const size_t ySize) {
	for (size_t x = start; x < end; x++)
	{
		for (size_t y = 0; y < ySize; y++)
		{
			double c1 = (static_cast<double>(x) / static_cast<double>(xSize) * 255.0);
			double c2 = (static_cast<double>(y) / static_cast<double>(ySize) * 255.0);
			img.SetPixel(x, y, c1, 0.0, c2);
		}
	}
}
void Scene::Render(Image& img) {
	size_t xSize = img.XSize();
	size_t ySize = img.YSize();

	/*
	The arguments to the thread function are moved or copied by value. 
	If a reference argument needs to be passed to the thread function, it has to be wrapped (e.g., with std::ref or std::cref).
	*/
	std::thread th1(&Scene::SubRender, this, std::ref(img), 0, xSize / 4, xSize, ySize);
	std::thread th2(&Scene::SubRender, this, std::ref(img), xSize / 4, xSize / 2, xSize, ySize);
	std::thread th3(&Scene::SubRender, this, std::ref(img), xSize / 2, xSize / 4 * 3, xSize, ySize);
	std::thread th4(&Scene::SubRender, this, std::ref(img), xSize / 4 * 3, xSize, xSize, ySize);

	th1.join();
	th2.join();
	th3.join();
	th4.join();
}