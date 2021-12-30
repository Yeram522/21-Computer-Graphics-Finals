#include "Mesh.h"
#include "Cube.h"

class SnowManBody : public Cube
{
public:
	SnowManBody(Position pos, Dimension dim) :Cube(pos, dim, L"snow.png")
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
