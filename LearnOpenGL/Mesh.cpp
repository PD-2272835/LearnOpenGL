#include "Mesh.h"

void Mesh::Render(glm::mat4& transform)
{
	Vertices->Bind();
	Elements->Bind();

	ShaderProgram->SetMat4("model", transform);
	glDrawElements(GL_TRIANGLES, Elements->Size, GL_UNSIGNED_INT, 0);

	Vertices->Unbind();
	Elements->Unbind();
}
