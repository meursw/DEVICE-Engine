#include "GFX_model.h"
#include "IMGUI/imgui.h"

#include <unordered_map>
#include <sstream>

using namespace DirectX;

/// MESH 
// This takes in a vector of bindables. 
// This means that some place else provides these bindables,
// and that this method doesn't haved a fixed set of bindables. 
// With this dependency injection, we are making the method more dynamic.
Mesh::Mesh(D3DClass* d3d, std::vector<std::shared_ptr<Bindable>> bindPtrs)
{
	AddBind(Topology::Resolve(d3d,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// Per mesh Binds.
	for (auto& pb : bindPtrs)
	{
		AddBind(std::move(pb));
	}

	// Transform bind.
	AddBind(std::make_shared<TransformCbuf>(d3d, *this));
}

void Mesh::Draw(D3DClass* d3d, FXMMATRIX accumulatedTransform) const
{
	XMStoreFloat4x4(&m_transform, accumulatedTransform);
	Drawable::Draw(d3d);
}

XMMATRIX Mesh::GetTransformXM() const
{
	return XMLoadFloat4x4(&m_transform);
}


/// NODE

Node::Node(int id, const std::string& name,std::vector<Mesh*> meshPtrs, const XMMATRIX& transform)
	:
	id(id),
	m_name(name),
	m_meshPtrs(std::move(meshPtrs))
{
	XMStoreFloat4x4(&m_baseTransform, transform);
	XMStoreFloat4x4(&m_appliedTransform, XMMatrixIdentity());
}

void Node::Draw(D3DClass* d3d, DirectX::FXMMATRIX accumulatedTransform) const
{
	const auto transform =
		XMLoadFloat4x4(&m_appliedTransform)*
		XMLoadFloat4x4(&m_baseTransform) * 
		accumulatedTransform;

	for (const Mesh* pMesh : m_meshPtrs)
	{
		pMesh->Draw(d3d, transform);
	}
	for (const auto& pNode : m_childNodes)
	{
		pNode->Draw(d3d, transform);
	}
}

void Node::SetAppliedTransform(DirectX::FXMMATRIX transform)
{
	XMStoreFloat4x4(&m_appliedTransform, transform);
}

void Node::ShowTree(Node*& pSelectedNode) const
{
	// if there is no selected node, set selectedId to an impossible value
	const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();

	// build up flags for current node
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((m_childNodes.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);

	// render this node
	const auto expanded = ImGui::TreeNodeEx(
		(void*)(intptr_t)GetId(), node_flags, m_name.c_str()
	);

	// processing for selecting node
	if (ImGui::IsItemClicked())
	{
		pSelectedNode = const_cast<Node*>(this);
	}

	// recursive rendering of open node's children
	if (expanded)
	{
		for (const auto& pChild : m_childNodes)
		{
			pChild->ShowTree(pSelectedNode);
		}
		ImGui::TreePop();
	}
}

void Node::AddChild(std::unique_ptr<Node> childNode)
{
	assert(childNode);
	m_childNodes.push_back(std::move(childNode));
}

int Node::GetId() const noexcept
{
	return id;
}

/// MODEL

// ModelWindow pImpl idiom.
class ModelWindow
{
public:
	void Show(D3DClass* d3d, const char* windowName, const Node& root)
	{
		windowName = windowName ? windowName : "Model";
		
		int nodeIndexTracker = 0;
		if (ImGui::Begin(windowName))
		{
			ImGui::Columns(2, nullptr, true);
			root.ShowTree(pSelectedNode);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				// Index operator creates transform if it doesnt exist.
				auto& transform = transforms[pSelectedNode->GetId()];
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
				ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
				ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);
			}
		}
		ImGui::End();
	}

	XMMATRIX GetTransform() const
	{
		const auto& transform = transforms.at(pSelectedNode->GetId());
		return
			XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *
			XMMatrixTranslation(transform.x, transform.y, transform.z);
	}

	Node* GetSelectedNode() const
	{
		return pSelectedNode;
	}

private:
	Node* pSelectedNode = nullptr;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};

public:
	// Map node indices to structs of transform parameters.
	// Keeps track of the transform parameters of each node.
	std::unordered_map<int, TransformParameters> transforms;
};

// This class handles:
//	1. Loading a model from a file name and getting an assimp scene
//	2. Parsing the meshes from that scene.
//  3. Parsing the nodes of the scene.

Model::Model(D3DClass* d3d, const std::string fileName, HWND hwnd)
	:
	m_pWindow(std::make_unique<ModelWindow>())
{
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(fileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);

	if (pScene == nullptr)
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		m_meshPtrs.push_back(ParseMesh(d3d, *pScene->mMeshes[i], hwnd, pScene->mMaterials));
	}

	int nextId = 0;
	m_Root = ParseNode(nextId, *pScene->mRootNode);
}

std::unique_ptr<Mesh> Model::ParseMesh(D3DClass* d3d, const aiMesh& mesh, HWND hwnd, const aiMaterial* const* pMaterials)
{
	using namespace std::string_literals;
	using DEVICE_VERTEX::VertexLayout;

	// Holds the bindables for this mesh.
	std::vector<std::shared_ptr<Bindable>> bindables;

	// Base path for all the textures. 
	// The texture filename that gets found from the mesh will append to base.
	const auto base = "../DEVICE/ASSETS/TEXTURES/GOBLIN/"s;

	bool hasSpecularMap = false;
	bool hasAlphaGloss = false;
	bool hasNormalMap = false;
	bool hasDiffuseMap = false;

	// Check if the mesh has a material assigned to it.
	if (mesh.mMaterialIndex >= 0)
	{
		// Get the material.
		auto& material = *pMaterials[mesh.mMaterialIndex];

		aiString texFilename;

		// Check if this mesh has a diffuse map.
		if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFilename) == aiReturn_SUCCESS)
		{
			bindables.push_back(Texture::Resolve(d3d, base + texFilename.C_Str(), hwnd, 0));
			hasDiffuseMap = true;
		}

		// Check if this mesh has a specular map.
		if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFilename) == aiReturn_SUCCESS)
		{
			bindables.push_back(Texture::Resolve(d3d, base + texFilename.C_Str(), hwnd, 1));
			hasSpecularMap = true;
		}

		// Check if this mesh has a normal map.
		if (material.GetTexture(aiTextureType_NORMALS, 0, &texFilename) == aiReturn_SUCCESS)
		{
			bindables.push_back(Texture::Resolve(d3d, base + texFilename.C_Str(), hwnd, 2));
			hasNormalMap = true;
		}

		if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
			bindables.push_back(Sampler::Resolve(d3d));

	}

	// Tag for the vertex and index buffers.
	const auto meshTag = "../DEVICE/ASSETS/MODELS/GOBLIN"s + "%" + mesh.mName.C_Str();

	// Load different type of vertex buffers, shaders, etc. based on the mesh data.
	// This needs to be seperated and generalized.
	if (hasDiffuseMap && hasNormalMap && hasSpecularMap)
	{
		// Create vertex layout.
		DEVICE_VERTEX::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Texture2D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
		));

		// Load vertices.
		for (UINT i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				*reinterpret_cast<XMFLOAT3*>(&mesh.mVertices[i]),
				*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mBitangents[i])
			);
		}

		// Load indices.
		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// Add vertex and index buffer binds.
		bindables.push_back(VertexBuffer::Resolve(d3d, meshTag, vbuf));
		bindables.push_back(IndexBuffer::Resolve(d3d, meshTag, indices));

		// Add vertex shader and get byte code for input layout.
		auto vertexShader = VertexShader::Resolve(
			d3d,
			ShaderType::VERTEX_SHADER,
			hwnd,
			"SHADERS/phongNormalMap.vs",
			"PhongVertexEntry"
		);

		auto vsBytecode = vertexShader->GetBytecode();
		bindables.push_back(std::move(vertexShader));

		// Add pixel shader.
		bindables.push_back(PixelShader::Resolve(
			d3d,
			ShaderType::PIXEL_SHADER,
			hwnd,
			"SHADERS/phongSpecNormalMap.ps",
			"PhongPixelEntry"
		));

		bindables.push_back(InputLayout::Resolve(d3d, vbuf.GetLayout(), vsBytecode));

	}
	else if (hasDiffuseMap && hasNormalMap)
	{
		// Create vertex layout.
		DEVICE_VERTEX::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Texture2D)
			.Append(VertexLayout::Normal)
			.Append(VertexLayout::Tangent)
			.Append(VertexLayout::Bitangent)
		));

		// Load vertices.
		for (UINT i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				*reinterpret_cast<XMFLOAT3*>(&mesh.mVertices[i]),
				*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mTangents[i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mBitangents[i])
			);
		}

		// Load indices.
		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// Add vertex and index buffer binds.
		bindables.push_back(VertexBuffer::Resolve(d3d, meshTag, vbuf));
		bindables.push_back(IndexBuffer::Resolve(d3d, meshTag, indices));

		// Add vertex shader and get byte code for input layout.
		auto vertexShader = VertexShader::Resolve(
			d3d,
			ShaderType::VERTEX_SHADER,
			hwnd,
			"SHADERS/phongNormalMap.vs",
			"PhongVertexEntry"
		);

		auto vsBytecode = vertexShader->GetBytecode();
		bindables.push_back(std::move(vertexShader));

		// Add pixel shader.
		bindables.push_back(PixelShader::Resolve(
			d3d,
			ShaderType::PIXEL_SHADER,
			hwnd,
			"SHADERS/phongNormalMap.ps",
			"PhongPixelEntry"
		));

		bindables.push_back(InputLayout::Resolve(d3d, vbuf.GetLayout(), vsBytecode));

	}
	else if (hasDiffuseMap)
	{
		DEVICE_VERTEX::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Texture2D)
			.Append(VertexLayout::Normal)
		));

		for (UINT i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				*reinterpret_cast<XMFLOAT3*>(&mesh.mVertices[i]),
				*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// Add vertex and index buffer binds.
		bindables.push_back(VertexBuffer::Resolve(d3d, meshTag, vbuf));
		bindables.push_back(IndexBuffer::Resolve(d3d, meshTag, indices));

		// Add vertex shader and get byte code for input layout.
		auto vertexShader = VertexShader::Resolve(
			d3d,
			ShaderType::VERTEX_SHADER,
			hwnd,
			"SHADERS/phong.vs",
			"PhongVertexEntry"
		);

		auto vsBytecode = vertexShader->GetBytecode();
		bindables.push_back(std::move(vertexShader));

		// Add pixel shader.
		bindables.push_back(PixelShader::Resolve(
			d3d,
			ShaderType::PIXEL_SHADER,
			hwnd,
			"SHADERS/phong.ps",
			"PhongPixelEntry"
		));

		bindables.push_back(InputLayout::Resolve(d3d, vbuf.GetLayout(), vsBytecode));
	}
	else if (!hasDiffuseMap && !hasNormalMap && !hasSpecularMap)
	{
		DEVICE_VERTEX::VertexBuffer vbuf(std::move(
			VertexLayout{}
			.Append(VertexLayout::Position3D)
			.Append(VertexLayout::Normal)
		));

		for (UINT i = 0; i < mesh.mNumVertices; i++)
		{
			vbuf.EmplaceBack(
				*reinterpret_cast<XMFLOAT3*>(&mesh.mVertices[i]),
				*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i])
			);
		}

		std::vector<unsigned short> indices;
		indices.reserve(mesh.mNumFaces * 3);
		for (unsigned int i = 0; i < mesh.mNumFaces; i++)
		{
			const auto& face = mesh.mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		// Add vertex and index buffer binds.
		bindables.push_back(VertexBuffer::Resolve(d3d, meshTag, vbuf));
		bindables.push_back(IndexBuffer::Resolve(d3d, meshTag, indices));

		// Add vertex shader and get byte code for input layout.
		auto vertexShader = VertexShader::Resolve(
			d3d,
			ShaderType::VERTEX_SHADER,
			hwnd,
			"SHADERS/phongNoTex.vs",
			"PhongVertexEntry"
		);

		auto vsBytecode = vertexShader->GetBytecode();
		bindables.push_back(std::move(vertexShader));

		// Add pixel shader.
		bindables.push_back(PixelShader::Resolve(
			d3d,
			ShaderType::PIXEL_SHADER,
			hwnd,
			"SHADERS/phongNoTex.ps",
			"PhongPixelEntry"
		));

		bindables.push_back(InputLayout::Resolve(d3d, vbuf.GetLayout(), vsBytecode));

	}
	else
		throw std::runtime_error("something went wrong with the textures in the material");

	return std::make_unique<Mesh>(d3d, std::move(bindables));
}

std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node)
{
	const auto transform = XMMatrixTranspose(XMLoadFloat4x4(
		reinterpret_cast<const XMFLOAT4X4*>(&node.mTransformation)
	));

	std::vector<Mesh*> curMeshPtrs;
	curMeshPtrs.reserve(node.mNumMeshes);

	// Get all the meshes attachted to this node.
	for (size_t i = 0; i < node.mNumMeshes; i++)
	{
		const auto meshIdx = node.mMeshes[i];
		curMeshPtrs.push_back(m_meshPtrs.at(meshIdx).get());
	}

	// Load all the children of this node.
	auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
	for (size_t i = 0; i < node.mNumChildren; i++)
	{
		pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
	}

	return pNode;
}

void Model::ShowWindow(D3DClass* d3d, const char* windowName)
{
	m_pWindow->Show(d3d, windowName, *m_Root);
}

void Model::Draw(D3DClass* d3d) const
{
	if (auto node = m_pWindow->GetSelectedNode())
	{
		node->SetAppliedTransform(m_pWindow->GetTransform());
	}
	m_Root->Draw(d3d, XMMatrixIdentity());
}

Model::~Model() {}

ModelException::ModelException(int line, const char* file, std::string note)
	:
	DEVICE_Exception(line, file),
	m_note(std::move(note))
{}

const char* ModelException::what() const noexcept
{
	std::ostringstream oss;
	oss << DEVICE_Exception::what() << std::endl
		<< "[Note] " << GetNote();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* ModelException::GetType() const noexcept
{
	return "DEVICE Model Exception";
}

const std::string& ModelException::GetNote() const noexcept
{
	return m_note;
}
