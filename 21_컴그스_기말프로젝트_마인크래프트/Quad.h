#pragma once
#include "Mesh.h"

//2D 사각형이다.
class Quad : public Mesh
{
private:
	Dimension dim;
private:
	VERTEX Vertices[7];
    const LPCWSTR  pSrcFile;

    ID3D11ShaderResourceView* pTexture;    // the pointer to the texture
public:
	Quad(Position pos, Dimension dim, const LPCWSTR  pSrcFile)
        : Mesh(pos), dim(dim), pSrcFile(pSrcFile)
	{
        CreateMesh();
       
	}

    void CreateMesh() override
    {
        VERTEX OurVertices[] =
        {
            {pos.x - dim.x / 2,  pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f},    // side 1
            {pos.x + dim.x / 2,pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f},
            {pos.x - dim.x / 2,pos.y + dim.y / 2,pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
            {pos.x + dim.x / 2,pos.y + dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f}
        };

        for (int i = 0; i < 4; i++)
            Vertices[i] = OurVertices[i];

        // create the vertex buffer
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
        bd.ByteWidth = sizeof(VERTEX) * 4;             // size is the VERTEX struct * 3
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

        dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


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
        };

        // create the index buffer
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(DWORD) * 6;
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
            NULL);          // no multithreading                                   // unmap the buffer
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
        devcon->DrawIndexed(6, 0, 0);
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
        devcon->UpdateSubresource(pCBuffer, 0, 0, this->move(matView, matProjection), 0, 0);
        this->render();
    }
};

