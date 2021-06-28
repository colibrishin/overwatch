#include <filesystem>
#include <iostream>
#include <tuple>
#include <list>
#include <utility>
#include <unordered_set>

#include "database.hpp"
#include "overwatch.hpp"
#include "menu.hpp"

std::unique_ptr<Data::Models::Game> init(std::unique_ptr<Data::Models::Game> ptrGame) {
	Database::Filesystem f;

	std::cout << "NOTICE : Checking necessary folders..." << std::endl;
	f = Database::Filesystem(Data::Constants::EXCLUDE_FOLDER, Database::Enums::FILE_MODE::WRITE_MODE);
	f = Database::Filesystem(Data::Constants::SNAPSHOT_FOLDER, Database::Enums::FILE_MODE::WRITE_MODE);
	f = Database::Filesystem(Data::Constants::GAME_FOLDER, Database::Enums::FILE_MODE::WRITE_MODE);

	std::cout << "NOTICE : Database Enumerate for Self-fix Started" << std::endl;
	for (auto& it : Data::Utility::getGameDirectoryIterator()) {
		if (it.path().extension() == Data::Constants::DATA_FILE_EXTENSION) {
			ptrGame->loadGame(it.path().stem());
			COUT << "NOTICE : Game " << ptrGame->getName() << " Loaded" << std::endl;
			for (auto it = ptrGame->accessMap().begin(); it != ptrGame->accessMap().end(); ++it){
				ptrGame->loadSnapshot(it);
			}
		}
	}
	return std::move(ptrGame);
}

int main(void) {
	std::unique_ptr<Data::Models::Game> g(new Data::Models::Game);
	Menu::Enums::ReturnMenu rtn;
	Data::Types::ctxGame tmpGame;

	std::cout << Overwatch::Constants::PRODUCT_NAME << " " << Overwatch::Constants::PRODUCT_VERSION << std::endl;
	std::cout << "NOTICE : Initialisation Started" << std::endl;
	
	g = init(std::move(g));

	g = Menu::Main::main(rtn, std::move(g));

	if (rtn.getValue() == Exception::Constants::RTN_EXIT_SUCCESS); // Save and quit
	else return 0; // something else?

	std::cout << Menu::Constants::STR_TIP_ENTER_CONTINUE;
	std::cin.ignore(1);
	std::getchar();
}