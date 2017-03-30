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

	// Can change these 2 values
	int m_nRecursions = 5; // Number of recursions (1 recursion = 1 triangle, 2 = 1 tri-force, etc...)
	float m_fTotalSideLength = 12.0f; // The side length of the entire triangle.
	//

	if (m_nRecursions <= 0) {
		m_nRecursions = 1;
	}

	float fTotalHeight = sqrt(pow(m_fTotalSideLength, 2) - pow(m_fTotalSideLength / 2, 2));
	float fSideLength = m_fTotalSideLength / pow(2, m_nRecursions - 1);
	float fHeight = fTotalHeight / pow(2, m_nRecursions - 1);

	// Creating triangle
	m_pSierpinski->AddVertexPosition(vector3(-fSideLength / 2, (fTotalHeight - fHeight) - (fTotalHeight / 2), 0.0f));
	m_pSierpinski->AddVertexColor(REGREEN);
	m_pSierpinski->AddVertexPosition(vector3(fSideLength / 2, (fTotalHeight - fHeight) - (fTotalHeight / 2), 0.0f));
	m_pSierpinski->AddVertexColor(RERED);
	m_pSierpinski->AddVertexPosition(vector3(0.0f, fTotalHeight - (fTotalHeight / 2), 0.0f));
	m_pSierpinski->AddVertexColor(REBLUE);

	//Compiling the mesh
	m_pSierpinski->CompileOpenGL3X();
	
	// Find total number of triangles based on number of recursions
	m_nTris = pow(3, m_nRecursions - 1);

	// see size of matrix array based on number of triangles
	m_fMatrixArray = new float[m_nTris * 16];

	if (m_nRecursions == 1) {
		const float* m4MVP = glm::value_ptr(glm::translate(vector3(0.0f, 0.0f, 0.0f)));

		memcpy(&m_fMatrixArray[0], m4MVP, 16 * sizeof(float));
	}
	else {
		CreateTriangles(0.0f, 0.0f, fSideLength, fHeight, m_fTotalSideLength, fTotalHeight, *m_fMatrixArray, m_nRecursions);
	}
}

// Recursively create sierpinski triangle based on recursions
void AppClass::CreateTriangles(float initX, float initY, float triLength, float triHeight, float totalTriLength, float totalTriHeight, float matArray, int numRecursions) {

	if (numRecursions > 2) {
		CreateTriangles(initX, initY, triLength, triHeight, totalTriLength/2, totalTriHeight/2, matArray, numRecursions - 1);
		CreateTriangles(initX - totalTriLength/4, initY - totalTriHeight/2, triLength, triHeight, totalTriLength / 2, totalTriHeight / 2, matArray, numRecursions - 1);
		CreateTriangles(initX + totalTriLength/4, initY - totalTriHeight/2, triLength, triHeight, totalTriLength / 2, totalTriHeight / 2, matArray, numRecursions - 1);
	}
	else {
		const float* m4MVP = glm::value_ptr(glm::translate(vector3(initX, initY, 0.0f)));
		memcpy(&m_fMatrixArray[m_nTriNum * 16], m4MVP, 16 * sizeof(float));
		m_nTriNum++;

		m4MVP = glm::value_ptr(glm::translate(vector3(initX - (triLength / 2), initY - triHeight, 0.0f)));
		memcpy(&m_fMatrixArray[m_nTriNum * 16], m4MVP, 16 * sizeof(float));
		m_nTriNum++;

		m4MVP = glm::value_ptr(glm::translate(vector3(initX + (triLength / 2), initY - triHeight, 0.0f)));
		memcpy(&m_fMatrixArray[m_nTriNum * 16], m4MVP, 16 * sizeof(float));
		m_nTriNum++;
	}
}

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
	
	m_pSierpinski->RenderList(m4Projection, m4View, m_fMatrixArray, m_nTris);//Rendering nObjects

	//m_pSierpinski->Render(m4Projection, m4View, IDENTITY_M4);

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