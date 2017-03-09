#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	static DWORD timerSinceBoot = GetTickCount(); // Timer since the computer was booted up
	DWORD timerSinceStart = GetTickCount() - timerSinceBoot; // Current time since application started (in milliseconds)

	// Variables to hold the time from 5 cycles ago and the change in time after 5 cycles
	static float fPreviousTimer5 = 0.0f;
	static float fDeltaTime5 = 0.0f;

	static float fPreviousTimer = 0.0f; // Keeping track of the time on the previous cycle (to find delta time)
	static float fTimer = 0.0f; // Initializing fTimer (to properly initialize fPreviousTimer)

	// Find the change in time over the last 5 cycles (to calculate average speed)
	static int iSpeed = 0;
	iSpeed++;
	if (iSpeed >= 5) {
		fDeltaTime5 = fTimer - fPreviousTimer5;
		fPreviousTimer5 = fTimer;
	}
	
	fPreviousTimer = fTimer;
	fTimer = timerSinceStart / 1000.0f; // Convert time to seconds

	float fDeltaTime = fTimer - fPreviousTimer; // The time between cycles is found from the difference of the current and previous timers
	static float fPercentageTracker = 0.0f; // Initialized variable to keep track of the time between each set of points
	fPercentageTracker += fDeltaTime; // Delta time is added to the tracker

	

	m_pMeshMngr->PrintLine(""); // Print an empty line
	m_pMeshMngr->PrintLine("Total Time: " + std::to_string(fTimer), RECORNFLOWERBLUE); // Print the timer

	// Vector3 array to hold the 11 points given in the assignment
	static vector3 v3aLocations[11] = {
		vector3(-4.0f, -2.0f, 5.0f),
		vector3(1.0f, -2.0f, 5.0f),
		vector3(-3.0f, -1.0f, 3.0f),
		vector3(2.0f, -1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f, -3.0f),
		vector3(4.0f, 1.0f, -3.0f),
		vector3(0.0f, 2.0f, -5.0f),
		vector3(5.0f, 2.0f, -5.0f),
		vector3(1.0f, 3.0f, -5.0f)
	};

	// Loop through the array to place spheres at the points given
	for (int i = 0; i < 11; i++) {
		m_pMeshMngr->AddSphereToRenderList(glm::translate(v3aLocations[i]) * glm::scale(vector3(0.1)), RERED, SOLID);
	}

	static int iStart = 0; // Inital start point
	static int iEnd = 1; // Initial end point
	
	float fDistance = glm::distance(v3aLocations[iStart], v3aLocations[iEnd]); // Find the distance between the two current points

	float percentage = MapValue(fPercentageTracker, 0.0f, fDistance/4, 0.0f, 1.0f); // Map the time it will take to move between based on the distance between them from 0 to 1

	// The start and end points change when the percent reaches 1
	if (percentage > 1.0f) {
		iStart++;
		iEnd++;
		// The start and end points each change to 0 when they reach the last index.
		if (iStart > 10) {
			iStart = 0;
		}
		if (iEnd > 10) {
			iEnd = 0;
		}
		
		// Reset the percentage and fPercentageTracker variables back to zero
		percentage = 0.0f;
		fPercentageTracker = 0.0f;
	}

	
	// Variables for previous and current positions
	static vector3 v3Previous = vector3(0.0f, 0.0f, 0.0f);
	static vector3 v3Current = v3Previous;

	// Calculate the average speed of the past 5 cycles
	static float fDeltaDistance = 0.0f;
	static float fCurrentSpeed = 0.0f;
	if (iSpeed >= 5) {
		fDeltaDistance = glm::distance(v3Previous, v3Current);
		v3Previous = v3Current;
		fCurrentSpeed = (fDeltaDistance / fDeltaTime5);
		iSpeed = 0;
	}

	v3Current = glm::lerp(v3aLocations[iStart], v3aLocations[iEnd], percentage); // Finds the current position using LERP based on the percentage found with the map.

	m_pMeshMngr->PrintLine("Translate Time: " + std::to_string(fPercentageTracker), RECORNFLOWERBLUE); // Print the timer for the current translation
	m_pMeshMngr->PrintLine("Percentage: " + std::to_string(percentage),RECORNFLOWERBLUE); // Print the percentage of the way it is through current translation
	m_pMeshMngr->PrintLine("Speed: " + std::to_string(fCurrentSpeed) + " Units/Second", RECORNFLOWERBLUE); // Print the speed of WallEye based on delta d and delta t between 5 cycles
	m_pMeshMngr->PrintLine("Start Index: " + std::to_string(iStart), RECORNFLOWERBLUE); // Print the starting index
	m_pMeshMngr->PrintLine("End Index: " + std::to_string(iEnd), RECORNFLOWERBLUE); // Print the end index
	matrix4 m4WallEye = glm::translate(v3Current);
	m_pMeshMngr->SetModelMatrix(m4WallEye, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	// Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	// A square (cube) to make it easier to read if the default camera view
	m_pMeshMngr->AddCubeToRenderList(glm::translate(vector3(-6.5f, 3.7f, 0.0f)) * glm::scale(vector3(8.5f, 3.3f, 1.0f)), REBLACK, SOLID);
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}