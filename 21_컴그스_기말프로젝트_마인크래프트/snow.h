#include "Mesh.h"
#include "Cube.h"

static float transTime = 0.0f; 

class Snow : public Cube
{
public:
	Snow(Position pos, Dimension dim) :Cube(pos, dim, L"snow.png")
	{

	}

	void update() override
	{
		this->lightning();
		
		//this->scaling(1.0f, 10.0f, 1.0f);
		this->translate(0.0f, transTime, 1.0f);

		devcon->UpdateSubresource(pCBuffer, 0, 0, this->move(matTrans, matView, matProjection), 0, 0);
		this->render();
	}
};
/*
 void update() override
	{
		static FLOAT Time; Time += 0.001f;
		for (int i = 0; i < Meshes.size(); i++)
		{
			Meshes[i]->scaling(0.0f,Time,0.0f);
			Meshes[i]->fixMove = ;
		}

		MeshClass::update();
	}
*/