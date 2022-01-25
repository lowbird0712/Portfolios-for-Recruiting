#include "Singleton/Core.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow) {

	if (!GET_SINGLE(Core)->Init(hInstance)) {
		DESTROY_SINGLE(Core);
		return 0;
	}

	int return_value = GET_SINGLE(Core)->Run();

	DESTROY_SINGLE(Core);

	return return_value;
}