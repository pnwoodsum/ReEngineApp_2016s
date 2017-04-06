#include "MyCameraClass.h"

MyCameraClass::MyCameraClass() {
	m_fYaw = 0.f;
	m_fPitch = 0.f;
	m_fRoll = 0.f;

	m_v3Up = REAXISY;
	m_v3Forward = REAXISZ;
	m_v3Right = REAXISX;

	float m_fFov = 45.f;

	m_v3View = vector3(10.f, -10.f, 10.f);
	m_v3Target = vector3(0.f, 0.f, 0.f);

	SetTarget(m_v3Target);
	m_m4ProjMatrix = glm::perspective(m_fFoV, 4.f / 3.f, 0.1f, 100.f);
	m_m4OrthoMatrix = glm::ortho(0.f, 400.0f, 0.f, 300.0f, 0.1f, 100.0f);
}

MyCameraClass::MyCameraClass(vector3 position, vector3 target, vector3 up) {
	m_m4ViewMatrix = glm::translate(position);
	m_m4ProjMatrix = glm::perspective(m_fFoV, 4.0f / 3.0f, 0.1f, 100.0f);
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
	m_v3View = v3Position;
	SetTarget(m_v3Target);
}

// Set the direction the camera is facing
void MyCameraClass::SetTarget(vector3 v3Target) {

	matrix4 Result;
	m_v3Up = REAXISY;
	vector3 v3Dir = glm::normalize(v3Target - m_v3View);
	m_v3Right = glm::normalize(glm::cross(v3Dir, m_v3Up));
	m_v3Up = glm::cross(m_v3Right, v3Dir);

	Result[0][0] = m_v3Right.x;
	Result[1][0] = m_v3Right.y;
	Result[2][0] = m_v3Right.z;
	Result[0][1] = m_v3Up.x;
	Result[1][1] = m_v3Up.y;
	Result[2][1] = m_v3Up.z;
	Result[0][2] = -v3Dir.x;
	Result[1][2] = -v3Dir.y;
	Result[2][2] = -v3Dir.z;
	Result[3][0] = -glm::dot(m_v3Right, m_v3View);
	Result[3][1] = -glm::dot(m_v3Up, m_v3View);
	Result[3][2] = glm::dot(v3Dir, m_v3View);

	vector3 v3YDir = vector3(v3Dir.x, 0.f, v3Dir.z);
	m_fYaw = glm::acos(
		(glm::dot(m_v3Forward, v3YDir))
		/ (glm::length(m_v3Forward) * glm::length(v3YDir))
	);
	if (v3Dir.x == 0.f && v3Dir.z == 0.f)
	{
		m_fYaw = 0.f;
		v3YDir = m_v3Up;
	}

	m_fPitch = glm::acos(
		(glm::dot(v3YDir, v3Dir))
		/ (glm::length(v3YDir) * glm::length(v3Dir))
	);
	m_v3Forward = v3Dir;

	quaternion qYaw = glm::angleAxis(glm::degrees(m_fYaw), m_v3Up);
	quaternion qPitch = glm::angleAxis(glm::degrees(m_fPitch), m_v3Right);
	quaternion qRoll = glm::angleAxis(glm::degrees(m_fRoll), m_v3Forward);

	matrix4 mYaw = ToMatrix4(qYaw);
	matrix4 mPitch = ToMatrix4(qPitch);
	matrix4 mRoll = ToMatrix4(qRoll);

	matrix4 mOrientation = mYaw * mPitch * mRoll;

	matrix4 mTranslation = glm::translate(m_v3View);

	m_m4ViewMatrix = Result;
}

// Set the up direction of the camera
void MyCameraClass::SetUp(vector3 v3Up) {

}

// Move the camera forward along its z axis
void MyCameraClass::MoveForward(float fIncrement) {
	m_v3View.x += fIncrement;
	SetPosition(m_v3View);
}

// Move the camera sideways along its x axis
void MyCameraClass::MoveSideWays(float fIncrement) {

}

// Move the camera vertically along its y axis
void MyCameraClass::MoveVertical(float fIncrement) {

}

// Change orientation of camera around its x axis
void MyCameraClass::ChangePitch(float fIncrement) {
	// Right
}

// Change orientation of camera around its z axis
void MyCameraClass::ChangeRoll(float fIncrement) {
	// Forward
}
// Change orientation of camera around its y axis
void MyCameraClass::ChangeYaw(float fIncrement) {
	// Up

}