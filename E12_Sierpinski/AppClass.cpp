#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("E12 - Instance Rendering");
	m_v4ClearColor = vector4(REBLACK, 1.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pSierpinski = new MyMesh();

	// These values can be changed
	int nRecursions = 2; // Number of recursions (1 recursion = 1 triangle, 2 = 1 tri-force, etc...)
	float fTotalSideLength = 10.0f; // The side length of the entire triangle.
	//

	if (nRecursions <= 0) {
		nRecursions = 1;
	}

	float fTotalHeight = sqrt(pow(fTotalSideLength, 2) - pow(fTotalSideLength / 2, 2));
	float fSideLength = fTotalSideLength / pow(2, nRecursions-1);
	float fHeight = fTotalHeight / pow(2, nRecursions - 1);

	// Creating triangle
	m_pSierpinski->AddVertexPosition(vector3(-fSideLength/2, (fTotalHeight - fHeight)-(fTotalHeight/2), 0.0f));
	m_pSierpinski->AddVertexColor(REGREEN);
	m_pSierpinski->AddVertexPosition(vector3(fSideLength/2, (fTotalHeight - fHeight)-(fTotalHeight/2), 0.0f));
	m_pSierpinski->AddVertexColor(RERED);
	m_pSierpinski->AddVertexPosition(vector3(0.0f, fTotalHeight-(fTotalHeight/2), 0.0f));
	m_pSierpinski->AddVertexColor(REBLUE);

	//Compiling the mesh
	m_pSierpinski->CompileOpenGL3X();

	if (nRecursions == 1) {
		return;
	}

	int nDraws = pow(3, nRecursions - 2);
	int nTris = pow(3, nRecursions - 1);

	
}

//void AppClass::CreateTriangles() {
//	for (int nObject = 0; nObject < m_nObjects; nObject++)
//	{
//		const float* m4MVP = glm::value_ptr(
//			glm::translate(vector3(0.01f * -nObject, 0.0f, 1.0f * -nObject)) *
//			glm::rotate(IDENTITY_M4, nObject * 5.0f, REAXISZ)
//		);
//		memcpy(&m_fMatrixArray[nObject * 16], m4MVP, 16 * sizeof(float));
//	}
//}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	ClearScreen(); // Clear the screen
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	
	m_pSierpinski->Render(m4Projection, m4View, IDENTITY_M4);

	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pSierpinski != nullptr)
	{
		delete m_pSierpinski;
		m_pSierpinski = nullptr;
	}
	super::Release();
}