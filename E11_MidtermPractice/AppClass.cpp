#include "AppClass.h"
void AppClass::Update(void)
{
#pragma region DOES NOT NEED CHANGES
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma endregion
#pragma region FEEL FREE TO USE THIS CLOCK
	//Calculate delta and total times
	static double dTotalTime = 0.0; //Total time of the simulation
	double dDeltaTime = m_pSystem->LapClock(); //time difference between function calls
	dTotalTime += dDeltaTime; //Incrementing the time differences 
#pragma endregion

#pragma region YOUR CODE GOES HERE
	m_m4Steve = glm::mat4(1.0f); // same as m_m4Steve = IDENTITY_M4; setting the identity to steve

	// Used to reset the percent map every 5 seconds
	static double dPercentTracker = 0;
	// Keep track of the percentage of the way through each 5 seconds it is
	static float fPercentage = 0.0f;
	// Used to keep track of whether moving in or out
	static bool bOut = true;
	// Keep track of the models current position
	static vector3 v3CurrentPosition = vector3(0.0f, 0.0f, 0.0f);
	// Keep track of the models current rotation in degrees
	static float fRotation = 0.0f;

	// Increment the percent tracker
	dPercentTracker += dDeltaTime;

	// Change the bOut bool and reset the percent tracker every 5 seconds
	if (dPercentTracker > 5.0f) {
		bOut = !bOut;
		dPercentTracker = 0.0f;
	}

	// Calculate the current percentage by mapping the percent tracker from 0 to 1
	fPercentage = MapValue((float)dPercentTracker, 0.0f, 5.0f, 0.0f, 1.0f);

	// Lerp from the center to the current distance if moving out
	if (bOut) {
		v3CurrentPosition = glm::lerp(vector3(0.0f, 0.0f, 0.0f), vector3(0.0f, 5.0f, 0.0f), fPercentage);
	}
	// Lerp from the current distance to the center if moving in
	else {
		v3CurrentPosition = glm::lerp(vector3(0.0f, 5.0f, 0.0f), vector3(0.0f, 0.0f, 0.0f), fPercentage);
	}

	// Lerp from 0 to 360 to find the current angle of rotation
	fRotation = glm::lerp(0.0f, 360.0f, fPercentage);

	// Rotate and translate the model
	m_m4Steve = glm::rotate(m_m4Steve, fRotation, vector3(0.0f, 0.0f, 1.0f)) * glm::translate(v3CurrentPosition);
#pragma endregion

#pragma region DOES NOT NEED CHANGES
	//Set the model matrix
	m_pMeshMngr->SetModelMatrix(m_m4Steve, "Steve");
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine("\n" + m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Seconds:");
	m_pMeshMngr->PrintLine(std::to_string(dTotalTime), RERED);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}
