#include "MyCameraClass.h"

class MyCameraClass {

public:
	MyCameraClass();

	matrix4 GetView(void);

	matrix4 GetProject(bool bOrthographic);

	void SetPosition(vector3 v3Position);

	void SetTarget(vector3 v3Target);

	void SetUp(vector3 v3Up);

	void MoveForward(float fIncrement);

	void MoveSideWays(float fIncrement);

	void MoveVertical(float fIncrement);

	void ChangePitch(float fIncrement);

	void ChangeRoll(float fIncrement);

	void ChangeYaw(float fIncrement);

private:


};