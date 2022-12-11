#pragma once

#include <iostream>
#include "entity.h"
#include "../../headers/shader_loader.h"

// not just "Ellipse" bcuz conflict with some class from one of our deps lol
class ColoredEllipse : public Entity {
	static constexpr float PI = 3.14159265359f;
	static constexpr float DPI = PI * 2;

	static constexpr float a60 = PI / 3;
	static constexpr float a120 = a60 * 2;
	static constexpr float a180 = PI;
	static constexpr float a240 = PI + a60;
	static constexpr float a300 = PI + a120;
	static constexpr float a360 = DPI;

	static float __r(float phi) {
		if (phi <= a60 || phi >= a300) return 1.f;
		if (phi >= a120 && phi <= a240) return 0.f;
		if (phi < a120) return 1.f - (phi - a60) / a60;
		if (phi > a240) return (phi - a240) / a60;
	}

	static float __g(float phi) {
		if (phi >= a60 && phi <= a180) return 1.f;
		if (phi >= a240) return 0.f;
		if (phi < a60) return phi / a60;
		if (phi > a180) return 1.f - (phi - a180) / a60;
	}

	static float __b(float phi) {
		return __g(DPI - phi);
	}

	GLuint Attrib_color;

	void InitVBO();

	void InitShader() override;
	void InitVO() override;
	void ReleaseVO() override;

public:
	ColoredEllipse();

	GLfloat scale[2] = { 1.f, 1.f };

	void Draw(const glm::mat4& model, Camera& cam) override;
};