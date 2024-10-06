#include "Object.h"

Object::Object(const VertexData* pVertexData)
	: _pVertexData(pVertexData)
	, _position(0.0f, 0.0f, 0.0f)
	, _rotation(glm::identity<glm::quat>())
	, _scale(1.0f, 1.0f, 1.0f)
{
}

auto Object::SetPosition(const glm::vec3& position) -> void
{
	_position = position;
}

auto Object::SetPosition(float x, float y, float z) -> void
{
	_position = glm::vec3(x, y, z);
}

auto Object::SetRotation(const glm::quat& rotation) -> void
{
	_rotation = rotation;
}

auto Object::SetScale(const glm::vec3& scale) -> void
{
	_scale = scale;
}

auto Object::GetModelMatrix() const -> glm::mat4
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), _position);
	glm::mat4 rotation = glm::mat4_cast(_rotation);
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), _scale);
	return translate * rotation * scale;
}

auto Object::GetPosition() const -> const glm::vec3&
{
	return _position;
}

auto Object::GetVertexData() const-> const VertexData*
{
	return _pVertexData;
}

PointLightObject::PointLightObject(const VertexData* pVertexData)
	: Object(pVertexData)
{
}

auto PointLightObject::SetColor(const glm::vec3& color) -> void
{
	_color = color;
}

auto PointLightObject::GetColor() const -> const glm::vec3&
{
	return _color;
}

auto PointLightObject::SetLinerAttenuation(float value) -> void
{
	_linerAttenuation = value;
}

auto PointLightObject::GetLinerAttenuation() const -> float
{
	return _linerAttenuation;
}

auto PointLightObject::SetQuadraticAttenuation(float value) -> void
{
	_quadraticAttenuation = value;
}

auto PointLightObject::GetQuadraticAttenuation() -> float
{
	return _quadraticAttenuation;
}
