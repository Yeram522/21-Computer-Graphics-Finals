#pragma once
#include "Mesh.h"
#include "Cube.h"

class SnowMan :public Cube
{
public:
	SnowMan(Position pos, Dimension dim) :Cube(pos, dim, L"snowmanface.png")
	{
	
	
	
	}

    void update() override
    {
        this->lightning();
        this->translate(0, 1, 5.0f);
        this->rotation(ANGLE::Z, -80.0f);
        this->rotation(ANGLE::X, 90.0f);

        devcon->UpdateSubresource(pCBuffer, 0, 0, this->move(  matRotate, matTrans, matView, matProjection), 0, 0);
        this->render();
    }
};

