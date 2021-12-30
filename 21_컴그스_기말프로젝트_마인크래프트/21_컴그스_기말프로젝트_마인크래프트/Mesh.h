#pragma once
#include <iostream>
#include<vector>
#include <d3dx11.h>
#include <d3dx10.h>
#include <D3DX10math.h>

using namespace std;

// a struct to define a single vertex
struct VERTEX { FLOAT X, Y, Z; D3DXVECTOR3 Normal; FLOAT U, V; };

enum class ANGLE {
    X,
    Y,
    Z
};

struct Position { FLOAT x, y, z; }; //shape position
typedef Position Dimension;
struct Color { D3DXCOLOR  col1, col2, col3; };

static ID3D11Device* dev;                     // the pointer to our Direct3D device interface
static ID3D11DeviceContext* devcon;           // the pointer to our Direct3D device context
static ID3D11Buffer* pCBuffer;                // the pointer to the constant buffer
static D3DXMATRIX matView, matProjection;

// a struct to define the constant buffer
struct CBUFFER
{
    D3DXMATRIX Final;
    D3DXMATRIX Rotation;
    D3DXVECTOR4 LightVector;
    D3DXCOLOR LightColor;
    D3DXCOLOR AmbientColor;
};

class Mesh
{
private:
    
    
protected:
    D3DXMATRIX matTrans, matScale, matRotate;
    Position pos;
    ID3D11Buffer* pIBuffer;                // the pointer to the index buffer
    ID3D11Buffer* pVBuffer;
    CBUFFER cBuffer;
    Mesh* mesh;
public:
    Mesh(Position pos) :pos(pos)
    {

    }

    virtual void CreateMesh() = 0 {}

    virtual void render()= 0 {}

    virtual void lightning()= 0 {}

    virtual void update() = 0 {}

    D3DXMATRIX getMatTrans() { return matTrans; }
    D3DXMATRIX getMatScale() { return matScale; }
    D3DXMATRIX getMatRotate() { return matRotate; }
    
    void translate(FLOAT sx, FLOAT sy, FLOAT sz)
    {
        D3DXMatrixTranslation(&matTrans, sx, sy, sz);
    }

    void rotation(ANGLE type, FLOAT angle)
    {
        switch (type)
        {
        case ANGLE::X:
            D3DXMatrixRotationX(&matRotate, angle);
            break;
        case ANGLE::Y:
            D3DXMatrixRotationY(&matRotate, angle);
            break;
        case ANGLE::Z:
            D3DXMatrixRotationZ(&matRotate, angle);
            break;
        }
    }

    void scaling(FLOAT sx, FLOAT sy, FLOAT sz)
    {
        D3DXMatrixScaling(&matScale, sx, sy, sz);
    }

    D3DXMATRIX* move(D3DXMATRIX matView, D3DXMATRIX matProjection)
    {
        cBuffer.Final = matView * matProjection;

        return &cBuffer.Final;
    }

    D3DXMATRIX* move(D3DXMATRIX mat1, D3DXMATRIX matView, D3DXMATRIX matProjection)
    {
        cBuffer.Final = mat1 * matView * matProjection;

        return &cBuffer.Final;
    }

    D3DXMATRIX* move(D3DXMATRIX mat1, D3DXMATRIX mat2, D3DXMATRIX matView, D3DXMATRIX matProjection)
    {
        cBuffer.Final = (mat1 * mat2) * matView * matProjection;

        return &cBuffer.Final;
    }

    D3DXMATRIX* move(D3DXMATRIX mat1, D3DXMATRIX mat2, D3DXMATRIX mat3, D3DXMATRIX matView, D3DXMATRIX matProjection)
    {
        cBuffer.Final = (mat1 * mat2 * mat3) * matView * matProjection;

        return &cBuffer.Final;
    }
};

