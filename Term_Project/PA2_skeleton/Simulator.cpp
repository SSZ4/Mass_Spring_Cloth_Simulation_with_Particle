#include"Simulator.h"
#define STB_IMAGE_IMPLEMENTATION
#include <GL/stb_image.h>

using namespace std;

Simulator::Simulator()
{
	
}
Simulator::~Simulator()
{

}


void Simulator::Initialize()
{
	timsStep = 0.01;
	ground =vec3(0.0,-15,0.0);
	GLuint Texture;
	Texture = LoadTexture("christmas.bmp");


	cloth = new mass_cloth();
	cloth->dx = 1;
	cloth->dy = 1;
	cloth->dz = 1;
	cloth->size_x = 50;
	cloth->size_y = 50;
	cloth->size_z = 1;
	cloth->structural_coef = 1500;
	cloth->shear_coef = 50;
	cloth->bending_coef = 1000;
	cloth->iteration_n = 20; 
	cloth->drawMode = 0;
	cloth->kres = 0.5;
	cloth->init(); 

	particle = new mass_particle();
	particle->number_particle = 45;
	particle->kres = 0.5;
	particle->init();


	//GLenum errorCode = glewInit();
	//if (GLEW_OK != errorCode) {
	//	cout << "Error: GLEW Initialization is failed\n" << glewGetErrorString(errorCode) << endl;
	//	exit(EXIT_FAILURE);
	//}
	//CreateProgram();
	//initTextureSkybox();
	//SkyDataTransfer();
}

void Simulator::Update()
{
	vec3 gravity(0.0, -9.8 / cloth->iteration_n, 0.0);
	for (int iter = 0; iter < cloth->iteration_n; iter++)
	{
		cloth->compute_force(timsStep, gravity);
		cloth->integrate(timsStep);
		cloth->collision_response(ground);
		cloth->collision_particle(particle);

		particle->compute_force(gravity);
		particle->integrate(timsStep);		
		particle->collision_ground(ground);
		particle->collision_particle();
	}	

	cloth->computeNormal();
}
 
 
void Simulator::Render()
{
	//glDepthFunc(GL_ALWAYS);

	Lighting();
  	DrawGround();
	particle->draw();
 	cloth->draw();	

	//glDepthFunc(GL_LEQUAL);
	//glUseProgram(program2);
	//glBindVertexArray(VAO3);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, TEXTURE3);
	//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
	//glDepthFunc(GL_LESS);
}

void Simulator::Lighting()
{
	glShadeModel(GL_SMOOTH);			 

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	float light_pos[] = {150.0, 150.0, 0.0, 1.0 };
	float light_dir[] = { -1, -1, 0 , 0.0};
	float light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	float frontColor[] = { 0.8, 0.8, 0.8, 1 };

	float matShininess = 20;
	float noMat[] = { 0, 0, 0, 1 };

	float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_AMBIENT, frontColor);					// Set ambient property frontColor.
	glMaterialfv(GL_FRONT, GL_DIFFUSE, frontColor);					// Set diffuse property frontColor.
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF , 80.0f );                  // 80도 원뿔
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 80.0f );                 // 초점 설정
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}
void Simulator::DrawGround(void){
	
	glColor3f(1.0, 1.0, 1.0);
	glEnable(GL_LIGHTING);
	glBegin(GL_QUADS);
	for (int x = 0; x <= 127; x++) {
		for (int y = 0; y <= 127; y++) {
			if (x >= 32 && x <= 95 && y >= 32 && y <= 95)
				continue;

			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * y);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * y);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * (y + 1));
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * (y + 1));
		}
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);	
	for(int x = 32; x <= 95; x++){
		for(int y = 32; y <= 95; y++){	
			glTexCoord2f((-125 + 250.0f / 64 * x) / 250, (-125 + 250.0f / 64 * y) / 250);		
			glNormal3f(0.0,1.0,0.0);
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * y);

			glTexCoord2f((-125 + 250.0f / 64 * (x+1)) / 250, (-125 + 250.0f / 64 * y) / 250);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * y);

			glTexCoord2f((-125 + 250.0f / 64 * (x+1)) / 250, (-125 + 250.0f / 64 * (y+1)) / 250);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-250.0f + 250.0f / 64 * (x + 1), ground.y, -250.0f + 250.0f / 64 * (y + 1));

			glTexCoord2f((-125 + 250.0f / 64 * x) / 250, (-125 + 250.0f / 64 * (y+1)) / 250);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3f(-250.0f + 250.0f / 64 * x, ground.y, -250.0f + 250.0f / 64 * (y + 1));
		}
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);

}

GLuint Simulator::LoadTexture(const char* filename)
{
	GLuint texture;
	int width, height;
	unsigned char* data;
	FILE* file;
	file = fopen(filename, "rb");

	if (file == NULL) return 0;
	width = 512;
	height = 512;
	data = (unsigned char*)malloc(width * height * 3);
	fread(data, 54, 1, file);		//int size = fseek(file,);

	fread(data, width * height * 3, 1, file);
	fclose(file);
	for (int i = 0; i < width * height; ++i)
	{
		int index = i * 3;
		unsigned char B, R;
		B = data[index];
		R = data[index + 2];

		data[index] = R;
		data[index + 2] = B;

	}
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);


	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	free(data);

	return texture;
}





/* 망한 skybox의 산물
void Simulator::CreateProgram() {
	
	
	string skyvertexCode;
	string skyfragmentCode;
	ifstream skyvShaderFile;
	ifstream skyfShaderFile;

	try {
		skyvShaderFile.open("skyVertex.glsl");
		skyfShaderFile.open("skyFragment.glsl");
		stringstream skyvShaderStream;
		skyvShaderStream << skyvShaderFile.rdbuf();
		stringstream skyfShaderStream;
		skyfShaderStream << skyfShaderFile.rdbuf();
		skyvShaderFile.close();
		skyfShaderFile.close();
		skyvertexCode = skyvShaderStream.str();
		skyfragmentCode = skyfShaderStream.str();
	}
	catch (ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* skyvShaderCode = skyvertexCode.c_str();
	const char* skyfShaderCode = skyfragmentCode.c_str();


	vertShader2 = createShader(skyvShaderCode, GL_VERTEX_SHADER);
	fragShader2 = createShader(skyfShaderCode, GL_FRAGMENT_SHADER);
	GLint p2 = glCreateProgram();
	glAttachShader(p2, vertShader2);
	glAttachShader(p2, fragShader2);
	glLinkProgram(p2);

	

	char infoLog[512];
	int success2;
	glGetProgramiv(p2, GL_LINK_STATUS, &success2);
	if (!success2) {
		glGetProgramInfoLog(p2, 512, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s\n", infoLog);
	}
	program2 = p2;

}

unsigned int Simulator::createShader(const char* src, GLenum type) {
	unsigned int shader;
	shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		if (type == 35633) {
			printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n %s\n", infoLog);
		}
		else {
			printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n %s\n", infoLog);
		}
	}

	return shader;
}

void Simulator::initTextureSkybox() {
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TEXTURE3);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/cubemap1.jpg", &width, &height, &nrChannels, 0);
	//data = stbi_load("textures/left.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 0, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	data = stbi_load("textures/cubemap2.jpg", &width, &height, &nrChannels, 0);
	//data = stbi_load("textures/right.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 1, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	data = stbi_load("textures/cubemap3.jpg", &width, &height, &nrChannels, 0);
	//data = stbi_load("textures/bottom.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 2, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	data = stbi_load("textures/cubemap4.jpg", &width, &height, &nrChannels, 0);
	//data = stbi_load("textures/top.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 3, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	data = stbi_load("textures/cubemap5.jpg", &width, &height, &nrChannels, 0);
	//data = stbi_load("textures/front.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 4, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	data = stbi_load("textures/cubemap6.jpg", &width, &height, &nrChannels, 0);
	//data = stbi_load("textures/back.jpg", &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + 5, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Simulator::SkyDataTransfer() {
	float vertex[8 * 3] = {
		1.0f, -1.0f,  1.0f,//Vertex0
		1.0f, -1.0f, -1.0f,//Vertex1
		-1.0f, -1.0f, -1.0f,//Vertex2
		-1.0f, -1.0f,  1.0f,//Vertex3
		1.0f,  1.0f,  1.0f,//Vertex4
		1.0f,  1.0f, -1.0f,//Vertex5
		-1.0f,  1.0f, -1.0f,//Vertex6
		-1.0f,  1.0f,  1.0f //Vertex7
	};
	unsigned int index[6 * 6] = {
		0, 2, 1, 0, 3, 2,//Face1
		4, 3, 0, 4, 7, 3,//Face2
		5, 0, 1, 5, 4, 0,//Face3
		5, 7, 4, 5, 6, 7,//Face4
		1, 2, 6, 1, 6, 5,//Face5
		7, 2, 3, 7, 6, 2//Face6		
	};

	//Generating Buffer Objects
	glGenVertexArrays(1, &VAO3);
	glGenBuffers(1, &VBO3);
	glGenBuffers(1, &IBO3);
	//Transferring Vertex data to Device
	glBindVertexArray(VAO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Tranferring Index data to Device
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
*/