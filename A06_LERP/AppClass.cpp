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

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "Creeper");

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
	static DWORD timerSinceBoot = GetTickCount(); //Timer since the computer was botted
	DWORD timerSinceStart = GetTickCount() - timerSinceBoot; // Current time

	static float fPreviousTimer = 0.0f;
	static float fTimer = 0.0f;

	fPreviousTimer = fTimer;
	fTimer = timerSinceStart / 1000.0f; // Convert time to seconds

	static float fPercentageTracker = 0.0f;
	fPercentageTracker += (fTimer - fPreviousTimer);

	m_pMeshMngr->PrintLine(""); // Print an empty line
	m_pMeshMngr->PrintLine("Total Time: " + std::to_string(fTimer)); // Print the timer

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

	for (int i = 0; i < 11; i++) {
		m_pMeshMngr->AddSphereToRenderList(glm::translate(v3aLocations[i]) * glm::scale(vector3(0.1)), RERED, SOLID);
	}

	static int iStart = 0;
	static int iEnd = 1;
	
	float fDistance = glm::distance(v3aLocations[iStart], v3aLocations[iEnd]);

	float percentage = MapValue(fPercentageTracker, 0.0f, fDistance/4, 0.0f, 1.0f);

	if (percentage > 1.0f) {

		iStart++;
		iEnd++;
		if (iStart > 10) {
			iStart = 0;
		}
		if (iEnd > 10) {
			iEnd = 0;
		}
		
		percentage = 0.0f;
		fPercentageTracker = 0.0f;
	}

	vector3 v3Current = glm::lerp(v3aLocations[iStart], v3aLocations[iEnd], percentage);

	m_pMeshMngr->PrintLine("Translate Time: " + std::to_string(fPercentageTracker)); // Print the timer
	m_pMeshMngr->PrintLine("Percentage: " + std::to_string(percentage)); // Print the timer

	matrix4 m4Creeper = glm::translate(v3Current);
	m_pMeshMngr->SetModelMatrix(m4Creeper, "Creeper");
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
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}