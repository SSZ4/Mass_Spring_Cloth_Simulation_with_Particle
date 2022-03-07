#pragma once
#include "pnode.h"
#include <stdio.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <ctime>

class mass_particle
{
public:
	std::vector<pnode*> pnodes;
	int			number_particle;
	double		kres;


	mass_particle() {

	}
	~mass_particle()
	{
		for (int i = 0; i < pnodes.size(); i++) { delete pnodes[i]; }
		pnodes.clear();
	}



public://여기부턴 함수
	void init() {
		for (int k = 0; k < number_particle / 9; k++) {
			for (int i = 0; i < 9; i++) {
				if (i <= 2) {
					pnode* xp = new pnode(vec3(21.5 + k, 45 + 6 * k, 8.5 + 4 * i - k));
					pnodes.push_back(xp);
				}
				if (i <= 5 && i > 2) {
					pnode* xp = new pnode(vec3(25.5, 45 + 6 * k, 8.5 + 4 * (i - 3)));
					pnodes.push_back(xp);
				}
				if (i <= 8 && i > 5) {
					pnode* xp = new pnode(vec3(29.5 - k, 45 + 6 * k, 8.5 + 4 * (i - 6) + k));
					pnodes.push_back(xp);
				}
			}
		}
	}

	void create_particle() {
		for (int k = 0; k < number_particle / 9; k++) {
			srand(time(NULL));
			double p1 = (double)(rand() % 200 + 1) / 100;
			double p2 = (double)(rand() % 200 + 1) / 100;
			double p3 = (double)(rand() % 200 + 1) / 100;
			for (int i = 0; i < 9; i++) {
				if (i <= 2) {
					pnode* xp = new pnode(vec3(21.5 + k + p1, 45 + 6 * k, 8.5 + 4 * i - k - p1));
					pnodes.push_back(xp);
				}
				if (i <= 5 && i > 2) {
					pnode* xp = new pnode(vec3(25.5 - p2, 45 + 6 * k, 8.5 + 4 * (i - 3) + p2));
					pnodes.push_back(xp);
				}
				if (i <= 8 && i > 5) {
					pnode* xp = new pnode(vec3(29.5 - k + p3, 45 + 6 * k, 8.5 + 4 * (i - 6) + k + p3));
					pnodes.push_back(xp);
				}
			}
		}
	}

	void collision_particle() {

		for (int i = 0; i < pnodes.size(); i++) {
			for (int j = 0; j < pnodes.size(); j++) {				
				if (i == j)
					continue;

				vec3 prel = pnodes[i]->position - pnodes[j]->position;
				vec3 vrel = pnodes[i]->velocity - pnodes[j]->velocity;
				
				if ((pnodes[i]->radius + pnodes[j]->radius + 0.2) - (pnodes[i]->position.dist(pnodes[j]->position)) > 0
					&& prel.dot(vrel) < 0) {//particle끼리 충돌하였는가				

					vec3 n1 = (pnodes[j]->position - pnodes[i]->position);
					n1.Normalize();
					vec3 v1n = (n1.dot(pnodes[i]->velocity)) * n1;
					vec3 v1t = pnodes[i]->velocity - v1n;			

					vec3 n2 = (pnodes[i]->position - pnodes[j]->position);
					n2.Normalize();
					vec3 v2n = (n2.dot(pnodes[j]->velocity)) * n2;
					vec3 v2t = pnodes[j]->velocity - v2n;

					vec3 v11n = (2 * pnodes[j]->mass * v2n + (pnodes[i]->mass - pnodes[j]->mass) * v1n) / (pnodes[i]->mass + pnodes[j]->mass);
					vec3 v22n = (2 * pnodes[i]->mass * v1n + (pnodes[j]->mass - pnodes[i]->mass) * v2n) / (pnodes[i]->mass + pnodes[j]->mass);

					pnodes[i]->velocity = v11n + v1t;
					pnodes[j]->velocity = v22n + v2t;
			
				}
					
			}
		}
	}

	void collision_ground(vec3 ground) {
		vec3 gnormal = vec3(0.0f, 1.0f, 0.0f);
		gnormal.Normalize();

		for (int i = 0; i < pnodes.size(); i++) {
			if ((pnodes[i]->position - ground).dot(gnormal) < 1
				&& gnormal.dot(pnodes[i]->velocity) < 0) {

				vec3 vn = gnormal.dot(pnodes[i]->velocity) * gnormal;
				vec3 vt = pnodes[i]->velocity - vn;
				vec3 result = vt - vn;
				pnodes[i]->velocity = 0.7 * result; // 완전탄성 아님

			}
		}
	}

	void compute_force(vec3 gravity)
	{
		for (int i = 0; i < pnodes.size(); i++)
		{
			pnodes[i]->add_force(gravity * pnodes[i]->mass);
		}
	}

	void integrate(double dt)
	{
		/* integrate Nodes*/
		for (int i = 0; i < pnodes.size(); i++)
		{
			pnodes[i]->integrate(dt);
		}
	}

	void draw();
};