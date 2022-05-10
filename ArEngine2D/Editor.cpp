#include "Editor.h"

#include <random>

namespace ArFac {
	Editor::BlockInfo::BlockInfo() :
		id{Block::TypeToID(Block::Type::Floor)},
		dir{Dir::Right}
	{}

	Editor::BlockInfo::BlockInfo(Block::ID id, Dir dir)
		: id{id}, dir{dir}
	{}

	Editor::Editor(Engine& game) : 
		mouse_{game.mouse}, keyboard_{game.keyboard}, 
		cam_{game.mouse}
	{ }

	void Editor::UpdateInput(float dt)
	{
		(void)dt;
		if (auto const coord{ScreenToGrid(cam_[mouse_.loc])};
			grid_.IsInside(coord))
		{
			if (mouse_.left.IsPressed())
			{
				PlaceBlock(coord.x, coord.y);
			}
			else if (mouse_.right.IsPressed())
			{
				DeleteBlock(coord.x, coord.y);
			}
		}
		if (mouse_.mid.IsPressed())
		{
			cam_.SetZoom(1.f);
		}
		if (keyboard_(Keys::Control).IsDown() and
			keyboard_(Keys::S).IsPressed())
		{
			SaveLevelToFile(sc_LevelName);
		}
		if (keyboard_(Keys::Control).IsDown() and
			keyboard_(Keys::O).IsPressed())
		{
			LoadLevelFromFile(sc_LevelName);
		}

		if (auto const maxID{Block::MaxBlockID()},
			minID{Block::MinBlockID()};
			keyboard_(Keys::Sub).IsPressed())
		{
			if (heldBlock_.id == maxID)
			{
				heldBlock_.id = minID;
			}
			else
			{
				heldBlock_.id += 1;
			}
		}
		else if (keyboard_(Keys::Add).IsPressed())
		{
			if (heldBlock_.id == minID)
			{
				heldBlock_.id = maxID;
			}
			else
			{
				heldBlock_.id -= 1;
			}
		}
	}

	void Editor::Draw(Grafix& gfx)
	{
		DrawGrid(gfx, sc_BackgroundColor, sc_LineColor, 1.f);
		DrawUI(gfx);
	}

	void Editor::DrawUI(Grafix& gfx)
	{
		gfx.FillRectangle(mouse_.loc - Vec2{-30.f, 30.f}, 50.f, 50.f, IDToColor(heldBlock_.id));
		gfx.DrawString(mouse_.loc - Vec2{-30.f, 50.f}, 
			std::string{Block::TypeToString(Block::IDToType(heldBlock_.id))}, Colors::White, 20.f);
	}

	void Editor::DrawGrid(Grafix& gfx, ColorF bgColor, ColorF strokeColor, float thick)
	{
		gfx.PushTransform(cam_.CurrTransform());
		
		auto const camExtents{cam_.CalcExtents()};
		
		// this assumes that the grid always covers the entire screen.
		auto const topLeft{ScreenToGrid(camExtents.topLeft)};
		auto const tempBotRight{ScreenToGrid(camExtents.botRight)};
		decltype(tempBotRight) botRight{
			std::min(tempBotRight.x + 1, grid_.Width()), 
			std::min(tempBotRight.y + 1, grid_.Height() - 1)
		};

		// better the two dimensional loop
		for (std::size_t i{grid_.CoordToIndex(topLeft)}, lim{grid_.CoordToIndex(botRight)};
			i < lim; ++i)
		{
			auto const coord{grid_.IndexToCoord(i)};
			auto const vec{GridToScreen(coord.x, coord.y)};
			auto const w{BlockWidth()};
			auto const& el{grid_[i]};
			auto const& actCol{IDToColor(el.id)};
			gfx.FillRectangle(vec, w, w, actCol);

			auto const hw = w * 0.5f;
			auto const qw = w * 0.25f;
			switch (el.dir)
			{
			case Dir::Right:
				gfx.DrawArrow({vec.x + qw, vec.y + hw}, {vec.x + hw + qw, vec.y + hw}, sc_LineColor);
				break;
			case Dir::Down:
				gfx.DrawArrow({vec.x + hw, vec.y + qw}, {vec.x + hw, vec.y + hw + qw}, sc_LineColor);
				break;
			case Dir::Left:
				gfx.DrawArrow({vec.x + hw + qw, vec.y + hw}, {vec.x + qw, vec.y + hw}, sc_LineColor);
				break;
			case Dir::Up:
				gfx.DrawArrow({vec.x + hw, vec.y + hw + qw}, {vec.x + hw, vec.y + qw}, sc_LineColor);
				break;
			default: throw;
			}

			gfx.DrawRectangle(vec, w, w, strokeColor, thick);
		}
		gfx.PopTransform();
	}

	void Editor::LoadLevelFromFile(std::string_view fileName)
	{
		std::ifstream file{std::string{fileName}};
		for (std::size_t i{ }, lim{grid_.Height()}; i < lim; ++i)
		{
			std::string line{ };
			std::getline(file, line);
			assert(line.size() == grid_.Width() * 2);
			for (std::size_t j{ }; j < grid_.Width(); ++j)
			{
				grid_(j, i) = {
					Block::CharToID(line[2 * j]),
					CharToDir(line[2 * j + 1]),
				};
			}
		}
	}

	void Editor::SaveLevelToFile(std::string_view fileName)
	{
		std::ofstream file{std::string{fileName}};
		for (std::size_t i{ }; i < grid_.Height(); ++i)
		{
			std::string line{ };
			line.reserve(grid_.Width() * 2);
			for (std::size_t j{ }; j < grid_.Width(); ++j)
			{
				auto const& el{grid_(j, i)};
				line += Block::IDToChar(el.id);
				line += DirToChar(el.dir);
			}
			file << line + '\n';
		}
	}

	DraggableCamera& Editor::Cam() noexcept
	{ return cam_; }

	float Editor::BlockWidth() noexcept
	{ return sc_BlockWidth; }

	float Editor::LevelWidth() const noexcept
	{ return grid_.Width() * BlockWidth(); }

	float Editor::LevelHeight() const noexcept
	{ return grid_.Height() * BlockWidth(); }

	float Editor::CalcMinZoom(float windowWidth) const
	{
		auto const gw{grid_.Width()};
		auto const gh{grid_.Height()};
		auto const bw{BlockWidth()};
		
		auto const absoluteMin = [&] {
			if (gw < gh) return windowWidth / (bw * gw);
			else		 return windowWidth / (bw * gh);
		}();
		return std::max(absoluteMin, CalcMaxZoom(windowWidth) * 0.5f);
	}

	float Editor::CalcMaxZoom(float windowWidth) const
	{
		auto const bw{BlockWidth()};
		return windowWidth / (bw * sc_MinBlocksPerSide);
	}

	ColorF const& Editor::IDToColor(Block::ID id) noexcept
	{
		using MyLUT = std::unordered_map<Block::Type, ColorF> const;
		static MyLUT sc_LUT{
			{Block::Type::Floor, Colors::LightGray},
			{Block::Type::Belt, Colors::Red},
			{Block::Type::Welder, Colors::Yellow},
			{Block::Type::Pusher, Colors::DarkCyan},
			{Block::Type::Sensor, Colors::Orange},
			{Block::Type::Wire, Colors::Black},
			{Block::Type::Producer, Colors::Magenta},
		};

		auto const type{Block::IDToType(id)};
		assert(sc_LUT.contains(type));
		return sc_LUT.find(type)->second;
	}

	char Editor::DirToChar(Dir dir) noexcept
	{
		assert(dir == Dir::Up   or dir == Dir::Left or
				dir == Dir::Down or dir == Dir::Right); 
		return static_cast<char>(dir);
	}

	Editor::Dir Editor::CharToDir(unsigned char ch) noexcept
	{
		assert(ch == DirToChar(Dir::Up)   or ch == DirToChar(Dir::Left) or
				ch == DirToChar(Dir::Down) or ch == DirToChar(Dir::Right));
		return static_cast<Dir>(ch);
	}

	void Editor::PlaceBlock(std::size_t col, std::size_t row) noexcept
	{
		static std::mt19937 s_Engine{std::random_device{}()};
		static std::uniform_int_distribution s_UniDest{0, 3};

		grid_(col, row) = heldBlock_;
		heldBlock_.dir = [&] {switch (s_UniDest(s_Engine)) {
			case 0: return Dir::Right;
			case 1: return Dir::Down;
			case 2: return Dir::Left;
			case 3: return Dir::Up;
			default: throw;
		}}();
	}

	void Editor::DeleteBlock(std::size_t col, std::size_t row) noexcept
	{
		grid_(col, row) = {Block::TypeToID(Block::Type::Floor), Dir::Right};
	}
	
	Vec2 Editor::GridToScreen(std::size_t col, std::size_t row) const noexcept
	{
		auto const w{BlockWidth()};
		return {col * w, row * w};
	}

	DataGridCoord Editor::ScreenToGrid(Vec2 const& vec) const noexcept
	{
		auto const w{BlockWidth()};
		return {
			static_cast<std::size_t>(vec.x / w),
			static_cast<std::size_t>(vec.y / w)
		};
	}

	Vec2 Editor::Snap(Vec2 const& location) const noexcept
	{
		auto const w{BlockWidth()};
		auto const [sx, sy] {ScreenToGrid(location)};
		return {sx * w, sy * w};
	}
}