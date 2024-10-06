#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "VertexData.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Material.h"
#include "RenderPass.h"

struct PassRenderPara
{
	const Texture* pMainTexture = nullptr;
	const Shader* pShader = nullptr;
	const Material* pMaterial = nullptr;
};

class Object
{
public:
	explicit Object(const VertexData* pVertexData);
	virtual ~Object() = default;

public:
	auto SetPosition(const glm::vec3& position) -> void;
	auto SetPosition(float x, float y, float z) -> void;
	auto SetRotation(const glm::quat& rotation) -> void;
	auto SetScale(const glm::vec3& scale) -> void;

	auto GetPosition() const -> const glm::vec3&;
	auto GetModelMatrix() const -> glm::mat4;
	auto GetVertexData() const-> const VertexData*;

public:
	template<RenderPass Pass>
	auto SetRenderPassPara(const Shader* pShader, const Texture* pMainTexture = nullptr, const Material* pMaterial = nullptr) -> void
	{
		_passRenderParaMap[Pass].pShader = pShader;
		_passRenderParaMap[Pass].pMainTexture = pMainTexture;
		_passRenderParaMap[Pass].pMaterial = pMaterial;
	}

	template<RenderPass Pass>
	auto GetRenderPassPara() -> const PassRenderPara&
	{
		// If key is not exist, add a default with all parameters nullptr.
		return _passRenderParaMap[Pass];
	}

protected:
	// World state
	glm::vec3 _position;
	glm::quat _rotation;
	glm::vec3 _scale;

	// Render context
	const VertexData* _pVertexData;
	std::unordered_map<RenderPass, PassRenderPara> _passRenderParaMap;
};

class PointLightObject : public Object
{
public:
	PointLightObject(const VertexData* pVertexData);

public:
	auto SetColor(const glm::vec3& color) -> void;
	auto GetColor() const -> const glm::vec3&;
	auto SetLinerAttenuation(float value) -> void;
	auto GetLinerAttenuation() const -> float;
	auto SetQuadraticAttenuation(float value) -> void;
	auto GetQuadraticAttenuation() -> float;

private:
	glm::vec3 _color;
	float _linerAttenuation;
	float _quadraticAttenuation;
};