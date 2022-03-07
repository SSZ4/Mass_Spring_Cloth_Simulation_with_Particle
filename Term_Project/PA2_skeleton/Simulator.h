#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>



#include "cloth.h"
#include "particle.h"

using namespace std;


class Simulator
{
public:
	Simulator(void);
	~Simulator(void);

public:
	void					Initialize(void);
 	void					Update();
	void					Render();
	void					Lighting(void);
	void					DrawGround(void);
	GLuint					LoadTexture(const char* filename);
	//void					CreateProgram();
	//unsigned int			createShader(const char* src, GLenum type);
	//void					initTextureSkybox();
	//void					SkyDataTransfer();

public:
	mass_cloth			*cloth;
	vec3				ground;
	mass_particle		*particle;
	float				timsStep;	

	//GLuint			program2, vertShader2, fragShader2 = 0;
	//unsigned int		VAO3, VBO3, NBO3, TBO3, IBO3, TEXTURE3;
};

