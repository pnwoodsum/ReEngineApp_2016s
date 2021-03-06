#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
<<<<<<< HEAD
	super::InitWindow("BoundingSpheres"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
	m_pSystem->SetWindowResolution(RESOLUTIONS::C_1280x720_16x9_HD);
	//m_pSystem->SetWindowFullscreen(); //Sets the window to be fullscreen
	//m_pSystem->SetWindowBorderless(true); //Sets the window to not have borders
=======
	super::InitWindow("Bounding Spheres DEMO");
>>>>>>> refs/remotes/labigm/master
}

void AppClass::InitVariables(void)
{
<<<<<<< HEAD
	//Reset the selection to -1, -1
	m_selection = std::pair<int, int>(-1, -1);
=======
>>>>>>> refs/remotes/labigm/master
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie");
<<<<<<< HEAD

=======
>>>>>>> refs/remotes/labigm/master
	m_pBS0 = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Zombie"));
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

	//Call the arcball method
	ArcBall();
<<<<<<< HEAD
	
	matrix4 m4Sphere = glm::translate(m_pBS0->m_v3Center) *
		glm::scale(vector3(m_pBS0->m_fRadius*2));
	m_pMeshMngr->AddSphereToRenderList(m4Sphere, RERED, WIRE);

	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), 0);
	
	m_pMeshMngr->SetModelMatrix(glm::translate(vector3(1.0f, 0.0f, 0.0f)), "Zombie");

=======
	matrix4 m4Sphere = glm::translate(m_pBS0->m_v3Center) * 
		glm::scale(vector3(m_pBS0->m_fRadius) * 2.0f);
	m_pMeshMngr->AddSphereToRenderList(m4Sphere, RERED, WIRE);
	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(glm::translate(vector3(1, 0, 0)), "Zombie");
	
>>>>>>> refs/remotes/labigm/master
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

<<<<<<< HEAD
	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
	m_pMeshMngr->Print("Radius:");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_fRadius), RERED);
	m_pMeshMngr->Print("Center: (");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.x), RERED);
	m_pMeshMngr->Print(", ");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.y), RERED);
	m_pMeshMngr->Print(", ");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.z), RERED);
	m_pMeshMngr->Print(")");

=======
	m_pMeshMngr->Print("Radius: ");
	m_pMeshMngr->PrintLine(std::to_string(m_pBS0->m_fRadius), RERED);
	m_pMeshMngr->Print("Center: (");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.x), RERED);
	m_pMeshMngr->Print(" , ");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.y), RERED);
	m_pMeshMngr->Print(" , ");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.z), RERED);
	m_pMeshMngr->PrintLine(")");

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
>>>>>>> refs/remotes/labigm/master
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
<<<<<<< HEAD
	//m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
=======
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
>>>>>>> refs/remotes/labigm/master
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pBS0);
<<<<<<< HEAD
	if (m_pBS0 != nullptr) {
=======

	if (m_pBS0 != nullptr)
	{
>>>>>>> refs/remotes/labigm/master
		delete m_pBS0;
		m_pBS0 = nullptr;
	}
	super::Release(); //release the memory of the inherited fields
}