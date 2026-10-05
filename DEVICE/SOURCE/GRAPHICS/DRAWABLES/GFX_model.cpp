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
Mesh::Mesh(D3DClass* d3d, std::vector<std::unique_ptr<Bindable>> bindPtrs)
{
	// Add a static Bind for all meshes.
	if (!IsStaticInitialized())
	{
		AddStaticBind(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> color(1.0f, 1.0f);

		DirectX::XMFLOAT4 materialColor = { color(rng), color(rng), color(rng), color(rng) };
		struct MaterialCbuf
		{
			DirectX::XMFLOAT4 color;
		} materialBuf;

		materialBuf.color = materialColor;
		AddStaticBind(std::make_unique<PixelConstantBuffer<MaterialCbuf>>(d3d->GetDevice(), materialBuf, 1));
		AddStaticBind(std::make_unique<Texture>(d3d, L"../DEVICE/ASSETS/TEXTURES/drybones.png", nullptr));
		AddStaticBind(std::make_unique<Sampler>(d3d->GetDevice()));
	}

	// Per mesh Binds.
	for (auto& pb : bindPtrs)
	{
		if (auto pi = dynamic_cast<IndexBuffer*>(pb.get()))
		{
			AddIndexBuffer(std::unique_ptr<IndexBuffer>{pi});
			pb.release();
		}
		else
		{
			AddBind(std::move(pb));
		}
	}

	// Transform bind.
	AddBind(std::make_unique<TransformCbuf>(d3d->GetDevice(), *this));
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
	void Show(const char* windowName, const Node& root)
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
				ImGui::SliderFloat("X", &transform.x, -2.0f, 2.0f);
				ImGui::SliderFloat("Y", &transform.y, -2.0f, 2.0f);
				ImGui::SliderFloat("Z", &transform.z, -2.0f, 2.0f);
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
	Node* pSelectedNode;
	struct TransformParameters
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
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
		aiProcess_GenNormals
	);

	if (pScene == nullptr)
		throw ModelException(__LINE__, __FILE__, imp.GetErrorString());

	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		m_meshPtrs.push_back(ParseMesh(d3d, *pScene->mMeshes[i], hwnd));
	}

	int nextId = 0;
	m_Root = ParseNode(nextId, *pScene->mRootNode);
}

std::unique_ptr<Mesh> Model::ParseMesh(D3DClass* d3d, const aiMesh& mesh, HWND hwnd)
{
	using DEVICE_VERTEX::VertexLayout;

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
	for (UINT i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	std::vector<std::unique_ptr<Bindable>> bindablePtrs;

	auto device = d3d->GetDevice();

	bindablePtrs.push_back(std::make_unique<VertexBuffer>(device, vbuf));
	bindablePtrs.push_back(std::make_unique<IndexBuffer>(device, indices));

	auto vertexShader = std::make_unique<VertexShader>(
		ShaderType::VERTEX_SHADER,
		device,
		hwnd,
		L"SHADERS/phong.vs",
		"PhongVertexEntry"
	);

	auto vsByteCode = vertexShader->GetBytecode();

	bindablePtrs.push_back(std::make_unique<PixelShader>(
		ShaderType::PIXEL_SHADER,
		device,
		hwnd,
		L"SHADERS/phong.ps",
		"PhongPixelEntry"
	));

	bindablePtrs.push_back(std::move(vertexShader));

	bindablePtrs.push_back(std::make_unique<InputLayout>(device, vbuf.GetLayout().GetD3DLayout(), vsByteCode));

	return std::make_unique<Mesh>(d3d, std::move(bindablePtrs));

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

void Model::ShowWindow(const char* windowName)
{
	m_pWindow->Show(windowName, *m_Root);
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
