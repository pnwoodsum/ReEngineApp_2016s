#include "MyCameraClass.h"

MyCameraClass::MyCameraClass() {
	m_fYaw = 0.f;
	m_fPitch = 0.f;
	m_fRoll = 0.f;

	// Initialize variables
	m_v3Up = REAXISY;
	m_v3Forward = REAXISZ;
	m_v3Right = REAXISX;

	float fFov = 45.f;

	m_v3View = vector3(0.0f, 0.0f, 50.f);
	m_v3Target = vector3(0.f, 0.f, 0.f);

	// Set init target
	SetTarget(m_v3Target);

	// Set init vars
	m_v3UpInit = m_v3Up;
	m_v3ForwardInit = m_v3Forward;
	m_v3RightInit = m_v3Right;

	// Set proj and ortho
	m_m4ProjMatrix = glm::perspective(fFov, 1280.f / 720.f, 0.1f, 200.f);
	m_m4OrthoMatrix = glm::ortho(-20.f, 20.0f, -15.0f, 15.0f, 0.01f, 200.0f);
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

// Set the direction the camera is facing
void MyCameraClass::SetTarget(vector3 v3Target) {
	// Update target
	m_v3Target = v3Target;
	// Calc new forward, right, and up
	m_v3Forward = glm::normalize(m_v3Target - m_v3View);
	m_v3Right = glm::normalize(glm::cross(m_v3Forward, REAXISY));
	SetUp(glm::cross(m_v3Right, m_v3Forward));
	// Update view
	m_m4ViewMatrix = glm::lookAt(m_v3View, m_v3Target, m_v3Up);
}

// Set the up direction of the camera
void MyCameraClass::SetUp(vector3 v3Up) {
	m_v3Up = v3Up;
}

// Move the camera forward along its z axis
void MyCameraClass::MoveForward(float fIncrement) {
	m_v3Pos -= fIncrement * m_v3Forward;
}

// Move the camera sideways along its x axis
void MyCameraClass::MoveSideWays(float fIncrement) {
	m_v3Pos -= fIncrement * m_v3Right;
}

// Move the camera vertically along its y axis
void MyCameraClass::MoveVertical(float fIncrement) {
	m_v3Pos -= fIncrement * m_v3Up;
}
void MyCameraClass::Reset()
{
	// Reset to initial view
	m_v3Forward = m_v3ForwardInit;
	m_v3Right = m_v3RightInit;
	m_v3Up = m_v3UpInit;
	m_v3Target = vector3(0.f, 0.f, 0.f);
	SetTarget(m_v3Target);
}
void MyCameraClass::CalcViewMatrix()
{
	// Reset the camera
	Reset();

	// Calc raw, pitch, and roll quats.
	glm::quat qPitch = glm::quat(glm::radians(m_fPitch) * m_v3Right);
	glm::quat qYaw = glm::quat(glm::radians(m_fYaw) * m_v3Up);
	glm::quat qRoll = glm::quat(glm::radians(m_fRoll) * m_v3Forward);

	//Calc orientation and m4Rotation using these values.
	orientation = qPitch * (qYaw * qRoll);

	matrix4 m4Rotation = ToMatrix4(orientation);

	// Update forward up and right vectors
	m_v3Forward = m_v3Forward * orientation;
	m_v3Up = m_v3Up * orientation;
	m_v3Right = m_v3Right * orientation;


	// Calculate view matrix
	m_m4ViewMatrix =
		m4Rotation * 
		glm::translate(m_m4ViewMatrix, (m_v3Pos));
		

}
// Change orientation of camera around its x axis
void MyCameraClass::ChangePitch(float fIncrement) {
	// Right
	m_fPitch -= fIncrement;
}

// Change orientation of camera around its z axis
void MyCameraClass::ChangeRoll(float fIncrement) {
	// Forward
	m_fRoll -= fIncrement;
}
// Change orientation of camera around its y axis
void MyCameraClass::ChangeYaw(float fIncrement) {
	// Up
	m_fYaw -= fIncrement;
}