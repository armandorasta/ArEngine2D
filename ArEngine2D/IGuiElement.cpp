#include "IGuiElement.h"

#include "Camera.h"

namespace ArGui {
	void IGuiElement::Draw(Grafix& gfx)
	{ Draw(gfx, Camera::CameraAtDefaultPosition()); }
}
