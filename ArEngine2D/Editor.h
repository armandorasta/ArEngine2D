#pragma once

#include "DataGrid.h"
#include "Block.h"
#include "Camera.h"
#include "Engine.h"

#include <fstream>

namespace ArFac {
	using namespace ArEngine2D;

	class Editor
	{
	private:
		enum class Dir : unsigned char
		{
			Up = '^',
			Left = '<',
			Down = 'v',
			Right = '>',
		};

		struct BlockInfo
		{
			BlockInfo();
			BlockInfo(Block::ID id, Dir dir);

			Block::ID id;
			Dir dir;
		};

	private: // static variables
		constexpr static auto sc_LevelName{"FacLevel.txt"};
		constexpr static auto sc_BlockWidth{64.f};
		constexpr static auto sc_MinBlocksPerSide{8UI32};
		constexpr static auto sc_BackgroundColor{Colors::LightGray};
		constexpr static auto sc_LineColor{Colors::Gray};

	public:
		// borrows all the needed references for the editor to 
		// function almost stand-alone.
		Editor(Engine& game);

	public:

		// updates
		void UpdateInput(float dt);

		// graphics
		void Draw(Grafix& gfx);
		
		// saving and loading
		void LoadLevelFromFile(std::string_view fileName);
		void SaveLevelToFile(std::string_view fileName);

		// these assume that the width of the window is larger.
		// can clearly just pass in the height when that's not the case tho.
		float CalcMinZoom(float windowWidth) const;
		float CalcMaxZoom(float windowWidth) const;

		// getters
		DraggableCamera& Cam() noexcept;
		float LevelWidth() const noexcept;
		float LevelHeight() const noexcept;
		
		// these must be used even inside the class.
		static float BlockWidth() noexcept;


	private:
		void DrawUI(Grafix& gfx);
		void DrawGrid(Grafix& gfx, ColorF bgColor, ColorF strokeColor, float thick);

		static ColorF const& IDToColor(Block::ID id) noexcept;
		static char DirToChar(Dir dir) noexcept;
		static Dir CharToDir(unsigned char ch) noexcept;

		// grid
		Vec2 GridToScreen(std::size_t col, std::size_t row) const noexcept;
		DataGridCoord ScreenToGrid(Vec2 const& vec) const noexcept;
		Vec2 Snap(Vec2 const& location) const noexcept;

	private:
		void PlaceBlock(std::size_t col, std::size_t row) noexcept;
		void DeleteBlock(std::size_t col, std::size_t row) noexcept;

	private:
		// for the editor its self
		BlockInfo heldBlock_{Block::MinBlockID(), Dir::Right};
		DataGrid<BlockInfo> grid_{32, 32};
		DraggableCamera cam_;

		// borrowed directly from the eng
		Mouse& mouse_;
		Keyboard& keyboard_;
	};
}
