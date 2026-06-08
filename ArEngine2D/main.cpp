#include "GuiGame.h"

#include <fstream>
#include <filesystem>
#include <execution>
#include <iostream>

#include "Random.h"
#include "GuiGame.h"


//INT WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT)
int main()
{
	ArGui::GuiGame engine{"my eng", 1280, 720};
	engine.Run();
	return 0;
}