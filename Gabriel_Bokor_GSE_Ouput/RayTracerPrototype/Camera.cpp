#include "Camera.h"

Camera::Camera(float _fov, float _aspectRatio, float _near, float _far)
{
	//Variable initialisation - Camera matrices, control variables, other important values

	m_position = glm::vec3(0);
	m_projectionMat = glm::perspective(_fov, _aspectRatio, _near, _far);
	m_viewMat = glm::mat4(1);
	m_transformMat = glm::inverse(m_viewMat) * glm::inverse(m_projectionMat);

	m_cameraAngleX = 90.0f;
	m_cameraAngleY = 90.0f;

	m_mouseSpeed = 0.005f;
	m_moveSpeed = 5.0f;

	m_direction = glm::mat4(1);

	m_speedBoost = 1;
}

Camera::~Camera()
{
}

void Camera::Tick(std::shared_ptr<Input> _input, float _dTime, bool _doMouse)
{
	//Tracks camera values to actual mouse movement values
	if (_doMouse)
	{
		m_cameraAngleX -= _input->GetMouseMove().y * m_mouseSpeed;
		m_cameraAngleY -= _input->GetMouseMove().x * m_mouseSpeed;
	}

	m_direction = glm::mat4(1);
	m_direction = glm::rotate(m_direction, m_cameraAngleY, glm::vec3(0, 1, 0));
	m_direction = glm::rotate(m_direction, m_cameraAngleX, glm::vec3(1, 0, 0));

	//Process all keyboard inputs to appropriate movement
	//WASD controls for movement with LShift for speed boost
	if (_input->IsKey(lshift))
	{
		m_speedBoost = 2;
	}
	else
	{
		m_speedBoost = 1;
	}

	if (_input->IsKey(space))
	{
		m_position -= glm::vec3(0, 0.5, 0);
	}
	if (_input->IsKey(lcontrol))
	{
		m_position += glm::vec3(0, 0.5, 0);
	}
	if (_input->IsKey(w))
	{
		m_translation = (m_direction * glm::vec4(0, 0, -1, 1)) * m_moveSpeed * _dTime * m_speedBoost;
		m_position += m_translation;
	}
	if (_input->IsKey(a))
	{
		m_translation = (m_direction * glm::vec4(-1, 0, 0, 1)) * m_moveSpeed * _dTime * m_speedBoost;
		m_position += m_translation;
	}
	if (_input->IsKey(s))
	{
		m_translation = (m_direction * glm::vec4(0, 0, 1, 1)) * m_moveSpeed * _dTime * m_speedBoost;
		m_position += m_translation;
	}
	if (_input->IsKey(d))
	{
		m_translation = (m_direction * glm::vec4(1, 0, 0, 1)) * m_moveSpeed * _dTime * m_speedBoost;
		m_position += m_translation;
	}

	//Update matrices with position and viewing angle data
	m_viewMat = glm::mat4(1);
	m_viewMat = glm::translate(m_viewMat, m_position);
	m_viewMat = glm::rotate(m_viewMat, m_cameraAngleY, glm::vec3(0, 1, 0));
	m_viewMat = glm::rotate(m_viewMat, m_cameraAngleX, glm::vec3(1, 0, 0));
	m_transformMat = m_viewMat * glm::inverse(m_projectionMat);
}

glm::mat4 Camera::GetTransformMat()
{
	return m_transformMat;
}