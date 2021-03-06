#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Sandbox"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
	m_pSystem->SetWindowResolution(RESOLUTIONS::C_1280x720_16x9_HD);
	//m_pSystem->SetWindowFullscreen(); //Sets the window to be fullscreen
	//m_pSystem->SetWindowBorderless(true); //Sets the window to not have borders
}

void AppClass::InitVariables(void)
{

	//Reset the selection to -1, -1
	m_selection = std::pair<int, int>(-1, -1);

	m_pCamera = new MyCameraClass();

	m_pRedCube = new PrimitiveClass();
	m_pRedCube->GenerateCube(10.0f, RERED);

	m_pBlueCube = new PrimitiveClass();
	m_pBlueCube->GenerateCube(10.0f, REBLUE);

	m_pGreenCube = new PrimitiveClass();
	m_pGreenCube->GenerateCube(10.0f, REGREEN);

	m_pYellowCube = new PrimitiveClass();
	m_pYellowCube->GenerateCube(10.0f, REYELLOW);

	m_pOrangeCube = new PrimitiveClass();
	m_pOrangeCube->GenerateCube(10.0f, REORANGE);

	m_pFloor = new PrimitiveClass();
	m_pFloor->GenerateCube(100.0f, REPURPLE);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		MyCameraRotation();

	//Call the arcball method
	//MyArcBall();

	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), 0);

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Calc view matrix
	m_pCamera->CalcViewMatrix();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	//m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());

	matrix4 projMat = m_pCamera->GetProject(false);
	matrix4 viewMat = m_pCamera->GetView();
	

	// Render cube
	m_pRedCube->Render(projMat, viewMat, IDENTITY_M4);
	m_pBlueCube->Render(projMat, viewMat, glm::translate(IDENTITY_M4, vector3(-20.0f, 0.0f, 0.0f)));
	m_pGreenCube->Render(projMat, viewMat, glm::translate(IDENTITY_M4, vector3(-20.0f, -20.0f, -20.0f)));
	m_pYellowCube->Render(projMat, viewMat, glm::translate(IDENTITY_M4, vector3(20.0f, 20.0f, 0.0f)));
	m_pOrangeCube->Render(projMat, viewMat, glm::translate(IDENTITY_M4, vector3(0.0f, -20.0f, 20.0f)));
	m_pFloor->Render(projMat, viewMat, glm::translate(IDENTITY_M4, vector3(0.0f, -90.0f, 0.0f)));

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}
void AppClass::MyCameraRotation()
{
	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

								//Initialize the position of the pointer to the middle of the screen
	CenterX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	CenterY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	float a_fSpeed = 0.005f;

	//Calculate the position of the mouse and store it
	POINT pt;
	GetCursorPos(&pt);
	MouseX = pt.x;
	MouseY = pt.y;

	//Calculate the difference in view with the angle
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fDeltaMouse = 0.0f;
	if (MouseX < CenterX)
	{
		fDeltaMouse = static_cast<float>(CenterX - MouseX);
		fAngleY += fDeltaMouse * a_fSpeed;
	}
	else if (MouseX > CenterX)
	{
		fDeltaMouse = static_cast<float>(MouseX - CenterX);
		fAngleY -= fDeltaMouse * a_fSpeed;
	}

	if (MouseY < CenterY)
	{
		fDeltaMouse = static_cast<float>(CenterY - MouseY);
		fAngleX -= fDeltaMouse * a_fSpeed;
	}
	else if (MouseY > CenterY)
	{
		fDeltaMouse = static_cast<float>(MouseY - CenterY);
		fAngleX += fDeltaMouse * a_fSpeed;
	}
	//Change the Yaw and the Pitch of the camera
	m_pCamera->ChangeYaw(fAngleY * 3.0f);
	m_pCamera->ChangePitch(-fAngleX * 3.0f);
	SetCursorPos(CenterX, CenterY);//Position the mouse in the center
}