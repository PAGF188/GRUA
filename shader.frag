#version 330 core

out vec4 fragColor;

in vec3 Normal;
in vec3 fragPos;
in vec2 TexCoord;

uniform sampler2D textura;

uniform vec3 posicionLuz;  //es indiferente para la luz ambiental
uniform vec3 colorLuz;     //para la luz ambiental
uniform vec3 colorObjeto;
uniform vec3 colorLuzDifusa;
uniform vec3 observadorPos;

void main(){

	fragColor=texture(textura,TexCoord);
	/*
	//notas
	//una vez calculada la intensidad de la luz, el color del fragmento dependera del color de la luz y el objeto
	//para varios objetos, distintas vec2 colorObjeto

	//luz ambiental
	float Iambiental=1.0; //intensidad luz ambiental-->Intensidada * Katenuacion || podriamos tener distintos valores para distintas materiales
	vec3 ambient=Iambiental*colorLuz;

	//luz difusa 
	//intensidad luz difusa--->intensidad*Katenuacion*producto escalar de (normal cara por vector rayo salida) basicamente al ser unitarios devuelve el coseno del angulo
	vec3 norm=normalize(Normal);
	vec3 direccionLuz=normalize(posicionLuz-fragPos);
	float diff=max(dot(norm,direccionLuz),0.0);
	vec3 difusa=3*diff*colorLuzDifusa;

	//luz especular en relacion al observador
	float k=1;
	vec3 observadorDir=normalize( observadorPos-fragPos);
	vec3 reflectDir=reflect(-direccionLuz,norm);
	float espec = pow(max(dot(observadorDir,reflectDir),0.0), 128);
	vec3 especular=k*espec*colorLuz;

	vec3 result=(ambient+difusa+especular)*colorObjeto;
	fragColor=vec4(result*texture(textura,TexCoord),1.0);
	*/
}