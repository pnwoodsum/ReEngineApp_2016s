#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - YOUR USER NAME GOES HERE"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");
	m_pMeshMngr->LoadModel("Planets\\02_Venus.obj", "Venus");

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double dRunTime = 0.0f;
	dRunTime += fCallTime;

	//Earth Orbit
	float fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; // Moon's orbit is 28 earth days, so half the time for half a route
	float fVenusHalfOrbTime = 112.5f * m_fDay; // Venus has an orbit of 225 days
	float fVenusHalfRevTime = 221.5f * m_fDay; // Venus has a revolution period of 243 days

	 //Create the quaternions for interpolation
	quaternion q1 = glm::angleAxis(0.0f, vector3(0.0f, 1.0f, 0.0f));
	quaternion q2 = glm::angleAxis(180.0f, vector3(0.0f, 1.0f, 0.0f));
	quaternion qEarthOrbit;
	quaternion qEarthRev;
	quaternion qMoon;
	quaternion qVenusOrbit;
	quaternion qVenusRev;
	 
	float fPercentageEarthOrbit = MapValue((float)dRunTime, 0.0f, fEarthHalfOrbTime, 0.0f, 1.0f); // map for earth orbit
	float fPercentageEarthRev = MapValue((float)dRunTime, 0.0f, fEarthHalfRevTime, 0.0f, 1.0f); // map for earth orbit
	float fPercentageMoon = MapValue((float)dRunTime, 0.0f, fMoonHalfOrbTime, 0.0f, 1.0f); // map for earth orbit
	float fPercentageVenusOrbit = MapValue((float)dRunTime, 0.0f, fVenusHalfOrbTime, 0.0f, 1.0f); // map for earth orbit
	float fPercentageVenusRev = MapValue((float)dRunTime, 0.0f, fVenusHalfRevTime, 0.0f, 1.0f); // map for earth orbit

	std::cout << fPercentageEarthRev << std::endl;

	qEarthOrbit = glm::mix(q1, q2, fPercentageEarthOrbit);
	qEarthRev = glm::mix(q1, q2, fPercentageEarthRev);
	qMoon = glm::mix(q1, q2, fPercentageMoon);
	qVenusOrbit = glm::mix(q1, q2, fPercentageVenusOrbit);
	qVenusRev = glm::mix(q1, q2, fPercentageVenusRev);

	// Update the sun's position (due to input)
	m_m4Sun = IDENTITY_M4;
	m_m4Sun = glm::translate(m_v3Sun);
	m_m4Sun = glm::scale(m_m4Sun, vector3(1.0f, 1.0f, 1.0f)*5.936f);

	m_m4Earth = IDENTITY_M4;
	m_m4Earth = glm::translate(m_v3Sun) *
		ToMatrix4(qEarthOrbit) *
		glm::translate(vector3(11.0f, 0.0f, 0.0f)) *
		ToMatrix4(qEarthRev) *
		glm::scale(m_m4Earth, vector3(1.0f, 1.0f, 1.0f)*0.524f);

	m_m4Moon = IDENTITY_M4;
	m_m4Moon = glm::translate(m_v3Sun) *
		ToMatrix4(qEarthOrbit) *
		glm::translate(vector3(11.0f, 0.0f, 0.0f)) *
		ToMatrix4(qMoon) *
		glm::translate(vector3(0.524f * 2.0f, 0.0f, 0.0f)) *
		glm::scale(m_m4Moon, vector3(1.0f, 1.0f, 1.0f)*(0.524f*0.25f));

	m_m4Venus = IDENTITY_M4;
	m_m4Venus = glm::translate(m_v3Sun) *
		ToMatrix4(qVenusOrbit) *
		glm::translate(vector3(5.926f + ((11.0f - 5.936f) * 0.72f), 0.0f, 0.0f)) *
		ToMatrix4(qVenusRev) *
		glm::scale(m_m4Venus, vector3(1.0f, 1.0f, 1.0f)*(0.524f*0.815f));

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m_m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m_m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m_m4Moon, "Moon");
	m_pMeshMngr->SetModelMatrix(m_m4Venus, "Venus");

	//Adds all loaded instances to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	static int nEarthOrbits = 0;
	static int nEarthRevolutions = 0;
	static int nMoonOrbits = 0;
	static int nVenusOrbits = 0;
	static int nVenusRevolutions = 0;

	static float fEarthOrbitTime = 0.0f;
	static float fEarthRevolutionTime = 0.0f;
	static float fMoonOrbitTime = 0.0f;
	static float fVenusOrbitTime = 0.0f;
	static float fVenusRevolutionTime = 0.0f;

	// Increment orbit and revolution data
	if ((float)dRunTime - fEarthOrbitTime >= 2.0f * fEarthHalfOrbTime){
		nEarthOrbits++;
		fEarthOrbitTime = (float)dRunTime;
	}
	if ((float)dRunTime - fEarthRevolutionTime >= 2.0f * fEarthHalfRevTime) {
		nEarthRevolutions++;
		fEarthRevolutionTime = (float)dRunTime;
	}
	if ((float)dRunTime - fMoonOrbitTime >= 2.0f * fMoonHalfOrbTime) {
		nMoonOrbits++;
		fMoonOrbitTime = (float)dRunTime;
	}
	if ((float)dRunTime - fVenusOrbitTime >= 2.0f * fVenusHalfOrbTime) {
		nVenusOrbits++;
		fVenusOrbitTime = (float)dRunTime;
	}
	if ((float)dRunTime - fVenusRevolutionTime >= 2.0f * fVenusHalfRevTime) {
		nVenusRevolutions++;
		fVenusRevolutionTime = (float)dRunTime;
	}

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(dRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

	m_pMeshMngr->Print("V_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nVenusOrbits));

	m_pMeshMngr->Print("V_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nVenusRevolutions));

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}