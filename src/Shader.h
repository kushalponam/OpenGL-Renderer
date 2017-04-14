#ifndef _SHADER_H
#define _SHADER_H

#include "Core.h"
#include "RenderableObject.h"

class Scene;
class Camera;
class Shader : GLObject {

public:

	Shader(std::string i_vertexShaderName, std::string i_fragmentShaderName);
	virtual bool LoadShaders() = 0;
	virtual void RegisterUniforms() = 0;
	virtual void BindObject(Object *obj)=0;
	virtual void SetUniform(int id, int value)=0;
	void Update() {};
	void Render() {};
	void CleanUp() {};
	virtual void Bind() = 0;
	void ReflectObjects(int direction);
	
protected:
	std::string vertexShaderName;
	std::string fragmentShaderName;
	cy::GLSLProgram program;
	
	std::vector<RenderableObject*> ObjectList;
	
	glm::mat4 BiasMVP = glm::mat4{
		0.5f,0.0f,0.0f,0.0f,
		0.0f,0.5f,0.0f,0.0f,
		0.0f,0.0f,0.5f,0.0f,
		0.5f,0.5f,0.5f,1.0f
	};

};


class BlinnShader : public Shader {

public:
	BlinnShader(std::string i_vertexShaderName, std::string i_fragmentShader);
	
	bool LoadShaders();
	void RegisterUniforms();
	void BindObject(Object *obj);
	void Bind() { program.Bind(); }
	void SetUniform(int id, int value) override
	{ Bind(); program.SetUniform(id, value); };

	void Init();
	void Update();
	void Render();
	void CleanUp();
};


class QuadShader : public Shader {

public:
	QuadShader(std::string i_vertexShaderName, std::string i_fragmentShader);

	bool LoadShaders();
	void RegisterUniforms();
	void BindObject(Object *obj);
	void Bind() { program.Bind(); }
	void SetUniform(int id, int value) override
	{ Bind(); program.SetUniform(id, value); };
	void Init();
	void Update();
	void Render();
	void CleanUp();

private:
//	std::vector<RenderableObject*> ObjectList;

};

class DepthShader : public Shader
{
public:
	DepthShader(std::string i_vertexShaderName, std::string i_fragmentShader);

	void Init() override;
	bool LoadShaders() override;
	void RegisterUniforms() override;
	void BindObject(Object* obj) override;
	void Bind() { program.Bind(); }
	void SetUniform(int id, int value) override
	{ Bind(); program.SetUniform(id, value); };
	void Update();
	void Render();
	void CleanUp();

	void SetCurrentLight(int i_currentLight) { currentLight = i_currentLight; }
private:
	int currentLight;

};

class PBRShader : public Shader
{
public:
	PBRShader(std::string i_vertexShaderName, std::string i_fragmentShader);

	bool LoadShaders();
	void RegisterUniforms();
	void BindObject(Object *obj);
	void Bind() { program.Bind(); }
	void SetUniform(int id, int value) override
	{
		Bind(); program.SetUniform(id, value);
	};

	void Init();
	void Update();
	void Render();
	void CleanUp();
};


#endif // !_SHADER_H

