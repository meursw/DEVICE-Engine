#include "GFX_modelclass.h"
#include <fstream>
#include <sstream>

using namespace DirectX;
using namespace std;

ModelClass::ModelClass(const wstring& objfile)
{
	ParseObjFileAndLoadModel(objfile);
}

void ModelClass::ParseObjFileAndLoadModel(const wstring& objfile)
{
	ifstream fin(objfile);

	// Open the file
	if (!fin.is_open())
	{
		std::wstring error = L"Could not load file: " + objfile;
		MessageBox(nullptr, error.c_str(), L"ERROR", MB_OK);
		throw DEVICE_Exception(__LINE__, __FILE__);
	}
	
	// These will store the data for each vertex.
	vector<XMFLOAT3> verts;
	vector<XMFLOAT2> texCoords;
	vector<XMFLOAT3> normals;

	// Stores the triangles of the model, that contain all the data.
	vector<vector<float>> vertexData;

	// Keep track of the vertex count.
	// This will increment everytime we create a face.
	int vertexCount{ 0 };

	char line[128];
	
	while (fin.getline(line, sizeof line))
	{
		// Use stringstream to get the data.
		stringstream s;
		// Used to get rid of junk data like "v", etc.
		char junk;

		// Supply the line to the strstream.		
		s << line;

		if (line[0] == 'v')
		{
			if (line[1] == 't')
			{
				XMFLOAT2 vt{};

				s >> junk >> junk;
				s >> vt.x >> vt.y;
				texCoords.push_back(vt);
			}
			else if (line[1] == 'n')
			{
				XMFLOAT3 vn{};

				s >> junk >> junk;
				s >> vn.x >> vn.y >> vn.z;
				normals.push_back(vn);
			}
			else
			{
				XMFLOAT3 v{};

				s >> junk;
				s >> v.x >> v.y >> v.z;
				verts.push_back(v);
			}
		}

		if (line[0] == 'f')
		{
			vector<XMFLOAT3> face;

			s >> junk;

			for (int i = 0; i < 3; i++)
			{
				XMFLOAT3 f{};

				s >> f.x >> junk >> f.y >> junk >> f.z;
				face.push_back(f);
			}

			for (int i = 0; i < 3; i++)
			{
				vertexCount++;

				XMFLOAT3 f = face[i];

				XMFLOAT3 v = verts[f.x - 1];
				XMFLOAT2 t = texCoords[f.y - 1];
				XMFLOAT3 n = normals[f.z - 1];

				vector<float> vData
				{
					v.x, v.y, v.z,
					t.x, t.y,
					n.x, n.y, n.z
				};

				vertexData.push_back(vData);
			}
		}
	}

	m_vertexCount = vertexCount;

	UINT i = 0;
	for (const vector<float>& vec : vertexData)
	{
		VertexType v{};
		v.pos = { vec[0], vec[1], vec[2] };
		v.tex = { vec[3], vec[4] };
		v.normal = { vec[5], vec[6], vec[7] };
		m_vertices.push_back(v);

		m_indices.push_back(i++);
	}

	fin.close();

}
