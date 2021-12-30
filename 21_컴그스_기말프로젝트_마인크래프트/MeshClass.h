#pragma once
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

