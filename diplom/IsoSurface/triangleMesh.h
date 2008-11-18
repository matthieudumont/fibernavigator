#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H
/*****************************************************************
*
* TriangleMesh.h
*
* Author:	Russ Stimpson
* Date:		2/7/03
*
*****************************************************************/

#include "Vector.h"
#include <vector>
#include "../DatasetHelper.h"

#include "../Fantom/FIndex.h"

#define PI 3.14159

class TriangleMesh {

	// Attributes
	private:
		DatasetHelper* m_dh;

		std::vector<Vector> vertices;
		std::vector<Vector> vertNormals;

		std::vector< Vector > triangles;
		std::vector<Vector> triNormals;
		std::vector< int >triangleTensor;
		std::vector<Vector>triangleColor;

		std::vector < std::vector<int> >vIsInTriangle;

		std::vector< std::vector<int> > neighbors;

		int	numVerts;
		int	numTris;

		bool openMeshError;

		// we don't delete vertices yet, so can do a cleanup only once
		bool isCleaned;
		// flag to indicate vertNormals and neighbors aren't calculated yet
		bool isFinished;

		Vector defaultColor;

	// Construction
	public:
		TriangleMesh (DatasetHelper* dh);
		~TriangleMesh ();

	// Operations
	public:
		void addVert(const Vector newVert);
		void addVert(const float x, const float y, const float z);
		void addTriangle(const int vertA, const int vertB, const int vertC);
		void addTriangle(const int vertA, const int vertB, const int vertC, const int tensorIndex);

		Vector calcTriangleNormal(const Vector);
		Vector calcTriangleNormal(const int triNum);

		void calcVertNormals();
		Vector getVertNormal(const int vertNum);

		int calcTriangleTensor(const int triNum);
		void calcTriangleTensors();

		int getNeighbor(const int coVert1, const int coVert2, const int triangleNum);
		void calcNeighbors();
		void calcNeighbor(const int triangleNum);

		void clearMesh();
		void finalize() { calcNeighbors(); calcVertNormals(); isFinished = true;};

		int getNumVertices()							{ return numVerts; };
		int getNumTriangles()							{ return numTris; };
		Vector getVertex (const int vertNum) 			{ return vertices[vertNum]; };
		Vector getVertex (const int triNum, int pos);
		Vector getNormal(const int triNum)				{ return triNormals[triNum]; };
		Vector getTriangle(const int triNum)			{ return triangles[triNum]; };
		Vector getTriangleColor(const int triNum)		{ return triangleColor[triNum];};
		std::vector<int> getStar(const int vertNum) 	{ return vIsInTriangle[vertNum]; };
		int getTriangleTensor(const int triNum)			{ return triangleTensor[triNum];};

		Vector getTriangleCenter(int triNum) ;


		void setVertex(const int vertNum, const Vector nPos)	{ vertices[vertNum] = nPos; };
		void eraseTriFromVert( const int triNum, const int vertNum);

		void setTriangle(const int triNum, const int vertA, const int vertB, const int vertC);
		void setTriangleColor(const int triNum, const float r, const float g, const float b);

		bool isInTriangle(const int vertNum, const int triangleNum);

		bool hasEdge(const int coVert1, const int coVert2, const int triangleNum);
		int getThirdVert(const int coVert1, const int coVert2, const int triangleNum);

		int getNextVertex(const int triNum, const int vertNum);

		bool getOpenMeshError() { return openMeshError; };

		void cleanUp();

		void doLoopSubD();

		void getCellVerticesIndices( const FIndex& cellId, std::vector< FIndex >& vertices ) const;
		void getPosition( FPosition& resultPos, const FIndex& pIndex ) const;
		void getEdgeNeighbor( const FIndex& cellId, int pos, std::vector< FIndex >& neigh ) const;
		void getNeighbors( const FIndex& vertId, std::vector< FIndex >& neighs ) const;


	private:


	// Overrides

	// Implementation
};
#endif
