#pragma once
#include "Mesh.h"
#include "Cube.h"

class SnowManHat : public Cube
{
public:
    SnowManHat(Position pos, Dimension dim) :Cube(pos, dim, L"hat.png")
	{



	}

    void update() override
    {
        this->lightning();
        this->translate(0, 0, 5.0f);

        devcon->UpdateSubresource(pCBuffer, 0, 0, this->move(matTrans, matView, matProjection), 0, 0);
        this->render();
    }
};

