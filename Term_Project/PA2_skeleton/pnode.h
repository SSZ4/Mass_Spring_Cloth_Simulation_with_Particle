#pragma once
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include "vector.h"
#include <GL/glut.h>

class pnode
{
public:
	double mass;
	double radius;
	vec3	force;
	vec3	position;
	vec3	velocity;
	vec3	acceleration;
	vec3	normal;
	GLUquadric* kp;


public:
	pnode(vec3 init_pos)
	{
		position = init_pos;
		mass = 1.0;
		radius = 0.5;
		kp = gluNewQuadric();
	}

	double	getPosX(void) { return position.getX(); }
	double	getPosY(void) { return position.getY(); }
	double	getPosZ(void) { return position.getZ(); }

	void add_force(vec3 additional_force)
	{
		force += additional_force;
	}

	void integrate(double dt)
	{
		//Basic Implements 2-2. Integration
		acceleration = force / mass;
		velocity = acceleration * dt + velocity;
		position = velocity * dt + position;

		/*initialize Force*/
		force.x = force.y = force.z = 0.0;
	}

	void draw();
};