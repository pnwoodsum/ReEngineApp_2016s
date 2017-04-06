#pragma once

#include "RE/ReEng.h"

class MyCameraClass
{

public:
	float m_fFoV;
	
	float m_fYaw;
	float m_fPitch;
	float m_fRoll;

	vector3 m_v3View; // Pos of camera in , world
	vector3 m_v3Up; // Unit vector up , local
	vector3 m_v3Forward; // Unit vector forward , local
	vector3 m_v3Right; // Unit vector right , local
	vector3 m_v3Target; // Pos of target , world

	matrix4 m_m4ViewMatrix = IDENTITY_M4;
	matrix4 m_m4ProjMatrix = IDENTITY_M4;
	matrix4 m_m4OrthoMatrix = IDENTITY_M4;

	MyCameraClass();
	MyCameraClass(vector3 position, vector3 target);

	// Get the camera's view matrix
	matrix4 GetView(void);

	// Get the camera's projection matrix
	matrix4 GetProject(bool bOrthographic);

	// Set the position of the camera
	void SetPosition(vector3 v3Position);

	// Set the direction the camera is facing
	void SetTarget(vector3 v3Target);

	// Set the up direction of the camera
	void SetUp(vector3 v3Up);

	// Move the camera forward along its z axis
	void MoveForward(float fIncrement);

	// Move the camera sideways along its x axis
	void MoveSideWays(float fIncrement);

	// Move the camera vertically along its y axis
	void MoveVertical(float fIncrement);

	// Change orientation of camera around its x axis
	void ChangePitch(float fIncrement);

	// Change orientation of camera around its z axis
	void ChangeRoll(float fIncrement);

	// Change orientation of camera around its y axis
	void ChangeYaw(float fIncrement);

private:


};
