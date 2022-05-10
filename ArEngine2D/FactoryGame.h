#pragma once

#include "FactoryCore.h"
#include "Camera.h"
#include "Block.h"
#include "Editor.h"

ARFAC_BEGIN_NAMESPACE

class FactoryGame : public ::ArEngine2D::Engine
{
public:

	using Engine::Engine;

public:

	void OnUserCreate() override;
	void OnUserUpdate(float dt) override;
	void OnUserDraw(Grafix& gfx) override;

private:
	void CenterCamera();
	DraggableCamera& GetCamera();
	void SetCamera(DraggableCamera& cam);
	void UpdateCamera(float dt);

private:
	DraggableCamera* pActiveCam_{};
	float camMinZoom_;
	float camMaxZoom_;
		
	Editor editor_{*this};

};

ARFAC_END_NAMESPACE