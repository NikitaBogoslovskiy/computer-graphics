#pragma once

#include "../../headers/pch.h"
#include <iostream>
#include "entity.h"
#include "../../headers/shader_loader.h"

#define left_bottom_near -0.5f,-0.5f,-0.5f
#define right_bottom_near 0.5f,-0.5f,-0.5f
#define right_top_near 0.5f,0.5f,-0.5f
#define left_top_near -0.5f,0.5f,-0.5f
#define left_bottom_far -0.5f,-0.5f,0.5f
#define right_bottom_far 0.5f,-0.5f,0.5f
#define right_top_far 0.5f,0.5f,0.5f
#define left_top_far -0.5f,0.5f,0.5f

/*
OpenGL expects the 0.0 coordinate on the y-axis to be 
on the bottom side of the image, 
but images usually have 0.0 at the top of the y-axis.
so ill have to beg god for forgiveness once again
*/

#define lb 1.0f,0.0f
#define rb 0.0f,0.0f
#define rt 0.0f,1.0f
#define lt 1.0f,1.0f

class MixedCube : public Entity {

protected:

	GLuint Attrib_texture;

public:
	MixedCube() : Entity() {
		//velocity = 0.001f;
	}
	// god forgive me for this
	void AltMixRatio(const float& alt) {
		mixRatio += alt;
		mixRatio = std::max(0.f, mixRatio);
		mixRatio = std::min(1.f, mixRatio);
	}
	float mixRatio = 0.5f;
	float mixRatioStep = 0.0001f;

};
