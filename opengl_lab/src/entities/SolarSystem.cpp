#include "../headers/entities/SolarSystem.h"
#include <ctime>

SolarSystem::SolarSystem(size_t _planets_number) {
	planets_number = _planets_number;
}

void SolarSystem::LoadModels(const char* sun_object_path, const char* sun_texture_path, const char* planet_object_path, const char* planet_texture_path)
{
	sun = new DynamicMesh();
	sun->Load(sun_object_path);
	sun->ChangeShaders("shaders/solar_system/sun.vert", "shaders/solar_system/sun.frag");
	sun->InitTextures((char*)sun_texture_path);
	sun->InitVO();

	planet = new DynamicMesh();
    planet->Load(planet_object_path);
    planet->ChangeShaders("shaders/solar_system/planet.vert", "shaders/solar_system/planet.frag");
    planet->InitTextures((char*)planet_texture_path);
    planet->InitVO();
}

void SolarSystem::PrepareData(const glm::vec3& center)
{
    auto sp = new SunParameters();
    sp->position = center;
    sp->scale_factor = 0.25;
    sp->selfrotation_speed = 1.0;
    sun_parameters = sp;

    model_matrices = new glm::mat4[planets_number];
    srand(time(NULL));
    for (unsigned int i = 0; i < planets_number; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        auto pp = new PlanetParameters();
        pp->movement_speed = 1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0));
        pp->scale_factor = 0.002 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 0.013));
        pp->selfrotation_speed = 3.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 15.0));
        float axis_x = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0));
        float axis_y = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0));
        float axis_z = -1.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 2.0));
        pp->rotation_axis = glm::normalize(glm::vec3(axis_x, axis_y, axis_z));
        model_matrices[i] = model;
        planets_parameters.push_back(pp);
    }
}

void SolarSystem::Draw(float time_coefficient, const glm::mat4& view, const glm::mat4& projection, float music_seconds)
{
    auto sun_model = glm::mat4(1.0f);
    sun_model = glm::translate(sun_model, sun_parameters->position);
    sun_model = glm::scale(sun_model, glm::vec3(sun_parameters->scale_factor));
    float self_rot_angle = glm::radians(SELF_ROTATION_ANGLE_UNIT * time_coefficient * sun_parameters->selfrotation_speed);
    sun_model = glm::rotate(sun_model, self_rot_angle, glm::vec3(0.0f, 1.0f, 0.0f));
    sun->Draw(sun_model, view, projection);
    
    auto center = sun_parameters->position;
    float radius = 2.5;
    float offset = 2.0;
    for (unsigned int i = 0; i < planets_number; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        if (music_seconds < 15 || (music_seconds >= 35 && music_seconds < 37.8))
        {
            float general_angle = GENERAL_ROTATION_ANGLE_UNIT * time_coefficient * planets_parameters[i]->movement_speed;
            float x = center.x + sin(general_angle) * (offset + radius * (i + 1));
            float y = center.y / 2.0;
            float z = center.z + cos(general_angle) * (offset + radius * (i + 1));
            model = glm::translate(model, glm::vec3(x, y, z));
            self_rot_angle = glm::radians(SELF_ROTATION_ANGLE_UNIT * time_coefficient * planets_parameters[i]->selfrotation_speed);
            model = glm::rotate(model, self_rot_angle, glm::vec3(0, 1, 0));
            model = glm::scale(model, glm::vec3(planets_parameters[i]->scale_factor));
        }
        else
        {
            float general_angle = GENERAL_ROTATION_ANGLE_UNIT * time_coefficient * planets_parameters[i]->movement_speed * 3;
            float x = center.x + sin(general_angle) * (offset + radius * (i + 1));
            float y = center.y / 2.0;
            y += -2.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 3.0));
            float z = center.z + cos(general_angle) * (offset + radius * (i + 1));
            model = glm::translate(model, glm::vec3(x, y, z));
            self_rot_angle = glm::radians(SELF_ROTATION_ANGLE_UNIT * time_coefficient * planets_parameters[i]->selfrotation_speed * 2);
            model = glm::rotate(model, self_rot_angle, planets_parameters[i]->rotation_axis);
            float extra_factor = 0.8 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 1.2));
            model = glm::scale(model, glm::vec3(planets_parameters[i]->scale_factor * extra_factor));
        }
        model_matrices[i] = model;
    }

    glUseProgram(planet->Program);
    glBindVertexArray(planet->VAO);

    unsigned int viewLoc = glGetUniformLocation(planet->Program, "view");
    unsigned int projectionLoc = glGetUniformLocation(planet->Program, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glGenBuffers(1, &instanced_array);
    glBindBuffer(GL_ARRAY_BUFFER, instanced_array);
    glBufferData(GL_ARRAY_BUFFER, planets_number * sizeof(glm::mat4), &model_matrices[0], GL_STATIC_DRAW);

    auto Attrib_IMatrix = glGetAttribLocation(planet->Program, "instanceMatrix");
    glEnableVertexAttribArray(Attrib_IMatrix);
    glVertexAttribPointer(Attrib_IMatrix, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
    glEnableVertexAttribArray(Attrib_IMatrix + 1);
    glVertexAttribPointer(Attrib_IMatrix + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(Attrib_IMatrix + 2);
    glVertexAttribPointer(Attrib_IMatrix + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(Attrib_IMatrix + 3);
    glVertexAttribPointer(Attrib_IMatrix + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(Attrib_IMatrix, 1);
    glVertexAttribDivisor(Attrib_IMatrix + 1, 1);
    glVertexAttribDivisor(Attrib_IMatrix + 2, 1);
    glVertexAttribDivisor(Attrib_IMatrix + 3, 1);

    uint i = 0;
    for (auto t : planet->textures) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, t);
    }
    glDrawElementsInstanced(GL_TRIANGLES, planet->indices.size(), GL_UNSIGNED_INT, 0, planets_number);

    glBindVertexArray(0);
    glUseProgram(0);
}
