#pragma once
#include "Mesh.h"
#include <functional>

class Cube: public Mesh
{
private:
    VERTEX Vertices[24];
    Dimension dim;
    const LPCWSTR  pSrcFile;
    
    ID3D11ShaderResourceView* pTexture;    // the pointer to the texture
public:
    function<void()> fixMove;

    Cube(Position pos, Dimension dim, const LPCWSTR  pSrcFile) : Mesh(pos), dim(dim), pSrcFile(pSrcFile)      
    {
        CreateMesh();

        fixMove = [&]() {devcon->UpdateSubresource(pCBuffer, 0, 0, this->move(matView, matProjection), 0, 0); };
    }

    void CreateMesh() override
    {
        //Init Graphic
        VERTEX OurVertices[] =
        {
            {pos.x - dim.x / 2,  pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f},    // side 1
            {pos.x + dim.x / 2,pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f},
            {pos.x - dim.x / 2,pos.y + dim.y / 2,pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
            {pos.x + dim.x / 2,pos.y + dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f},

            {pos.x - dim.x / 2,pos.y - dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f},    // side 2
            {pos.x - dim.x / 2,pos.y + dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f},
            {pos.x + dim.x / 2,pos.y - dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f},
            {pos.x + dim.x / 2, pos.y + dim.y / 2,pos.z - dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f},

            {pos.x - dim.x / 2, pos.y + dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f},    // side 3
            {pos.x - dim.x / 2, pos.y + dim.y / 2,pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f},
            {pos.x + dim.x / 2, pos.y + dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f},
            {pos.x + dim.x / 2, pos.y + dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f},

            {pos.x - dim.x / 2, pos.y - dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f},    // side 4
            {pos.x + dim.x / 2, pos.y - dim.y / 2,pos.z - dim.z / 2, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f},
            {pos.x - dim.x / 2, pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f},
            {pos.x + dim.x / 2, pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f},

            {pos.x + dim.x / 2, pos.y - dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f},    // side 5
            {pos.x + dim.x / 2, pos.y + dim.y / 2,pos.z - dim.z / 2, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
            {pos.x + dim.x / 2, pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
            {pos.x + dim.x / 2, pos.y + dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f},

            {pos.x - dim.x / 2, pos.y - dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f},    // side 6
            {pos.x - dim.x / 2, pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
            {pos.x - dim.x / 2, pos.y + dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
            {pos.x - dim.x / 2, pos.y + dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f},
        };

        for (int i = 0; i < 24; i++)
            Vertices[i] = OurVertices[i];


        // create the vertex buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX) * 24;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        dev->CreateBuffer(&bd, NULL, &pVBuffer);

        // copy the vertices into the buffer
        D3D11_MAPPED_SUBRESOURCE ms;
        devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
        memcpy(ms.pData, Vertices, sizeof(Vertices));                 // copy the data
        devcon->Unmap(pVBuffer, NULL);

        // create the index buffer out of DWORDs
        DWORD OurIndices[] =
        {
            0, 1, 2,    // side 1
            2, 1, 3,
            4, 5, 6,    // side 2
            6, 5, 7,
            8, 9, 10,    // side 3
            10, 9, 11,
            12, 13, 14,    // side 4
            14, 13, 15,
            16, 17, 18,    // side 5
            18, 17, 19,
            20, 21, 22,    // side 6
            22, 21, 23,
        };

        // create the index buffer
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(DWORD) * 36;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bd.MiscFlags = 0;

        dev->CreateBuffer(&bd, NULL, &pIBuffer);

        devcon->Map(pIBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
        memcpy(ms.pData, OurIndices, sizeof(OurIndices));                   // copy the data
        devcon->Unmap(pIBuffer, NULL);

        D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
            pSrcFile,  // load Bricks.png in the local folder
            NULL,           // no additional information
            NULL,           // no multithreading
            &pTexture,      // address of the shader-resource-view
            NULL);          // no multithreading
    }

    void render() override
    {
        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
        devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);
        // select which primtive type we are using
        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        devcon->PSSetShaderResources(0, 1, &pTexture);
        devcon->DrawIndexed(36, 0, 0);
    }

    void lightning() override
    {
        cBuffer.LightVector = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
        cBuffer.LightColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        cBuffer.AmbientColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void update() override
    {
        this->lightning();
        //devcon->UpdateSubresource(pCBuffer, 0, 0, this->move(matView, matProjection), 0, 0);
        fixMove();
        this->render();
    }
    
};

