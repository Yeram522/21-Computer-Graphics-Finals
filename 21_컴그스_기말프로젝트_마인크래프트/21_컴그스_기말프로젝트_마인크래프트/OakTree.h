#pragma once
#include "MeshClass.h"
#include "Cube.h"

class OakTree : public MeshClass
{
public:
	OakTree(Position pos):MeshClass()
	{
		CreateMesh<Cube>(new Cube({ pos.x,pos.y,pos.z+0 }, { 1,1,1 }, L"oak.png"));
		CreateMesh<Cube>(new Cube({ pos.x,pos.y + 1,pos.z + 0 }, { 1,1,1 }, L"oak.png"));
		CreateMesh<Cube>(new Cube({ pos.x,pos.y -1,pos.z + 0 }, { 1,1,1 }, L"oak.png"));

		for (float i = 0; i < 5; i++)
		{
			CreateMesh<Cube>(new Cube({ pos.x,pos.y + 2,pos.z-i }, { 1,1,1 }, L"bush.png"));
			CreateMesh<Cube>(new Cube({ pos.x+1,pos.y + 2,pos.z -i }, { 1,1,1 }, L"bush.png"));
			CreateMesh<Cube>(new Cube({ pos.x -1,pos.y + 2,pos.z -i }, { 1,1,1 }, L"bush.png"));
			CreateMesh<Cube>(new Cube({ pos.x+ 2,pos.y + 2,pos.z -i }, { 1,1,1 }, L"bush.png"));
			CreateMesh<Cube>(new Cube({ pos.x -2,pos.y + 2,pos.z -i }, { 1,1,1 }, L"bush.png"));
		}

		for (float i = 0; i < 5; i++)
		{
			CreateMesh<Cube>(new Cube({ pos.x,pos.y + 3,pos.z -i }, { 1,1,1 }, L"bush.png"));
			CreateMesh<Cube>(new Cube({ pos.x+1,pos.y + 3,pos.z -i }, { 1,1,1 }, L"bush.png"));
			CreateMesh<Cube>(new Cube({ pos.x -1,pos.y + 3,pos.z -i }, { 1,1,1 }, L"bush.png"));
		}
		
		CreateMesh<Cube>(new Cube({ pos.x,pos.y + 5,pos.z -2 }, { 1,1,1 }, L"bush.png"));
		CreateMesh<Cube>(new Cube({ pos.x+1,pos.y + 4,pos.z -2 }, { 1,1,1 }, L"bush.png"));
		CreateMesh<Cube>(new Cube({ pos.x -1,pos.y + 4,pos.z -2 }, { 1,1,1 }, L"bush.png"));
		CreateMesh<Cube>(new Cube({ pos.x,pos.y + 4,pos.z -3 }, { 1,1,1 }, L"bush.png"));
		CreateMesh<Cube>(new Cube({ pos.x,pos.y + 4,pos.z -1 }, { 1,1,1 }, L"bush.png"));
	}

};

