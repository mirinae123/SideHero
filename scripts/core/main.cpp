#include "core.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {
	bool result = Core::Start();

	if (result) {
		Core::Run();
	}

	Core::End();
}