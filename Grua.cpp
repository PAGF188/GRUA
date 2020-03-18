#pragma once
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <math.h> 
#include "Sphere.h"


#ifdef _WIN32
#include <windows.h>    // include windows.h to avoid thousands of compile errors even though this class is not depending on Windows
#endif

#include <iostream>
#include <iomanip>
#include <cmath>
#include "Grua.h"

#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>


Grua::Grua(float x, float y, float z, float anguloX1, float anguloZ1, float anguloX2, float anguloZ2, float anguloBase) {
	set(x, y, z, anguloX1, anguloZ1, anguloX2, anguloZ2,anguloBase);
	posicionTope = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

void Grua::set(float x, float y, float z, float anguloX1, float anguloZ1, float anguloX2, float anguloZ2,float anguloBase) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->anguloX1 = anguloX1;
	this->anguloZ1 = anguloZ1;
	this->anguloX2 = anguloX2;
	this->anguloZ2 = anguloZ2;
	this->anguloBase = anguloBase;
}

void Grua::setX(float x) {
	this-> x = x;
}
void Grua::setY(float y) {
	this->y = y;
}
void Grua::setZ(float z) {
	this->z = z;
}
void Grua::setAnguloX1(float anguloX1) {
	this->anguloX1 = anguloX1;
}
void Grua::setAnguloZ1(float anguloZ1) {
	this->anguloZ1 = anguloZ1;
}
void Grua::setAnguloX2(float anguloX2) {

	this->anguloX2 = anguloX2;
}
void Grua::setAnguloZ2(float anguloZ2) {
	this->anguloZ2 = anguloZ2;
}
void Grua::setAnguloBase(float anguloBase) {
	this->anguloBase = anguloBase;
}
void Grua::setTexturaBola(GLuint texturaBola) {
	this->texturaBola = texturaBola;
}
void Grua::setTexturaCuerpo(GLuint texturaCuerpo) {
	this->texturaCuerpo = texturaCuerpo;
}

//la forma de moverla sera modificando los parametros de x e y
void Grua::draw(GLuint shader) {
	posicionTope = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	Sphere esfera;
	unsigned int transformLocK = glGetUniformLocation(shader, "transform");

	glBindVertexArray(Grua::Base());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Esto es la base
	glm::mat4 transform;
	glm::mat4 temp;
	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(this->x, 0.5f, this->z));
	transform = glm::rotate(transform, anguloBase,glm::vec3(0.0f, 1.0f, 0.0f));
	temp = transform;
	transform = glm::scale(transform, glm::vec3(1.3, 0.2, 1.3));
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));  //esto se usa para cargar la matriz en el shader
	glBindTexture(GL_TEXTURE_2D, this->texturaCuerpo);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	//esto es la primera esfera.
	transform = glm::mat4();
	transform = temp;
	transform = glm::translate(transform, glm::vec3(0.0f, 0.25, 0.0f));
	transform = glm::rotate(transform, anguloZ1, glm::vec3(0.0f, 0.0f, 1.0f));    //rotacion izquierda-derecha (z)
	transform = glm::rotate(transform, anguloX1, glm::vec3(1.0f, 0.0f, 0.0f));	  //rotacion adelante atras (x)
	temp = transform;
	transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
	glBindTexture(GL_TEXTURE_2D, this->texturaBola);
	esfera.draw();
	glBindTexture(GL_TEXTURE_2D,0);
	
	//1 brazo
	glBindVertexArray(Grua::Base());
	transform = glm::mat4();
	transform = temp;
	transform = glm::translate(transform, glm::vec3(0.0f, 1.7f, 0.0f));
	temp = transform;
	transform = glm::scale(transform, glm::vec3(0.15, 1.37, 0.15));
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));  //esto se usa para cargar la matriz en el shader
	glBindTexture(GL_TEXTURE_2D, this->texturaCuerpo);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	//2 esfera
	transform = glm::mat4();
	transform = temp;
	transform = glm::translate(transform, glm::vec3(0.0f,1.5f, 0.0f));
	transform = glm::rotate(transform, anguloZ2, glm::vec3(0.0f, 0.0f, 1.0f));    //rotacion izquierda-derecha (z)
	transform = glm::rotate(transform, anguloX2, glm::vec3(1.0f, 0.0f, 0.0f));	  //rotacion adelante atras (x)
	temp = transform;
	transform = glm::scale(transform, glm::vec3(0.3, 0.3, .3));
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
	glBindTexture(GL_TEXTURE_2D, this->texturaBola);
	esfera.draw();
	glBindTexture(GL_TEXTURE_2D, 0);
	
	//2 brazo
	glBindVertexArray(Grua::Base());
	transform = glm::mat4();
	transform = temp;
	transform = glm::translate(transform, glm::vec3(0.0f, 1.0f, 0.0f));
	//nota para tener la posicion del tope
	posicionTope = transform * posicionTope;

	transform = glm::scale(transform, glm::vec3(0.05, 1, 0.05));
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));  //esto se usa para cargar la matriz en el shader
	glBindTexture(GL_TEXTURE_2D, this->texturaCuerpo);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	

}
//cubo de color rojo
GLuint Grua::Base() {

	GLuint VAO;
	unsigned int VBO, NBO,TBO;
	
	float vertices[] = {
		-1.0f,-1.0f,-1.0f, // triángulo 1 : comienza
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triángulo 1 : termina
		1.0f, 1.0f,-1.0f, // triángulo 2 : comienza
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, // triángulo 2 : termina
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};
	
	float normales[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	float uv[] = {
		0.000059f, 0.000004f,
		0.000103f, 0.336048f,
		0.335973f,  0.335903f,
		1.000023f,  0.000013f,
		0.667979f,  0.335851f,
		0.999958f,  0.336064f,
		0.667979f,  0.335851f,
		0.336024f,  0.671877f,
		0.667969f,  0.671889f,
		1.000023f,  0.000013f,
		0.668104f,  0.000013f,
		0.667979f,  0.335851f,
		0.000059f,  0.000004f,
		0.335973f,  0.335903f,
		0.336098f,  0.000071f,
		0.667979f,  0.335851f,
		0.335973f,  0.335903f,
		0.336024f,  0.671877f,
		1.000004f,  0.671847f,
		0.999958f,  0.336064f,
		0.667979f,  0.335851f,
		0.668104f,  0.000013f,
		0.335973f,  0.335903f,
		0.667979f,  0.335851f,
		0.335973f,  0.335903f,
		0.668104f,  0.000013f,
		0.336098f,  0.000071f,
		0.000103f,  0.336048f,
		0.000004f,  0.671870f,
		0.336024f,  0.671877f,
		0.000103f,  0.336048f,
		0.336024f,  0.671877f,
		0.335973f,  0.335903f,
		0.667969f,  0.671889f,
		1.000004f,  0.671847f,
		0.667979f,  0.335851f
	};
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//posicion
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


	//normales
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normales), normales, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	//uv
	glGenBuffers(1, &TBO);
	glBindBuffer(GL_ARRAY_BUFFER, TBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &NBO);
	glDeleteBuffers(1, &TBO);

	return(VAO);
}


GLuint Grua::cargarTextura(char *path) {

	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);

	int ancho, alto, canales;
	unsigned char *data = NULL;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load(path, &ancho, &alto, &canales, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	stbi_image_free(data);
	return(textura);
}

GLuint * Grua::cargarAgua() {

	std::vector<std::string> path =
	{
		"caust00.png",
		"caust01.png",
		"caust02.png",
		"caust03.png",
		"caust04.png",
		"caust05.png",
		"caust06.png",
		"caust07.png",
		"caust08.png",
		"caust09.png",
		"caust10.png",
		"caust12.png",
		"caust13.png",
		"caust14.png",
		"caust15.png",
	};

	int ancho, alto, canales;

	GLuint *textura;
	textura = (GLuint *)malloc(sizeof(GLuint) * 15);

	glGenTextures(15, textura);

	for (int i = 0; i < 15; i++) {
		glBindTexture(GL_TEXTURE_2D, textura[i]);

		unsigned char *data = stbi_load(path[i].c_str(), &ancho, &alto, &canales, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ancho, alto, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			printf("Error al cargar alguna textura del agua");
			exit(-1);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	}

	return(textura);
}

GLuint Grua::loadCubemap(std::vector<std::string> faces) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			exit(-1);
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}