#include "TeditGame.h"

#include <fstream>
#include <filesystem>

INT WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
{
	ARTED_ TeditGame engine{"my engine", 1280, 720};
	engine.Run();
	return 0;
}