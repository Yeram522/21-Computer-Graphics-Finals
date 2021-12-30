# 21-Computer-Graphics-Finals
21_컴퓨터그래픽스 기말과제 : 인터렉티브 아트 만들기.

👀 개요

지금까지 directX 튜토리얼을 기반으로 배운, 

transforming, modeling, shader, texturing, lighting, blending을 활용하여 간단한 클래스 작성 후,

사용자의 입력을 받아 카메라 및 오브젝트가 움직이도록 만들었다.

기본적인, 코드는 교수님이 제공해주신, 튜토리얼 코드를 사용했다.





📄Class: Mesh

Mesh라는 Class를 만들어, indext-Buffer, vertex-Buffer, constant-Buffer, 행렬정보 및 위치정보를 담게 한 후, Quad,Cube, Circle의 class가 Mesh Class를 상속받도록 하였다.

//Mesh Class 멤버 변수

```c++
protected:
    D3DXMATRIX matTrans, matScale, matRotate;
    Position pos;
    ID3D11Buffer* pIBuffer;                // the pointer to the index buffer
    ID3D11Buffer* pVBuffer;
    CBUFFER cBuffer;
```

//멤버 가상 함수

```c++
public:
    virtual void CreateMesh() = 0 {}

    virtual void render()= 0 {}

    virtual void lightning()= 0 {}

    virtual void update() = 0 {}
```

순수가상함수로 만들어서, 반드시 자식클래스에서 위의 함수들을 재정의하도록 만들었다.



//transform 관련 멤버 함수

```c++
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
```

transform, rotation, scaling 행렬계산을 하는 함수이다. 행렬계산을 한 후 

move함수를 통해 전체 행렬을 계산해서 버퍼에 넣어줘야한다.

move함수는 오버라이드를 통해, 매개변수의 개수를 여러개로 두었다.

```c++
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
```



📄Class: Cube

대표적으로 Cube클래스를 소개하겠다.

Cube Class는 Mesh를 상속받은 클래스이다. 

private 멤버변수에는 vertex array와 dimension, texture파일 정보가 있다.

```c++
private:
    VERTEX Vertices[24];
    Dimension dim;
    const LPCWSTR  pSrcFile;
    
    ID3D11ShaderResourceView* pTexture;    // the pointer to the texture
```



```c++
VERTEX OurVertices[] =
{
  {pos.x - dim.x / 2,  pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f},    // side 1
  {pos.x + dim.x / 2,pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f},
  {pos.x - dim.x / 2,pos.y + dim.y / 2,pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
  {pos.x + dim.x / 2,pos.y + dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f},
    .
    .
    .
   {pos.x - dim.x / 2, pos.y - dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f},    // side 6
   {pos.x - dim.x / 2, pos.y - dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
   {pos.x - dim.x / 2, pos.y + dim.y / 2, pos.z - dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
   {pos.x - dim.x / 2, pos.y + dim.y / 2, pos.z + dim.z / 2, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f},
  };

  for (int i = 0; i < 24; i++)
     Vertices[i] = OurVertices[i];
```

원래, Vector 컨테이너를 이용하여 자동으로 넣고싶었는데, 자꾸 오류가나서, 따로 배열을 초기화하여 넣어줬다. 추후에 directX 공부를 하면서, 이 부분은 개선해보려고 한다.

그리고, 나머지 멤버함수들은 Mesh의 가상함수들을 재정의 해주었다.



이런식으로, Class를 만들고, 이 Cube클래스를 이용하여 Terrain 클래스, SnowMan등의 다른 오브젝트 클래스를 만들었다.



📄Class:MeshClass

Mesh클래스를 만들어서, 여러 매쉬들을 한번에 묶어 관리하게끔 하였다. 따라서 CreateMesh라는 함수를 Template 함수로 정의하여 다양한 도형 클래스들을  upcasting하여 vector에 저장하여 update 할 수 있도록 해보았다.

```c++
#include "Mesh.h"
#include "Cube.h"


class Cube;
class Mesh;

class MeshClass
{
protected:
	vector<Mesh*> Meshes;
public:
	MeshClass()
	{
		//Cube* test = new Cube({ 1,1,1 }, { 1,1,1 }, L"diamond.png");
		//this->CreateMesh<Cube>(test);
	}

	template<typename T>
	void CreateMesh(T* newMesh)
	{	
		if (!newMesh) return;
		T* mesh = newMesh;
		Meshes.push_back(mesh);
	}


	virtual void update()
	{
		for (int i = 0; i < Meshes.size(); i++)
		{
			Meshes[i]->update();
		}
	}
};
```

![](https://user-images.githubusercontent.com/63442636/147727888-dc64c78d-d5c0-49dc-8b1b-09bf90cfd613.gif)

