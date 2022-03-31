#include "Engine.h"
#include <stdexcept>

class MyEngine : public ArEngine2D::Engine
{
private:
	using self = MyEngine;
	using base = Engine;

public:

	using base::base;

public:

	void OnUserCreate() override
	{

	}

	void OnUserUpdate(float dt) override
	{

	}
};

INT APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT nCmdShow)
{
	try
	{
		MyEngine window{"shit", 800, 600};
		window.Run();
	}
	catch (const std::exception&)
	{

	}

	return 0;
}