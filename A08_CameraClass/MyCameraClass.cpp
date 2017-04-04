#include "MyCameraClass.h"


MyCameraClass::MyCameraClass(vector3 position, vector3 target, vector3 up) {
	m_fRotation;
	m_v3ViewVector;
	m_v3UpVector;

	m_m4ViewMatrix = glm::translate(position);
	m_m4ProjMatrix = glm::perspective(m_fFoV, 4.0f/3.0f, 0.1f, 100.0f);
	m_m4OrthoMatrix = glm::ortho(0.0f, 400.0f, 0.0f, 300.0f, 0.1f, 100.0f);
}

// Get the camera's view matrix
matrix4 MyCameraClass::GetView(void) {
	return m_m4ViewMatrix;
}

// Get the camera's projection matrix
matrix4 MyCameraClass::GetProject(bool bOrthographic) {
	if (bOrthographic) {
		return m_m4OrthoMatrix;
	}
	else {
		return m_m4ProjMatrix;
	}
}

// Set the position of the camera
void MyCameraClass::SetPosition(vector3 v3Position) {
	m_v3ViewVector = v3Position;
	quaternion q1 = glm::angleAxis(m_fRotation, m_v3UpVector);
	matrix4 m1 = ToMatrix4(q1);
	m_m4ViewMatrix = glm::translate(m_v3ViewVector) * m1;
	
}

// Set the direction the camera is facing
void MyCameraClass::SetTarget(vector3 v3Target) {
	vector3 v3Dir = glm::normalize(m_v3ViewVector - v3Target);

}

// Set the up direction of the camera
void MyCameraClass::SetUp(vector3 v3Up) {

}

// Move the camera forward along its z axis
void MyCameraClass::MoveForward(float fIncrement) {
	m_v3ViewVector.x += fIncrement;
	SetPosition(m_v3ViewVector);
}

// Move the camera sideways along its x axis
void MyCameraClass::MoveSideWays(float fIncrement) {

}

// Move the camera vertically along its y axis
void MyCameraClass::MoveVertical(float fIncrement) {

}

// Change orientation of camera around its x axis
void MyCameraClass::ChangePitch(float fIncrement) {

}

// Change orientation of camera around its z axis
void MyCameraClass::ChangeRoll(float fIncrement) {

}
// Change orientation of camera around its y axis
void MyCameraClass::ChangeYaw(float fIncrement) {

}