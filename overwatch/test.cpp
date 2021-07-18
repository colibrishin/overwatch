#include "test.hpp"
#include "macros/utility.hpp"
#include "data/models.hpp"
#include "data/loader.hpp"
#include <cstdlib>

void Test::CreateDummyGame(Manager::Manager& manager) {
	Models::Game* game;
	game = new Models::Game(L"Dummy", L"Dummy.exe", L"C:\\", L"C:\\save", L".sav");

	manager.createGame(*game);
	delete game;
	system("pause");
}