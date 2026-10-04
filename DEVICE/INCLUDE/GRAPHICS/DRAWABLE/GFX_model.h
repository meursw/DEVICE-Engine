#pragma once
#include "GFX_drawablebase.h"
#include "GFX_BindableInc.h"
#include "GFX_vertex.h"

#include <optional>

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
class Mesh : public DrawableBase<Mesh>
{
public:
	Mesh(D3DClass*, std::vector<std::unique_ptr<Bindable>>);
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
	friend class ModelWindow;
public:
	Node(const std::string&, std::vector<Mesh*>, const DirectX::XMMATRIX& transform);
	void Draw(D3DClass*, DirectX::FXMMATRIX) const;
	void SetAppliedTransform(DirectX::FXMMATRIX);
private:
	void AddChild(std::unique_ptr<Node>);
private:
	std::string m_name;
	std::vector<std::unique_ptr<Node>> m_childNodes;
	std::vector<Mesh*> m_meshPtrs;
	// The transform loaded from the file.
	DirectX::XMFLOAT4X4 m_baseTransform;
	// The transform applied from the control window.
	DirectX::XMFLOAT4X4 m_appliedTransform;

// IMGUI
public:
	void ShowTree(int&, std::optional<int>&, Node*& pSelectedNode) const;
};

class Model
{
public:
	Model(D3DClass*, const std::string fileName, HWND);
	void Draw(D3DClass*) const;
	~Model();

private:
	static std::unique_ptr<Mesh> ParseMesh(D3DClass*, const aiMesh& mesh, HWND);
	std::unique_ptr<Node> ParseNode(const aiNode& node);

private:
	std::unique_ptr<Node> m_Root;
	std::vector<std::unique_ptr<Mesh>> m_meshPtrs;

// IMGUI 
public:
	void ShowWindow(const char*);
private:
	std::unique_ptr<class ModelWindow> m_pWindow;
};

