#version 330
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 objectColor; //--- 응용 프로그램에서 설정한 객체의 색상
uniform vec3 lightPos;
uniform vec3 lightColor; //--- 응용 프로그램에서 설정한 조명 색상
uniform vec3 viewPos;
void main ()
{
	float ambientLight = 0.5; //--- 주변 조명 계수: 0.0 ≤ ambientLight ≤ 1.0
	vec3 ambient = ambientLight * lightColor; //--- 주변 조명값

	vec3 normalVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diffuseLight = max(dot(normalVector, lightDir), 0.0);
	vec3 diffuse = diffuseLight * lightColor;

	int shininess = 128;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, normalVector);
	float specularLight = max(dot(viewDir, reflectDir), 0.0);
	specularLight = pow(specularLight, shininess);
	vec3 specular = specularLight * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4 (result, 1.0);
}