#include "Block.h"

namespace ArFac {
	std::vector<Block::BlockInfo> const Block::sc_Blocks{
		BlockInfo{Type::Floor, "Floor", '0', 0},
		BlockInfo{Type::Belt, "Belt", 'C', 1},
		BlockInfo{Type::Welder, "Welder", 'W', 2},
		BlockInfo{Type::Pusher, "Pusher", 'P', 3},
		BlockInfo{Type::Sensor, "Sensor", 'S', 4},
		BlockInfo{Type::Wire, "Wire", '=', 5},
		BlockInfo{Type::Producer, "Producer", '#', 6},
	};

	Block::ID Block::CharToID(char ch)
	{ return FindHelper(ch, &BlockInfo::ch).id; }

	char Block::IDToChar(Block::ID id)
	{ return FindHelper(id, &BlockInfo::id).ch; }

	Block::ID Block::TypeToID(Type type)
	{ return FindHelper(type, &BlockInfo::type).id; }

	Block::Type Block::IDToType(Block::ID id)
	{ return FindHelper(id, &BlockInfo::id).type; }

	std::string_view Block::TypeToString(Type type)
	{ return FindHelper(type, &BlockInfo::type).str; }

	Block::Type Block::StringToType(std::string_view str)
	{ return FindHelper(str, &BlockInfo::str).type; }

	Block::ID Block::MinBlockID() noexcept
	{ return static_cast<Block::ID>(1ULL); }
	Block::ID Block::MaxBlockID() noexcept
	{ return static_cast<Block::ID>(sc_Blocks.size() - 1); }
}

