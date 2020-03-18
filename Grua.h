#pragma once
//Para las transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Grua {
public:
	//constructor
	Grua(float x = 0.0f, float y = 0.0f, float z = 0.0f, float anguloX1 = 0.0f, float anguloZ1 = 0.0f, float anguloX2 = 0.0f, float anguloZ2 = 0.0f,float anguloBase=0.0f);
	//destructor
	~Grua(){}

	//metodos
	//getters
	float getX() const { return x; }
	float getY() const { return y; }
	float getZ() const { return z; }
	float getAnguloX1() const { return anguloX1; }
	float getAnguloZ1() const { return anguloZ1; }
	float getAnguloX2() const { return anguloX2; }
	float getAnguloZ2() const { return anguloZ2; }
	float getAnguloBase() const { return anguloBase; }
	GLuint getTexturaBola() const { return texturaBola; }
	GLuint getTexturaCuerpo() const { return texturaCuerpo; }
	glm::vec4 getPosicionTope() const { return posicionTope; }
	//setters
	void set(float x, float y, float z, float anguloX1, float anguloZ1, float anguloX2, float anguloZ2,float anguloBase);
	void setX(float x);
	void setY(float y);
	void setZ(float z);
	void setAnguloX1(float anguloX1);
	void setAnguloZ1(float anguloZ1);
	void setAnguloX2(float anguloX2);
	void setAnguloZ2(float anguloZ2);
	void setAnguloBase(float anguloBase);
	void setTexturaBola(GLuint texturaBola);
	void setTexturaCuerpo(GLuint texturaCuerpo);

	//otros metodos
	void draw(GLuint shader);
	GLuint cargarTextura(char * path);
	GLuint * cargarAgua();
	GLuint loadCubemap(std::vector<std::string> faces);

protected:
private:
	//atributos
	float x;
	float y;
	float z;
	float anguloX1;
	float anguloZ1;
	float anguloX2;
	float anguloZ2;
	float anguloBase;
	GLuint texturaBola;
	GLuint texturaCuerpo;
	glm::vec4 posicionTope;
	
	//metodos auxiliares
	//devuelve el VAO de un cubo de color rojo
	GLuint Base();
};
