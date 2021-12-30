#pragma once
#include "MeshClass.h"
#include "Cube.h"

class Terrain : public MeshClass
{
public:
	Terrain(Position pos,Dimension dim, const LPCWSTR  pSrcFile) :MeshClass()
	{
		for (float i = 0; i < dim.x; i++)
		{
			for (float j = 0; j < dim.z; j++)
			{
				CreateMesh<Cube>(new Cube({ pos.x +i,pos.y,j }, { 1,1,1 }, pSrcFile));
				if (i == 0) continue;
				CreateMesh<Cube>(new Cube({ pos.x - i,pos.y,j }, { 1,1,1 }, pSrcFile));
			}
			
		}	
	}	
};

