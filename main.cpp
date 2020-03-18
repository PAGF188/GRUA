/*Grua 3.3 dibujado sobre plano x-z*/
/*NOTAS: lapsoTime es del deltaTime */
/*CONTROLES
WASD-->BASE
FTGH-->1 BRAZ0
JIKL-->2 BRAZO
f1-->camara externa
	-raton mover enfoque
	-rueda raton fov
f2-->camara 3 persona
f3-->camara 1 persona*/
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <vector>
#include "Sphere.h"
#include "Grua.h"
//Para las transformaciones
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VELOCIDAD 2.0
#define MOVIMIENTO 1.0
#define VELOCIDAD_ROTACION 1.0
#define PI 3.1416

GLFWwindow* window;

float borrarX = 0.0;
float borrarZ = 0.0;

//TAMAÑO VENTANA
int ancho = 768;
int alto = 768;

//VAO PARA EL SUELO y textyra
unsigned int VAO;
GLuint texturaSuelo;
unsigned int VAOsky;
GLuint texturasky;
//para mar
int iteradorAgua = 0; //iterador textura agua
GLuint *texturaMar;

//PARA MOVIMIENTO CAMARA
int camara = 2;  //inicializada a camara en 3 persona
// position
glm::vec3 position = glm::vec3(0, 15, 10);
glm::vec3 direction;
glm::vec3 right;
float horizontalAngle = 3.14f;
float verticalAngle = 0.0f;
float fov = 100.0f;
float speed = 3.0f; 
float mouseSpeed = 0.2f;

//obetos
Grua grua;
glm::vec3 pos=glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 pos2=glm::vec3(0.0f, 0.0f, 0.0f);;
glm::vec3 dir= glm::vec3(0.0f, 0.0f, 0.0f);;
//funciones
void camera(GLuint shader);
void tercera_persona(GLuint shader);
void primera_persona(GLuint shader);
void framebuffer_size_callback(GLFWwindow  *window, int width, int height);
void processInput(GLFWwindow *window);

extern GLuint setShaders(const char *nVertx, const char *nFrag);
GLuint shaderProgram;
GLuint shaderSKY;


//para calcular tiempo
double lastTime = glfwGetTime();
double currentTime;
double lapsoTime;
int nbFrames = 0;

void tiempo() {
	static double unSegundo = 0;
	currentTime = glfwGetTime();
	nbFrames++;
	lapsoTime = currentTime - lastTime;
	unSegundo = unSegundo + lapsoTime;
	if (unSegundo >= 1.0) {
		printf("%f frames/s\n", double(nbFrames));
		nbFrames = 0;
		unSegundo = 0;
	}
	lastTime = currentTime;
}

void skyBLOCK() {
	unsigned int VBO;
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &VAOsky);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAOsky);
	glGenBuffers(1, &VBO);

	//datos para los vertices VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	//posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}


void Suelo() {
	unsigned int VBO, EBO;

	float vertices[] = {

		0.5f, 0.0f, 0.5f,   0.0f, 1.0f,0.0f,  1.0f,0.0f,
		0.5f, 0.0f, -0.5f,  0.0f, 1.0f,0.0f,  1.0f,1.0f,
		-0.5f,0.0f,-0.5f,   0.0f, 1.0f,0.0f,  0.0f,1.0f,

		-0.5f,0.0f,-0.5f,   0.0f, 1.0f,0.0f,  0.0f,1.0f,
		-0.5f, 0.0f, 0.5f,  0.0f, 1.0f,0.0f,  0.0f,0.0f,
		 0.5f, 0.0f, 0.5f,  0.0f, 1.0f,0.0f,  1.0f,0.0f,

	};

	glGenVertexArrays(1, &VAO);
	// bind the Vertex Array Object first.
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	
	//datos para los vertices VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteBuffers(1, &VBO);
}

void Display() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram);

	if (camara == 1) {
		camera(shaderProgram);
	}
	if (camara == 2) {
		tercera_persona(shaderProgram);
	}
	if (camara == 3) {
		primera_persona(shaderProgram);
	}


	//dibujamos el sky block
	
	
	glDepthMask(GL_FALSE);
	glUseProgram(shaderSKY);
	glm::mat4 transformsky;
	unsigned int transformLocKsky = glGetUniformLocation(shaderSKY, "transform");
	transformsky = glm::mat4();
	transformsky = glm::translate(transformsky, glm::vec3(0, 0, 0));
	glUniformMatrix4fv(transformLocKsky, 1, GL_FALSE, glm::value_ptr(transformsky));

	glBindVertexArray(VAOsky);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texturasky);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDepthMask(GL_TRUE);
	
	glUseProgram(shaderProgram);
	glm::mat4 transform;
	unsigned int transformLocK = glGetUniformLocation(shaderProgram, "transform");
	//empezamos dibujando el suelo
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, texturaSuelo);
	glBindVertexArray(VAO);
	for (int i = -50; i <= 50; i++) {
		for (int j = 50; j >= 0; j--) {
			transform = glm::mat4();
			transform = glm::translate(transform, glm::vec3(i, 0.0f, j-10));
			glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
			
			glDrawArrays(GL_TRIANGLES, 0, 6); //Para dibuhar con array, 2 argumento numero de vertices
			
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	//dibujamos el mar un solo cubo
	glBindTexture(GL_TEXTURE_2D, texturaMar[iteradorAgua]);

	transform = glm::mat4();
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -14.0f));
	transform = glm::scale(transform, glm::vec3(7.0f,7.0f,7.0f));
	glUniformMatrix4fv(transformLocK, 1, GL_FALSE, glm::value_ptr(transform));
	glDrawArrays(GL_TRIANGLES, 0, 6); //Para dibuhar con array, 2 argumento numero de vertices

	glBindTexture(GL_TEXTURE_2D, 0);
	iteradorAgua++;
	if (iteradorAgua == 15) {
		iteradorAgua = 0;
	}
	glBindVertexArray(0);

	//dibujamos la grua
	grua.draw(shaderProgram);
	//en pos2 guardamos la posicion antigua
	pos2 = pos;
	//en pos la nueva
	pos = glm::vec3(grua.getX(), grua.getY(), grua.getZ());
	//vector director posicion actual-antigua
	dir = pos - pos2;
	glm::normalize(dir);
	//deteccion de closion grua con agua
	if (((-3.5<grua.getX()+1.5) && (3.5>grua.getX()-1.5)) && ((-17.5<grua.getZ()+1.5)&&(-10.5>grua.getZ()-1.5))) {
		printf("colision\n");
		grua.setX(pos[0]-dir[0]*1.5);
		grua.setY(pos[1]-dir[1]*1.5);
		grua.setZ(pos[2]-dir[2]*1.5);
	}

	
}

void tercera_persona(GLuint shader) {
	glViewport(0, 0, ancho, alto);
	//proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(60.0f), (float)ancho / (float)alto, 0.0001f, 50.0f);
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//view
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(glm::vec3(grua.getX(),grua.getY()+10.0f,grua.getZ()+10.0f), glm::vec3(grua.getX(), grua.getY(),grua.getZ()), glm::vec3(0.0f, 1.0f,0.0f));
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//para la luz especular
	unsigned int observadorPos = glGetUniformLocation(shader, "observadorPos");
	glUniform3f(observadorPos, grua.getX(), grua.getY() + 10.0f, grua.getZ() + 10.0f);
}

void primera_persona(GLuint shader) {
	glViewport(0, 0, ancho, alto);

	//proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(100.0f), (float)ancho / (float)alto, 0.0001f, 50.0f);
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//view
	glm::mat4 view;
	view = glm::mat4();
	view = glm::lookAt(glm::vec3(grua.getX()-1.0*sin(grua.getAnguloBase()), grua.getY() + 2.0f, grua.getZ() - 1.0*cos(grua.getAnguloBase())), glm::vec3(grua.getX()-3*sin(grua.getAnguloBase()), grua.getY()+2.0f, grua.getZ()-3.0*cos(grua.getAnguloBase())), glm::vec3(0.0f, 1.0f, 0.0f));
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//para la luz especular
	unsigned int observadorPos = glGetUniformLocation(shader, "observadorPos");
	glUniform3f(observadorPos, grua.getX() - 1.0*sin(grua.getAnguloBase()), grua.getY() + 2.0f, grua.getZ() - 1.0*cos(grua.getAnguloBase()));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	fov = fov - yoffset * 5;
}

void camera(GLuint shader) {

	//proyeccion
	glm::mat4 projection;
	projection = glm::mat4();
	projection = glm::perspective(glm::radians(fov), (float)ancho / (float)alto, 0.0001f, 1000.0f);
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


	//POSICIOND EL RATON
	double xpos, ypos;
	glfwGetCursorPos(window,&xpos, &ypos);
	//la devolvemos a su pocion inicial
	glfwSetCursorPos(window, ancho / 2, alto / 2);

	horizontalAngle += mouseSpeed * lapsoTime * float((float)ancho / 2 - xpos);
	verticalAngle += mouseSpeed * lapsoTime * float((float)alto / 2 - ypos);

	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);


	glm::mat4 view; // Se Calcula
	unsigned int viewLoc = glGetUniformLocation(shader, "view");

	//calculamos la wiew
	view = glm::lookAt(
		position,           // posicion
		position+direction, //mira (vector)
		up                  
	);

	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	//para la luz especular
	unsigned int observadorPos = glGetUniformLocation(shader, "observadorPos");
	glUniform3f(observadorPos,position[0],position[1],position[2]);
}

void luz() {
	glUseProgram(shaderProgram);
	//El color del objeto
	unsigned int colorLoc = glGetUniformLocation(shaderProgram, "colorObjeto");
	glUniform3f(colorLoc, 0.2f, 0.2f, 0.2f);
	//El color de la luz ambiental
	unsigned int ambiental = glGetUniformLocation(shaderProgram, "colorLuz");
	glUniform3f(ambiental, 1.0f, 1.0f, 1.0f);
	//color luz difusa
	unsigned int difusa = glGetUniformLocation(shaderProgram, "colorLuzDifusa");
	glUniform3f(difusa, 1.0f, 0.0f, 0.0f);
	//posicion de la luz difusa
	unsigned int lightPosLoc = glGetUniformLocation(shaderProgram, "posicionLuz");
	glUniform3fv(lightPosLoc, 1, glm::value_ptr(grua.getPosicionTope()));
	//glUniform3f(lightPosLoc,borrarX,6,borrarZ);
}

void openGlInit() {
	//Habilito aqui el depth en vez de arriba para los usuarios de linux y mac mejor arriba
	//Incializaciones varias
	glClearDepth(1.0f); //Valor z-buffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // valor limpieza buffer color
	glEnable(GL_DEPTH_TEST); // z-buffer
	glEnable(GL_CULL_FACE); //ocultacion caras back
	glCullFace(GL_BACK);
}

void framebuffer_size_callback(GLFWwindow  *window, int _ancho, int _alto) {
	ancho = _ancho;
	alto = _alto;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Creo la ventana
														
	window = glfwCreateWindow(ancho, alto, "GRUA3.3", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback); //accion de hacer scroll con el raton

	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	openGlInit();
	
	//inicializo los objetos
	Suelo();
	skyBLOCK();
	texturaSuelo=grua.cargarTextura((char *)"suelo1.jpg");
	texturaMar = grua.cargarAgua();
	grua.setTexturaBola(grua.cargarTextura((char *) "stone.png"));
	grua.setTexturaCuerpo(grua.cargarTextura((char *) "wood.png"));
	std::vector<std::string> faces =
	{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"front.jpg",
		"back.jpg"
	};

	texturasky = grua.loadCubemap(faces);

	//generarShader();
	shaderSKY = setShaders("sky.vert","sky.frag");
	shaderProgram = setShaders("shader.vert", "shader.frag");
	// Lazo de la ventana mientras no la cierre
	// -----------
	
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);
		//camera(window);
		luz();
		Display();
		tiempo();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	//movimeinto base
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		grua.setX(grua.getX() - VELOCIDAD * sin(grua.getAnguloBase())*lapsoTime);
		grua.setZ(grua.getZ() - VELOCIDAD * cos(grua.getAnguloBase())*lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		grua.setX(grua.getX() + VELOCIDAD * sin(grua.getAnguloBase())*lapsoTime);
		grua.setZ(grua.getZ() + VELOCIDAD * cos(grua.getAnguloBase())*lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		grua.setAnguloBase(grua.getAnguloBase() - VELOCIDAD_ROTACION * lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		grua.setAnguloBase(grua.getAnguloBase() + VELOCIDAD_ROTACION * lapsoTime);
	}
	//movimiento brazo1
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		grua.setAnguloX1(grua.getAnguloX1() - MOVIMIENTO * lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		grua.setAnguloX1(grua.getAnguloX1() + MOVIMIENTO * lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		grua.setAnguloZ1(grua.getAnguloZ1() + MOVIMIENTO * lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		grua.setAnguloZ1(grua.getAnguloZ1() - MOVIMIENTO * lapsoTime);
	}
	//movimiento brazo2
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		grua.setAnguloX2(grua.getAnguloX2() - MOVIMIENTO * lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		grua.setAnguloX2(grua.getAnguloX2() + MOVIMIENTO * lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		grua.setAnguloZ2(grua.getAnguloZ2() - MOVIMIENTO * lapsoTime);
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		grua.setAnguloZ2(grua.getAnguloZ2() + MOVIMIENTO * lapsoTime);
	}
	//camara
	//camara libre
	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
		camara = 1;
	}
	//camara tercera_persona
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
		camara = 2;
	}
	//camara externa
	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS) {
		camara = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		borrarZ--;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		borrarZ++;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		borrarX--;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		borrarX++;
	}
}

