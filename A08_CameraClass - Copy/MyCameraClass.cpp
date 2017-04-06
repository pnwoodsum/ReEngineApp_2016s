#include "MyCameraClass.h"

MyCameraClass::MyCameraClass() {
	m_fYaw = 0.f;
	m_fPitch = 0.f;
	m_fRoll = 0.f;

	m_v3Up = REAXISY;
	m_v3Forward = REAXISZ;
	m_v3Right = REAXISX;

	float m_fFov = 45.f;

	m_v3View = vector3(0.f, 0.f, 10.f);
	m_v3Target = vector3(0.f, 0.f, 0.f);

	SetTarget(m_v3Target);
	m_m4ProjMatrix = glm::perspective(m_fFoV, 4.f / 3.f, 0.1f, 100.f);
	m_m4OrthoMatrix = glm::ortho(0.f, 400.0f, 0.f, 300.0f, 0.1f, 100.0f);
}

MyCameraClass::MyCameraClass(vector3 position, vector3 target) {
	m_fYaw = 0.f;
	m_fPitch = 0.f;
	m_fRoll = 0.f;

	m_v3Up = REAXISY;
	m_v3Forward = REAXISZ;
	m_v3Right = REAXISX;

	float m_fFov = 45.f;

	m_v3View = position;
		m_v3Target = target;

	SetTarget(m_v3Target);
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
	m_m4ViewMatrix = glm::translate(m_m4ViewMatrix, v3Position - m_v3View);
	m_v3View = v3Position;
	// SetTarget(m_v3Target);
}

// Set the direction the camera is facing
void MyCameraClass::SetTarget(vector3 v3Target) {
	m_v3Target = v3Target;

	m_v3Forward = glm::normalize(v3Target - m_v3View);
	m_v3Right = glm::normalize(glm::cross(m_v3Forward, REAXISY));
	SetUp(glm::normalize(glm::cross(m_v3Right, m_v3Forward)));

	m_m4ViewMatrix = glm::lookAt(m_v3View, m_v3Target, m_v3Up);

	m_fPitch = glm::degrees(glm::acos(glm::dot(m_v3Forward, REAXISZ)));
	m_fRoll = glm::degrees(glm::acos(glm::dot(m_v3Up, REAXISY)));
	m_fYaw = glm::degrees(glm::acos(glm::dot(m_v3Right, REAXISX)));
}

// Set the up direction of the camera
void MyCameraClass::SetUp(vector3 v3Up) {
	m_v3Up = v3Up;
}

// Move the camera forward along its z axis
void MyCameraClass::MoveForward(float fIncrement) {

	SetPosition(m_v3View - (m_v3Forward * fIncrement));
}

// Move the camera sideways along its x axis
void MyCameraClass::MoveSideWays(float fIncrement) {
	SetPosition(m_v3View - (m_v3Right * fIncrement));
}

// Move the camera vertically along its y axis
void MyCameraClass::MoveVertical(float fIncrement) {
	SetPosition(m_v3View - (m_v3Up * fIncrement));
}


// Change orientation of camera around its x axis
void MyCameraClass::ChangePitch(float fIncrement) {
	// Right
	m_fPitch -= fIncrement;

	//glm::quat qRot = glm::angleAxis(fIncrement, m_v3Right);
	glm::quat qPitch = glm::angleAxis(m_fPitch, REAXISX);
	glm::quat qRoll = glm::angleAxis(m_fRoll, REAXISZ);
	glm::quat qYaw = glm::angleAxis(m_fYaw, REAXISY);

	//matrix4 mRot = ToMatrix4(qRot);
	matrix4 mPitch = ToMatrix4(qPitch);
	matrix4 mRoll = ToMatrix4(qRoll);
	matrix4 mYaw = ToMatrix4(qYaw);

	m_v3Forward = glm::rotate(m_v3Forward, fIncrement, m_v3Right);
	SetUp(glm::cross(m_v3Forward, m_v3Right));

	//m_m4ViewMatrix = mRot * m_m4ViewMatrix;
	m_m4ViewMatrix = mPitch * mRoll * mYaw *glm::translate(m_v3View);
}

// Change orientation of camera around its z axis
void MyCameraClass::ChangeRoll(float fIncrement) {
	// Forward
	m_fRoll -= fIncrement;

	glm::quat qPitch = glm::angleAxis(m_fPitch, REAXISX);
	glm::quat qRoll = glm::angleAxis(m_fRoll, REAXISZ);
	glm::quat qYaw = glm::angleAxis(m_fYaw, REAXISY);

	matrix4 mPitch = ToMatrix4(qPitch);
	matrix4 mRoll = ToMatrix4(qRoll);
	matrix4 mYaw = ToMatrix4(qYaw);

	m_v3Right = glm::rotate(m_v3Right, fIncrement, m_v3Forward);
	SetUp(glm::cross(m_v3Forward, m_v3Right));

	m_m4ViewMatrix = mPitch * mRoll * mYaw *glm::translate(m_v3View);
}

// Change orientation of camera around its y axis
void MyCameraClass::ChangeYaw(float fIncrement) {
	// Up
	m_fYaw -= fIncrement;

	//glm::quat qRot = glm::angleAxis(fIncrement, m_v3Up);
	glm::quat qPitch = glm::angleAxis(m_fPitch, REAXISX);
	glm::quat qRoll = glm::angleAxis(m_fRoll, REAXISZ);
	glm::quat qYaw = glm::angleAxis(m_fYaw, REAXISY);

	//matrix4 mRot = ToMatrix4(qRot);
	matrix4 mPitch = ToMatrix4(qPitch);
	matrix4 mRoll = ToMatrix4(qRoll);
	matrix4 mYaw = ToMatrix4(qYaw);

	m_v3Forward = glm::rotate(m_v3Forward, fIncrement, m_v3Up);
	m_v3Right = glm::cross(m_v3Up, m_v3Forward);

	//m_m4ViewMatrix = mRot * m_m4ViewMatrix;
	m_m4ViewMatrix = mPitch * mYaw * mYaw * glm::translate(m_v3View);
}