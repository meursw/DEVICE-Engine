#pragma once
#include "GFX_BindableInc.h"
#include "GFX_vertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

///
/// This class not only handles loading and drawing a model with a single mesh,
/// but also a scene, meaning a model with multiple meshes.
/// It construct a tree with a root node/mesh and other mesh children.
/// A transformation applied to a parent mesh is propagated to the children.
///

// Model Exception handling for assimp errors.
class ModelException : public DEVICE_Exception
{
public:
	ModelException(int, const char*, std::string);
	const char* what() const noexcept override;
	const char* GetType() const noexcept override;
	const std::string& GetNote() const noexcept;
private:
	std::string m_note;
};

// A single mesh of a scene. It inherits from the Drawable class so that
// it can call its methods for drawing.
class Mesh : public Drawable
{
public:
	Mesh(D3DClass*, std::vector<std::shared_ptr<Bindable>>);
	void Draw(D3DClass*, DirectX::FXMMATRIX) const;
	DirectX::XMMATRIX GetTransformXM() const override;
	
private:
	mutable DirectX::XMFLOAT4X4 m_transform;
};

class Node
{
	// The Model if a friend of Node since
	// it is going to be adding nodes to it.
	friend class Model;
public:
	Node(int id, const std::string&, std::vector<Mesh*>, const DirectX::XMMATRIX&);
	void Draw(D3DClass*, DirectX::FXMMATRIX) const;
	void SetAppliedTransform(DirectX::FXMMATRIX);
	int GetId() const noexcept;
private:
	void AddChild(std::unique_ptr<Node>);
private:
	std::string m_name;
	int id;
	std::vector<std::unique_ptr<Node>> m_childNodes;
	std::vector<Mesh*> m_meshPtrs;
	// The transform loaded from the file.
	DirectX::XMFLOAT4X4 m_baseTransform;
	// The transform applied from the control window.
	DirectX::XMFLOAT4X4 m_appliedTransform;

// IMGUI
public:
	void ShowTree(Node*& pSelectedNode) const;
};

class Model
{
public:
	Model(D3DClass*, const std::string fileName, HWND);
	void Draw(D3DClass*) const;
	~Model();

private:
	static std::unique_ptr<Mesh> ParseMesh(D3DClass*, const aiMesh& mesh, HWND, const aiMaterial* const*);
	std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node);

private:
	std::unique_ptr<Node> m_Root;
	std::vector<std::unique_ptr<Mesh>> m_meshPtrs;

// IMGUI 
public:
	void ShowWindow(const char*);
private:
	std::unique_ptr<class ModelWindow> m_pWindow;
};

