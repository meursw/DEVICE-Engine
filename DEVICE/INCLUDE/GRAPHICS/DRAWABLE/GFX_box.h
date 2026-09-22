#pragma once
#include "GFX_drawablebase.h"
#include "GFX_d3dclass.h"

class Box : public DrawableBase<Box>
{
public:
	Box(
		ID3D11Device*,
		HWND,
		std::mt19937& rng,
		std::uniform_real_distribution<float>& a,
		std::uniform_real_distribution<float>& b,
		std::uniform_real_distribution<float>& c,
		std::uniform_real_distribution<float>& d,
		std::uniform_real_distribution<float>& scale
	);
	
	void Update(float delta) override;
	DirectX::XMMATRIX GetTransformXM() const override;

private:
	float r;
	float roll{}, pitch{}, yaw{}, theta{}, phi{}, chi{};
	float droll{}, dpitch{}, dyaw{}, dtheta{}, dphi{}, dchi{};
	float scale{};
	float variableScale{};
};

