#pragma once
#include"glm/glm.hpp"
#include "Math.h"

//在模型和世界坐标系，使用的是四维齐次坐标
struct Vertex
{
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec2 texCoords;
	glm::vec3 normal;

	Vertex(const glm::vec4& _position, const glm::vec4& _color, const glm::vec2& _texCoords, const glm::vec3& _normal)
		:position(_position), color(_color), texCoords(_texCoords), normal(_normal)
	{}

	Vertex(
		const glm::vec3& _pos,
		const glm::vec4& _color = glm::vec4(0, 0, 0, 0),
		const glm::vec2& _tex = glm::vec2(0, 0),
		const glm::vec3& _normal = glm::vec3(0, 0, 1)
	) :
		position(_pos, 1.0f), color(_color), texCoords(_tex), normal(_normal) {}
	Vertex(const Vertex& v) :position(v.position), color(v.color), texCoords(v.texCoords), normal(v.normal) {}
};




//顶点着色器到片元着色器之间的中间结构体
class V2F {
public:
	glm::vec4 worldPos;
	glm::vec4 windowPos;
	glm::vec4 color;
	glm::vec2 texcoord;
	glm::vec3 normal;

	V2F() = default;
	~V2F() = default;
	V2F(
		const glm::vec4& _wPos,
		const glm::vec4& _pPos,
		const glm::vec4& _color,
		const glm::vec2& _tex,
		const glm::vec3& _normal
	) :
		worldPos(_wPos), windowPos(_pPos), color(_color), texcoord(_tex), normal(_normal) {}
	V2F(const V2F& v) :
		worldPos(v.worldPos), windowPos(v.windowPos), color(v.color), texcoord(v.texcoord), normal(v.normal) {}

	static V2F lerp(const V2F& v1, const V2F& v2, const float& factor) {
		V2F result;
		result.windowPos = Math::Lerp(v1.windowPos, v2.windowPos, factor);
		result.worldPos = Math::Lerp(v1.worldPos, v2.worldPos, factor);
		result.color = Math::Lerp(v1.color, v2.color, factor);
		result.normal = Math::Lerp(v1.normal, v2.normal, factor);
		result.texcoord = Math::Lerp(v1.texcoord, v2.texcoord, factor);
		return result;
	}
};

class Shader {

public:
	Shader() {
		ModelMatrix = glm::mat4(1.0f);
		ViewMatrix = glm::mat4(1.0f);
		ProjectMatrix = glm::mat4(1.0f);
	}
	~Shader() = default;
private:
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectMatrix;

public:
	V2F VertexShader(const Vertex& a2v) {
		V2F o;
		o.worldPos = ModelMatrix * a2v.position;
		// PVM*v
		o.windowPos = ProjectMatrix * ViewMatrix * o.worldPos;
		o.color = a2v.color;
		o.normal = a2v.normal;
		o.texcoord = a2v.texCoords;
		return o;
	}
	
	//现在直接输出点的颜色
	glm::vec4 FragmentShader(const V2F& v) {
		glm::vec4 color;
		color = v.color;
		return color;
	}

	void setModelMatrix(const glm::mat4& model) {
		ModelMatrix = model;
	}
	void setViewMatrix(const glm::mat4& view) {
		ViewMatrix = view;
	}
	void setProjectMatrix(const glm::mat4& project) {
		ProjectMatrix = project;
	}
};