#include "Node.h"
#include "pnode.h"


#include <GL/glut.h>
#include "cloth.h"
#include "particle.h"
#include <cstdlib>
#include <ctime>

void Node::draw()
{
	glDisable(GL_LIGHTING);
	glPointSize(2.0);

	glBegin(GL_POINTS);	
	glVertex3f(getPosX(), getPosY(), getPosZ());
	glEnd();
	glEnable(GL_LIGHTING);
}

void pnode::draw()
{
	srand(time(NULL));
	double r = (double)(rand() % 100 + 1) / 100;
	double g = (double)(rand() % 100 + 1) / 100;
	double b = (double)(rand() % 100 + 1) / 100;

	glDisable(GL_LIGHTING);
	glColor3f(r, g, b);

	glPushMatrix();
	glTranslatef(getPosX(), getPosY(), getPosZ());
	gluSphere(kp, radius, 50, 10);
	glPopMatrix();


	glEnable(GL_LIGHTING);
}

void mass_spring::draw()
{
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(2.0);

 	glBegin(GL_LINES);
	glVertex3f(p1->position.x, p1->position.y, p1->position.z);
	glVertex3f(p2->position.x, p2->position.y, p2->position.z);
  	glEnd();	 
	glEnable(GL_LIGHTING);
}

void mass_cloth::draw()
{	
	srand(time(NULL));
	double r = (double)(rand() % 100 + 1) / 100;
	double g = (double)(rand() % 100 + 1) / 100;
	double b = (double)(rand() % 100 + 1) / 100;
	switch (drawMode)
	{
	case DRAW_MASS_NODES:		
		glDisable(GL_LIGHTING);		
		glColor3f(r,g,b);
		for (int i = 1; i < nodes.size(); i++)
			nodes[i]->draw();
		//glColor3f(0.15, 0.95, 0.97);
		//for (int i = 0; i < nodes2.size(); i++)
		//	nodes2[i]->draw();
		glEnable(GL_LIGHTING);
		break;
	case DRAW_SPRINGS:
		glDisable(GL_LIGHTING);
		for (int i = 0; i < spring.size(); i++)
			spring[i]->draw();
		glEnable(GL_LIGHTING);
		break;
	case DRAW_FACES:
		//glColor3f(0.5f, 0.5f, 0.5f);
		//glEnable(GL_LIGHTING);
		//
		//glEnable(GL_TEXTURE_2D);
		//for (int i = 0; i < faces.size(); i = i + 3) {
		//	glBegin(GL_TRIANGLES);
		//	glTexCoord2f(faces[i]->inipos.x, faces[i]->inipos.z);
		//	glNormal3f(faces[i]->normal.x, faces[i]->normal.y, faces[i]->normal.z);
		//	glVertex3f(faces[i]->position.x, faces[i]->position.y, faces[i]->position.z);
		//
		//	glTexCoord2f(faces[i + 1]->inipos.x, faces[i + 1]->inipos.z);
		//	glNormal3f(faces[i + 1]->normal.x, faces[i + 1]->normal.y, faces[i + 1]->normal.z);
		//	glVertex3f(faces[i + 1]->position.x, faces[i + 1]->position.y, faces[i + 1]->position.z);
		//
		//	glTexCoord2f(faces[i + 2]->inipos.x, faces[i + 2]->inipos.z);
		//	glNormal3f(faces[i + 2]->normal.x, faces[i + 2]->normal.y, faces[i + 2]->normal.z);
		//	glVertex3f(faces[i + 2]->position.x, faces[i + 2]->position.y, faces[i + 2]->position.z);
		//	glEnd();
		//}
		//glDisable(GL_TEXTURE_2D);

		//Additional Implements 4-3. Texture Coordinate Mapping
		break;
	default:
		break;
	}
	glPopMatrix();
}

void mass_particle::draw() {
	glDisable(GL_LIGHTING);
	for (int i = 0; i < pnodes.size(); i++)
		pnodes[i]->draw();
	glEnable(GL_LIGHTING);
}
