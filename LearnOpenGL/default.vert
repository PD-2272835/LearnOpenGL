#version 330 core
layout (location = 0) in vec3 aPos;		//vertex position is the first vec3 attribute
layout (location = 1) in vec3 aColor;

out vec3 VertexColor; //output to fragment shader


uniform mat4 model; //tranform from local to world space
uniform mat4 view; //transform from world to view space
uniform mat4 projection; //transform from view to clip space


void main()
{
	// Vclip = Mproj * Mview * Mmodel * Vlocal
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	VertexColor = aColor;
}