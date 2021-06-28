#include <iostream>
#include <set>

#include "menu.hpp"
#include "overwatch.hpp"
#include "winapi.hpp"
#include "utility.hpp"

std::unique_ptr<Data::Models::Game> Menu::Game::edit(Menu::Enums::ReturnMenu rtn, std::unique_ptr<Data::Models::Game> g) {
	std::list<std::pair<STRING, STRING>> lstGame;
	unsigned long long list_size = 0;
	while (1) {
		list_size = 0;
		for (auto& it : Data::Utility::getGameDirectoryIterator())
			if (it.path().extension() == Data::Constants::DATA_FILE_EXTENSION)
				++list_size;

		lstGame.resize(list_size);
		auto pos = lstGame.begin();

		std::cout << "== Choose Game to edit ==" << std::endl;
		for (auto& it : Data::Utility::getGameDirectoryIterator()) {
			if (it.path().extension() == Data::Constants::DATA_FILE_EXTENSION) {
				g->loadGame(it.path().stem());
				if (g->good()) {
					*pos = std::make_pair(g->getName(), g->getFilePath().stem().wstring());
					COUT << std::distance(lstGame.begin(), pos) + 1 << L". " << g->getName() << std::endl;
					++pos;
				}
			}
		}
		std::cout << "q. Exit to Game Menu" << std::endl;
		Menu::Types::MenuInputT input(list_size, L"q");

		input.input();
		if (input.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (input.getStatus() & Menu::Enums::TMenuTReturns::QUIT) {
			rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			return std::move(g);
		}
		if ((input.getStatus() & Menu::Enums::TMenuTReturns::FAIL) |
			(input.getStatus() & Menu::Enums::TMenuTReturns::STR))
			std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
		else if (input.getStatus() & Menu::Enums::TMenuTReturns::INT) {
			pos = lstGame.begin();
			std::advance(pos, input.getInt() - 1);
			// DETAIL EDIT MENU
		}
	}
	rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
	return std::move(g);
}

Menu::Enums::ReturnMenu Menu::Game::MenusAdd::setNameInstance(Menu::Types::MenuInputT& io, STRING& nameInstance) {
	while (1) {
		std::cout << "Set Game Name (Max " << Database::Constants::INSTANCE_MAX_NAME - 1 << " characters, b to go back) : ";
		io.input();

		if (io.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::QUIT)
			return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
		else {
			if ((io.getStatus() & Menu::Enums::TMenuTReturns::STR) &
				(io.getStatus() & Menu::Enums::TMenuTReturns::FAIL))
				std::cout << "ERROR : This cannot be blank" << std::endl;
			else {
				nameInstance = io.getString();
				return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			}
		}
	}
	return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
}

// TODO : PROCESS NEEDED
Menu::Enums::ReturnMenu Menu::Game::MenusAdd::setNameProc(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG) {
	WinAPI::Process::Enums::ReturnCode rtnCode;
	WinAPI::Process::Types::mapProcessList mapPlist;
	std::filesystem::path pathProc;
	auto target = mapPlist.end();

	while (1) {
		mapPlist.clear();
		rtnCode = WinAPI::Process::getProcesses(mapPlist);

		if (rtnCode.getValue() & Exception::Constants::RTN_EXIT_FAILED) {
			std::cout << WinAPI::Process::Utility::getErrorMessage(rtnCode.getValue()) << std::endl;
			return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
		}

		std::cout << "Select Game Executable (Press enter if the game is not running) : " << std::endl;
		std::cout << "=======" << std::endl;
		for (auto& it : mapPlist) {
			std::cout << it.first << ". ";
			COUT << it.second << std::endl;
		}
		std::cout << "=======" << std::endl;
		io.input();

		if (io.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::STR)
			std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::QUIT)
			return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
		else if (io.getStatus() & Menu::Enums::TMenuTReturns::INT) {
			if (io.getStatus() & Menu::Enums::TMenuTReturns::FAIL)
				return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			else if ((target = mapPlist.find(io.getInt())) == mapPlist.end())
				std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
			else {
				rtnCode = WinAPI::Process::getPathProcess(pathProc, target->first);
				if (rtnCode.getValue() & Exception::Constants::RTN_EXIT_FAILED)
					std::cout << "ERROR : Unable to find the process" << std::endl;
				else {
					UT_NULLbreakCopy(ctxG.nameProc, target->second.c_str(), sizeof(ctxG.nameProc));
					UT_NULLbreakCopy(ctxG.pathGame, pathProc.remove_filename().c_str(), sizeof(ctxG.pathGame));
					return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
				}
			}
		}
	}
	
	return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
}

Menu::Enums::ReturnMenu Menu::Game::MenusAdd::setPathGame(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG) {
	while (1) {
		std::cout << "Set Game Executable Path : ";
		io.input();

		if (io.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::INT)
			std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::QUIT)
			return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
		else if (io.getStatus() & Menu::Enums::TMenuTReturns::STR) {
			if (io.getStatus() & Menu::Enums::TMenuTReturns::FAIL)
				std::cout << Menu::Constants::STR_ERR_BLANK_NOT_ALLOWED << std::endl;
			else if (!std::filesystem::exists(io.getString()))
				std::cout << "ERROR : Unable to find the file" << std::endl;
			else if (!std::filesystem::is_regular_file(io.getString()))
				std::cout << "ERROR : Target must be a file" << std::endl;
			else {
				UT_memcpy(ctxG.pathGame, io.getString().c_str(), sizeof(ctxG.pathGame));
				return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			}
		}
	}

	return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
}

Menu::Enums::ReturnMenu Menu::Game::MenusAdd::setPathSave(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG) {
	while (1) {
		std::cout << "Set Save file Path (Press enter if game folder matches with save folder) : ";
		io.input();

		if (io.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::INT)
			std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::QUIT)
			return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
		else if (io.getStatus() & Menu::Enums::TMenuTReturns::STR) {
			if (io.getStatus() & Menu::Enums::TMenuTReturns::FAIL) {
				std::filesystem::path tmp = ctxG.pathGame;
				UT_NULLbreakCopy(ctxG.pathSave, ctxG.pathGame, sizeof(ctxG.pathSave));
				return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			}
			if (!std::filesystem::exists(io.getString()))
				std::cout << "ERROR : Unable to find the directory" << std::endl;
			else if (!std::filesystem::is_directory(io.getString()))
				std::cout << "ERROR : Target must be a directory" << std::endl;
			else {
				UT_NULLbreakCopy(ctxG.nameProc, std::filesystem::path(io.getString()).remove_filename().c_str(), sizeof(ctxG.nameProc));
				UT_NULLbreakCopy(ctxG.pathGame, io.getString().c_str(), sizeof(ctxG.pathGame));
				return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			}
		}
	}
	return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
}

Menu::Enums::ReturnMenu Menu::Game::MenusAdd::setStrExtension(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG) {
	std::set<STRING> extensions;

	while (1) {
		std::cout << "What is the save file extension? " << std::endl;
		std::cout << "=======" << std::endl;

		extensions.clear();
		for (auto& it : std::filesystem::directory_iterator(ctxG.pathSave)) {
			if (it.is_regular_file()) {
				// TODO : Exclude Needed (EXE, JPG, MP4 etc...)
				extensions.insert(it.path().extension().wstring());
			}
		}

		for (auto& it : extensions)
			COUT << it << std::endl;
		std::cout << "=======" << std::endl;
		io.input();

		if (io.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::INT)
			std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
		if (io.getStatus() & Menu::Enums::TMenuTReturns::QUIT) {
			return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
		}
		else if (io.getStatus() & Menu::Enums::TMenuTReturns::STR) {
			if (io.getStatus() & Menu::Enums::TMenuTReturns::FAIL)
				std::cout << Menu::Constants::STR_ERR_BLANK_NOT_ALLOWED << std::endl;
			else if (extensions.find(io.getString()) == extensions.end())
				std::cout << "ERROR : Choose from options" << std::endl;
			else {
				UT_NULLbreakCopy(ctxG.strExtension, io.getString().c_str(), sizeof(ctxG.strExtension));
				return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			}
		}
	}
	return Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
}

std::unique_ptr<Data::Models::Game> Menu::Game::add(Menu::Enums::ReturnMenu& rtn, std::unique_ptr<Data::Models::Game> g) {
	while (1) {
		STRING nameGame;
		Menu::Types::MenuInputT input(0, L"b");
		Data::Types::ctxGame tmpGame;
		short stage = 0;
		bool stage_seq = true;

		while (stage_seq) {
			if (stage < 0) {
				rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
				return std::move(g);
			}
			switch (stage) {
			case 0:
				rtn = Menu::Game::MenusAdd::setNameInstance(input, nameGame);
				break;
			case 1:
				rtn = Menu::Game::MenusAdd::setNameProc(input, tmpGame);
				break;
			case 2:
				if (UT_strempty(tmpGame.pathGame) || UT_strempty(tmpGame.nameProc))
					rtn = Menu::Game::MenusAdd::setPathGame(input, tmpGame);
				else rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
				break;
			case 3:
				rtn = Menu::Game::MenusAdd::setPathSave(input, tmpGame);
				break;
			case 4:
				rtn = Menu::Game::MenusAdd::setStrExtension(input, tmpGame);
				break;
			default:
				stage_seq = false;
				break;
			}

			// TODO
			if (!(rtn.getValue() & Exception::Constants::RTN_EXIT_FAILED)) ++stage;
			else --stage;
		}

		g->createGame(tmpGame, nameGame.c_str());
		if (g->good()) {
			COUT << "NOTICE : Game " << g->getName() << " Created." << std::endl;
			rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			return std::move(g);
		}

		else {
			std::cout << "ERROR : Could not create a metadata" << std::endl;
			rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
			return std::move(g);
		}
	}
}

std::unique_ptr<Data::Models::Game> Menu::Game::list(Menu::Enums::ReturnMenu& rtn, std::unique_ptr<Data::Models::Game> g) {
	unsigned long long cnt = 0;
	for (auto& it : Data::Utility::getGameDirectoryIterator()) {
		if (it.path().extension() == Data::Constants::DATA_FILE_EXTENSION) {
			g->loadGame(it.path().stem());
			if (g->good()) COUT << ++cnt << L". " << g->getName() << std::endl;
		}
	}
	std::cout << Menu::Constants::STR_TIP_ENTER_CONTINUE;
	std::cin.ignore(1);
	std::getchar();

	rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
	return std::move(g);
}

std::unique_ptr<Data::Models::Game> Menu::Game::main(Menu::Enums::ReturnMenu& rtn, std::unique_ptr<Data::Models::Game> g) {
	Menu::Types::MenuInputT input(4, L"q");

	while (1) {
		std::cout << "== Main Menu ==" << std::endl;
		std::cout << "1. List Game" << std::endl;
		std::cout << "2. Add Game" << std::endl;
		std::cout << "3. Edit Game" << std::endl;
		std::cout << "4. Remove Game" << std::endl;
		std::cout << "q. Back to Main Menu" << std::endl;
		std::cout << "===============" << std::endl;

		input.input();
		if (input.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (input.getStatus() & Menu::Enums::TMenuTReturns::QUIT) {
			rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			break;
		}
		if ((input.getStatus() & Menu::Enums::TMenuTReturns::FAIL) |
			(input.getStatus() & Menu::Enums::TMenuTReturns::STR))
			std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
		else if (input.getStatus() & Menu::Enums::TMenuTReturns::INT) {
			switch (Menu::Utility::numToBitmask(input.getInt())) {
			case Menu::Constants::Returns::GAME_OPT_LIST:
				g = Menu::Game::list(rtn, std::move(g));
				break;
			case Menu::Constants::Returns::GAME_OPT_ADD:
				g = Menu::Game::add(rtn, std::move(g));
				break;
			case Menu::Constants::Returns::GAME_OPT_EDIT:
				g = Menu::Game::edit(rtn, std::move(g));
				break;
			case Menu::Constants::Returns::GAME_OPT_REM:
				// REMOVE FUNCTION
			default:
				rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
				return std::move(g);
			}
		}
	}

	rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
	return std::move(g);
}

std::unique_ptr<Data::Models::Game> Menu::Main::main(Menu::Enums::ReturnMenu& status, std::unique_ptr<Data::Models::Game> g) {
	Menu::Types::MenuInputT input(3, L"q");
	Menu::Enums::ReturnMenu rtn(Exception::Constants::RTN_EXIT_FAILED);

	while (1) {
		std::cout << "== Main Menu ==" << std::endl;
		std::cout << "1. Game Menu" << std::endl;
		std::cout << "2. Snapshot Menu" << std::endl;
		std::cout << "3. Setting Menu" << std::endl;
		std::cout << "q. Exit" << std::endl;
		std::cout << "===============" << std::endl;

		input.input();
		if (input.getStatus() == Menu::Enums::TMenuTReturns::NIL)
			std::cout << Menu::Constants::STR_ERR_SYS_INPUT << std::endl;
		if (input.getStatus() & Menu::Enums::TMenuTReturns::QUIT) {
			status = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_SUCCESS);
			break;
		}
		if ((input.getStatus() & Menu::Enums::TMenuTReturns::FAIL) |
			(input.getStatus() & Menu::Enums::TMenuTReturns::STR))
			std::cout << Menu::Constants::STR_ERR_UNKNOWN_OPTION << std::endl;
		else if (input.getStatus() & Menu::Enums::TMenuTReturns::INT) {
			switch (Menu::Utility::numToBitmask(input.getInt())) {
			case Menu::Constants::Returns::MAIN_OPT_GAME:
				g = Menu::Game::main(rtn, std::move(g));
				break;
			case Menu::Constants::Returns::MAIN_OPT_SNAPSHOT:
				// Snapshot Menu Function
			case Menu::Constants::Returns::MAIN_OPT_SETTINGS:
				// Setting Menu Function
			default:
				rtn = Menu::Enums::ReturnMenu(Exception::Constants::RTN_EXIT_FAILED);
				return std::move(g);
			}
		}
	}

	return std::move(g);
}

unsigned long long Menu::Utility::numToBitmask(unsigned long long n) {
	unsigned long long rtn = 1;
	return rtn << n;
}