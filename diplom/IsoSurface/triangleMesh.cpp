// TriangleMesh.cpp
#include "triangleMesh.h"
#include "loopSubD.h"
#include <string.h>

// Construction
TriangleMesh::TriangleMesh (DatasetHelper* dh)
{
	m_dh = dh;
	numVerts	 = 0;
	numTris		 = 0;
	defaultColor = Vector(0.5f, 0.5f, 0.5f);

	isCleaned = false;
	isFinished = false;
}

// Destructor
TriangleMesh::~TriangleMesh ()
{

}

// Operations

void TriangleMesh::addVert(const Vector newVert)
{
	vertices.push_back( newVert );
	std::vector<int> v;
	vIsInTriangle.push_back( v );
	numVerts = vertices.size();
}

void TriangleMesh::addVert(const float x, const float y, const float z)
{
	Vector newVert(x,y,z);
	vertices.push_back( newVert );
	std::vector<int> v;
	vIsInTriangle.push_back( v );
	numVerts = vertices.size();
}


void TriangleMesh::addTriangle(const int vertA, const int vertB, const int vertC)
{
	Vector t(vertA, vertB, vertC);
	triangles.push_back(t);
	triNormals.push_back(calcTriangleNormal(t));
	vIsInTriangle[vertA].push_back(numTris);
	vIsInTriangle[vertB].push_back(numTris);
	vIsInTriangle[vertC].push_back(numTris);
	std::vector<int> v(3,-1);
	neighbors.push_back( v );
	numTris = triangles.size();

	Vector p = ( getVertex(vertA) + getVertex(vertB) + getVertex(vertC) )/3.0;
	int x = wxMin(m_dh->columns-1, wxMax(0,(int)p[0]));
	int y = wxMin(m_dh->rows   -1, wxMax(0,(int)p[1]));
	int z = wxMin(m_dh->frames -1, wxMax(0,(int)p[2]));

	int index = x + y * m_dh->columns + z * m_dh->columns * m_dh->rows;
	triangleTensor.push_back(index);
	triangleColor.push_back(defaultColor);
}

void TriangleMesh::addTriangle(const int vertA, const int vertB, const int vertC, const int tensorIndex)
{
	Vector t(vertA, vertB, vertC);
	triangles.push_back(t);
	triNormals.push_back(calcTriangleNormal(t));
	vIsInTriangle[vertA].push_back(numTris);
	vIsInTriangle[vertB].push_back(numTris);
	vIsInTriangle[vertC].push_back(numTris);
	std::vector<int> v(3,-1);
	neighbors.push_back( v );
	numTris = triangles.size();
	triangleTensor.push_back(tensorIndex);
	triangleColor.push_back(defaultColor);
}

void TriangleMesh::setTriangleColor(const int triNum, const float r, const float g, const float b)
{
	Vector c(r,g,b);
	triangleColor[triNum] = c;
}

Vector TriangleMesh::calcTriangleNormal(const Vector t)
{

	Vector v1 = vertices[t[1]] - vertices[t[0]];
	Vector v2 = vertices[t[2]] - vertices[t[0]];

	Vector tempNormal = v1.Cross(v2);
	tempNormal.normalize();
	return tempNormal;
}

Vector TriangleMesh::calcTriangleNormal(const int triNum)
{

	Vector v1 = vertices[triangles[triNum][1]] - vertices[triangles[triNum][0]];
	Vector v2 = vertices[triangles[triNum][2]] - vertices[triangles[triNum][0]];

	Vector tempNormal = v1.Cross(v2);
	tempNormal.normalize();
	return tempNormal;
}

Vector TriangleMesh::getVertNormal(const int vertNum)
{
	Vector sum(0,0,0);

	for(size_t i = 0 ; i < vIsInTriangle[vertNum].size() ; ++i)
	{
		sum = sum + triNormals[vIsInTriangle[vertNum][i]];
	}
	sum.normalize();
	return sum;
}

Vector TriangleMesh::getVertex (const int triNum, int pos)
{
	if (pos < 0 || pos > 2) pos = 0;
	return vertices[triangles[triNum][pos]];
}

void TriangleMesh::calcVertNormals()
{
	vertNormals.clear();
	for ( int i = 0 ; i <numVerts ; ++i)
		vertNormals.push_back(getVertNormal(i));
}

int TriangleMesh::getNeighbor(const int coVert1, const int coVert2, const int triangleNum)
{
	std::vector<int>candidates = vIsInTriangle[coVert1];
	std::vector<int>compares   = vIsInTriangle[coVert2];

	for (size_t i = 0 ; i < candidates.size() ; ++i)
		for (size_t k = 0 ; k < compares.size() ; ++k)
		{
			if ( (candidates[i] != triangleNum) && (candidates[i] == compares[k]))
				return candidates[i];
		}
	return triangleNum;
}

void TriangleMesh::calcNeighbors()
{
	for( int i = 0 ; i < numTris ; ++i)
	{
		int coVert0 = triangles[i][0];
		int coVert1 = triangles[i][1];
		int coVert2 = triangles[i][2];

		neighbors[i][0] = getNeighbor(coVert0, coVert1, i);
		neighbors[i][1] = getNeighbor(coVert1, coVert2, i);
		neighbors[i][2] = getNeighbor(coVert2, coVert0, i);
	}
}

void TriangleMesh::calcNeighbor(const int triangleNum)
{
	int coVert0 = triangles[triangleNum][0];
	int coVert1 = triangles[triangleNum][1];
	int coVert2 = triangles[triangleNum][2];

	neighbors[triangleNum][0] = getNeighbor(coVert0, coVert1, triangleNum);
	neighbors[triangleNum][1] = getNeighbor(coVert1, coVert2, triangleNum);
	neighbors[triangleNum][2] = getNeighbor(coVert2, coVert0, triangleNum);
}

int TriangleMesh::calcTriangleTensor(const int triNum)
{
	Vector p = getTriangleCenter(triNum);
	int x = wxMin(m_dh->columns-1, wxMax(0,(int)(p[0] + 0.5)));
	int y = wxMin(m_dh->rows   -1, wxMax(0,(int)(p[1] + 0.5)));
	int z = wxMin(m_dh->frames -1, wxMax(0,(int)(p[2] + 0.5)));
	return  x + y * m_dh->columns + z * m_dh->columns * m_dh->rows;
}

void TriangleMesh::calcTriangleTensors()
{
	triangleTensor.clear();
	for( int i = 0 ; i < numTris ; ++i)
	{
		triangleTensor.push_back(calcTriangleTensor(i));
	}
}

Vector TriangleMesh::getTriangleCenter(const int triNum)
{
	Vector v0 = vertices[(int)triangles[triNum][0]];
	Vector v1 = vertices[(int)triangles[triNum][1]];
	Vector v2 = vertices[(int)triangles[triNum][2]];
	Vector p = ( v0 + v1 + v2 )/3.0;
	return p;
}

bool TriangleMesh::hasEdge(const int coVert1, const int coVert2, const int triangleNum){

	return(isInTriangle(coVert1,triangleNum) && isInTriangle(coVert2,triangleNum));

}

bool TriangleMesh::isInTriangle(const int vertNum, const int triangleNum)
{
	return ( (vertNum == triangles[triangleNum][0]) ||
			 (vertNum == triangles[triangleNum][1]) ||
			 (vertNum == triangles[triangleNum][2]) );
}

void TriangleMesh::clearMesh()
{
	vertices.clear();
	vertNormals.clear();
	triangles.clear();
	triangleTensor.clear();
	triangleColor.clear();
	neighbors.clear();
	triNormals.clear();
	vIsInTriangle.clear();

	numVerts	 = 0;
	numTris		 = 0;
}

int TriangleMesh::getThirdVert(const int coVert1, const int coVert2, const int triangleNum)
{
	int index = 0;
	bool found = false;

	while((index < 2) && !found){
		if((triangles[triangleNum][index] == coVert1) || (triangles[triangleNum][index] == coVert2)){
			index++;
		} else {
			found = true;
		}
	}

	return triangles[triangleNum][index];
}


void TriangleMesh::setTriangle(const int triNum, const int vertA, const int vertB, const int vertC)
{
	// TODO
	eraseTriFromVert(triNum, triangles[triNum][1]);
	eraseTriFromVert(triNum, triangles[triNum][2]);

	triangles[triNum][0] = vertA;
	triangles[triNum][1] = vertB;
	triangles[triNum][2] = vertC;

	vIsInTriangle[vertB].push_back(triNum);
	vIsInTriangle[vertC].push_back(triNum);

	triNormals[triNum] = calcTriangleNormal(triNum);
}

void TriangleMesh::eraseTriFromVert(const int triNum, const int vertNum)
{
	std::vector<int>temp;
	for ( size_t i = 0 ; i < vIsInTriangle[vertNum].size() ; ++i)
	{
		if ( triNum != vIsInTriangle[vertNum][i])
			temp.push_back(vIsInTriangle[vertNum][i]);
	}
	vIsInTriangle[vertNum] = temp;
}

int TriangleMesh::getNextVertex(const int triNum, const int vertNum)
{
	int answer = -1;

	if(triangles[triNum][0] == vertNum){
		answer = triangles[triNum][1];
	} else if(triangles[triNum][1] == vertNum){
		answer = triangles[triNum][2];
	} else {
		answer = triangles[triNum][0];
	}

	return answer;
}

void TriangleMesh::cleanUp()
{
	if ( isCleaned ) return;
	std::vector<int> queue;
	std::vector<bool> visited(numTris, false);
	std::vector< std::vector<int> >objects;
	queue.push_back(0);
	std::vector<int>n;

	while (!queue.empty())
	{
		std::vector<int>newObject;
		newObject.clear();
		while (!queue.empty())
		{
			int index = queue.back();
			visited[index] = true;
			queue.pop_back();
			newObject.push_back(index);
			n = neighbors[index];
			for ( int i = 0 ; i < 3 ; ++i)
			{
				if ( (n[i] != -1) && !visited[n[i]])
					queue.push_back(n[i]);
			}
		}
		int counter = 0;
		for (int i = 0 ; i < numTris ; ++i)
			if (!visited[i]) ++counter;
		//printf("%d triangles not visited\n", counter);
		for (int i = 0 ; i < numTris ; ++i)
		{
			if (!visited[i])
			{
				queue.push_back(i);
				break;
			}
		}
		objects.push_back(newObject);
	}
	if (objects.size() == 1) return;
	size_t biggest = 0;
	size_t sizeBiggest = objects[0].size();
	for ( size_t i = 0 ; i < objects.size() ; ++i)
	{
		if (sizeBiggest < objects[i].size() )
		{
			biggest = i;
			sizeBiggest = objects[i].size();
		}
	}

	std::vector<int>obj = objects[biggest];
	std::vector<Vector>tempTriangles;
	for (size_t i = 0 ; i < obj.size() ; ++i)
	{
		tempTriangles.push_back(triangles[obj[i]]);
	}

	vertNormals.clear();
	triangles.clear();
	triNormals.clear();
	vIsInTriangle.clear();
	neighbors.clear();
	numTris = 0;

	std::vector<int> v;
	for (int i = 0 ; i < numVerts ; ++i)
	{
		vIsInTriangle.push_back( v );
	}

	for (size_t i = 0 ; i < tempTriangles.size() ; ++i)
	{
		Vector t = tempTriangles[i];
		addTriangle((int)t[0], (int)t[1], (int)t[2]);
	}
	calcNeighbors();
	calcVertNormals();

	isCleaned = true;
}

void TriangleMesh::doLoopSubD()
{
	loopSubD loop(this);
}


void TriangleMesh::getCellVerticesIndices( const FIndex& triNum,
							      std::vector< FIndex >& vertices ) const
{
    //assert( triNum < numTris );
    vertices.clear();

	vertices.push_back( FIndex((int)triangles[triNum][0]) );
	vertices.push_back( FIndex((int)triangles[triNum][1]) );
	vertices.push_back( FIndex((int)triangles[triNum][2]) );
}

void TriangleMesh::getPosition( FPosition& resultPos, const FIndex& pIndex ) const
{
    positive ind = pIndex.getIndex();
    //assert( ind < numVerts );
    resultPos.resize(3);
    resultPos[0] = vertices[ind].x;
    resultPos[1] = vertices[ind].y;
    resultPos[2] = vertices[ind].z;
}

void TriangleMesh::getEdgeNeighbor( const FIndex& triNum, int pos, std::vector< FIndex >& neigh ) const
{
	//assert( triNum < numTris );
	neigh.clear();
	neigh.push_back(FIndex((int)neighbors[triNum][pos]));
}

void TriangleMesh::getNeighbors( const FIndex& vertId, std::vector< FIndex >& neighs ) const
{
	//assert( triNum < numTris );
	neighs.clear();
	std::vector<int>neighbors = vIsInTriangle[vertId];
	for (size_t i = 0 ; i < neighbors.size() ; ++i)
	{
		neighs.push_back(FIndex(neighbors[i]));
	}
}
