#pragma once
#include "Mesh.h"
#include "Cube.h"

class diamondCube: public Cube
{
public:
	diamondCube(Position pos, Dimension dim):Cube(pos, dim, L"diamond.png")
	{

	}

    void update() override
    {
        this->lightning();
        static float Time = 0.0f; Time += 0.0003f;
        this->translate(1, 1, 2);
        this->rotation(ANGLE::Y, Time);
       
        devcon->UpdateSubresource(pCBuffer, 0, 0, this->move(matRotate, matTrans,matView, matProjection), 0, 0);
        this->render();
    }
};

