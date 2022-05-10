#include "FactoryGame.h"

#include "DataGrid.h"

#include <filesystem>
#include <fstream>

namespace ArFac {
	void FactoryGame::OnUserCreate()
	{
		SetCamera(editor_.Cam());
		camMaxZoom_ = editor_.CalcMaxZoom(1.f * window.Width());
		camMinZoom_ = editor_.CalcMinZoom(1.f * window.Width());
		CenterCamera();
	}

	void FactoryGame::OnUserUpdate(float dt)
	{
		editor_.UpdateInput(dt);
		UpdateCamera(dt);
	}

	void FactoryGame::OnUserDraw(Grafix& gfx)
	{
		gfx.ClearScreen();
		editor_.Draw(gfx);
		gfx.DrawString({}, std::format("tran loc: {}", (*pActiveCam_)[mouse.loc]), Colors::Orange, 30.f);
		gfx.DrawString({0.f, 30.f}, std::format("loc: {}", mouse.loc), Colors::Orange, 30.f);
		gfx.DrawString({0.f, 60.f}, std::format("cam loc: {}", pActiveCam_->Loc()), Colors::Orange, 30.f);
		gfx.DrawString({0.f, 90.f}, std::format("cam scale: {}", pActiveCam_->Scale()), Colors::Orange, 30.f);
	}

	void FactoryGame::CenterCamera()
	{
		auto& cam{GetCamera()};
		cam.MoveTo(0.5f * Vec2{editor_.LevelWidth(), editor_.LevelHeight()});
		cam.SetZoom(camMinZoom_);
	}

	DraggableCamera& FactoryGame::GetCamera()
	{
		return *pActiveCam_;
	}

	void FactoryGame::SetCamera(DraggableCamera& cam)
	{
		pActiveCam_ = &cam;
	}
	
	void FactoryGame::UpdateCamera(float dt)
	{
		auto& cam{GetCamera()};

		// update the scale
		cam.UpdateZoomUsingScrollWheel();
		
		// clamp scale (updating and clamp the scale must be done first)
		auto const mapVec = Vec2{editor_.LevelWidth(), editor_.LevelHeight()};
		auto const& loc{cam.Loc()};
		auto const camScale{cam.Scale()};
		auto const winVec = Vec2{1.f * window.Width(), 1.f * window.Height()};
		auto const actMinZoom{std::min(camMinZoom_, camScale * (mapVec.y / winVec.y))};
		cam.SetZoom(std::clamp(camScale, camMinZoom_, camMaxZoom_));

		// update position
		cam.UpdateDrag(mouse.right);

		// clamp position
		auto const updatedCamScale{cam.Scale()};
		auto const topLeft = (0.5f / updatedCamScale) * winVec;
		auto const botRight = mapVec - topLeft;
		cam.MoveTo(Vec2::Clamp(loc, topLeft, botRight));
	}
}