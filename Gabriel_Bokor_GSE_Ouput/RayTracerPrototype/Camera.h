#pragma once
#include "Headers.h"
#include "Input.h"

struct Camera
{
	/*****************************************************************************//**
	*	Constructor for camera class
	*
	*	Initialises all relevant variables for camera control, such as position,
	*	mouse control variables and relevant matrices
	* 
	*	@param _fov The cameras FOV value
	*	@param _aspRat The cameras aspect ratio
	*	@param _near The cameras near value for perspective matrix
	*	@param _far The cameras near value for perspective matrix
	*********************************************************************************/
	Camera(float _fov, float _aspRat, float _near, float _far);

	~Camera();//! Deconstructor for camera class

	/*****************************************************************************//**
	*	Tick function for camera class
	*
	*	Recieves user input data in order to process any user movement through 
	*	different keyboard inputs. Also used to process user mouse movement. 
	*	Delta time is passed in to control movement with framerate
	* 
	*	Also disables mouse control when the GUI is being used
	* 
	*	@param _input Contains which keys have been pressed/released each frame
	*	@param _dTime The delta time variable for control with framerate
	*	@param _doMouse True/false for whether GUI is up
	*********************************************************************************/
	void Tick(std::shared_ptr<Input> _input, float _dTime, bool _doMouse);
	
	glm::mat4 GetTransformMat();

private:
	glm::mat4 m_projectionMat;//! Stores projection matrix value
	glm::mat4 m_viewMat;//! Stores view matrix value
	glm::mat4 m_transformMat;//! Stores transformation matrix value
	glm::mat4 m_direction;//! Stores direction value

	glm::vec3 m_position;//! Stores position vector
	glm::vec3 m_translation;//! Stores translation vector as temporary vector in movement control

	float m_mouseSpeed;//! Stores mouse speed value
	float m_moveSpeed;//! Stores movement speed value
	float m_cameraAngleX;//! Stores the X-axis camera angle value
	float m_cameraAngleY;//! Stores the Y-axis camera angle value
	float m_speedBoost;//! Stores the movement speed boost factor value
};