#include "data/models.hpp"
#include "menu.hpp"
#include "win/winapi.hpp"

// WinAPI macro blocks the keyword max()
#undef max

#include <set>
#include <numeric>

void Menu::Snapshot::remove() {

}

void Menu::Snapshot::edit() {

}

void Menu::Snapshot::add() {
	auto tmpSnapshot = new Models::Snapshot();
	STRING str;

	//Name Segment
	while (1) {
		try {
			AlphabeticalInput controlName(CHAR_TO_WSTR("Set Snapshot name : "), {});

			controlName.start();
			str = controlName.getInputValue();
			tmpSnapshot->setName(str);
			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	// Path to save files Segment
	while (1) {
		try {
			auto controlPath = PathInput(CHAR_TO_WSTR("Where is the save file? : "));

			controlPath.start();
			Filesystem::pathT path = controlPath.getInputValue();

			tmpSnapshot->setPathToFile(path);
			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	try {
		ptrManager->addSnapshot(*tmpSnapshot);
		COUT << "Snapshot " << tmpSnapshot->getData().nameSave << " Added to " << ptrManager->getCurrentGame().getData().nameGame << "." << std::endl;
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Snapshot::list() {
	try {
		if (!ptrManager->isGameLoaded()) {
			throw Exceptions::no_game_loaded("");
		}

		Models::MiniSnapshotMapT mapSnapshot = ptrManager->listSnapshots();
		std::list<STRING> nameSnapshots;

		for (auto& it : mapSnapshot) {
			nameSnapshots.push_back(it.second.nameSave);
		}

		WaitPromptInput controlWait(CHAR_TO_WSTR("== Snapshot lists (Enter to Continue...) =="), nameSnapshots);

		controlWait.start();
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Snapshot::select_game() {
	auto listGames = ptrManager->listGames();
	NumericalInput controlNum(CHAR_TO_WSTR("== Choose game to change snapshot status =="), listGames.first);
	STRING str;
	unsigned long long numChosen;

	while (1) {
		try {
			controlNum.start();
			str = controlNum.getInputValue();
			auto it = listGames.second.begin();

			numChosen = std::stoi(str);
			if (numChosen < 0 || numChosen > listGames.second.size())
				throw Exceptions::invalid_input("Unknown Option.");
			std::advance(it, numChosen - 1);

			ptrManager->loadGame(*it);
			COUT << "Game " << ptrManager->getCurrentGameName() << " is Loaded." << std::endl;
			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Menu::Flows::snapshot() {
	std::list<STRING> menuList = { 
		CHAR_TO_WSTR("Select game"), 
		CHAR_TO_WSTR("List snapshots"), 
		CHAR_TO_WSTR("Add snapshot"),
		CHAR_TO_WSTR("Edit Snapshot"),
		CHAR_TO_WSTR("Remove snapshot"),
		CHAR_TO_WSTR("Exit") 
	};

	STRING str;

	while (1) {
		try {
			STRING nameGame;
			try {
				nameGame = ptrManager->getCurrentGameName();
			}
			catch (...) {
				nameGame = CHAR_TO_WSTR("None");
			}

			NumericalInput control(CHAR_TO_WSTR("== Snapshot Menu ==\n Current Game : ") + nameGame, menuList);

			control.start();
			str = control.getInputValue();

			switch (std::stoi(str)) {
			case 1:
				Menu::Snapshot::select_game();
				break;
			case 2:
				Menu::Snapshot::list();
				break;
			case 3:
				Menu::Snapshot::add();
				break;
			case 4:
				Menu::Snapshot::edit();
				break;
			case 5:
				Menu::Snapshot::remove();
				break;
			case 6:
				return;
			default:
				throw Exceptions::invalid_input("Unknown option");
			}
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Menu::Game::remove() {
	auto listGames = ptrManager->listGames();
	STRING str;

	if (listGames.first.empty()) {
		WaitPromptInput controlWait(CHAR_TO_WSTR("== No games to shown =="));
		controlWait.start();
		return;
	}

	while (1) {
		NumericalInput controlNum(CHAR_TO_WSTR("Choose game to remove : "), listGames.first);
		try {
			controlNum.start();
			str = controlNum.getInputValue();
			auto numChosen = std::stoi(str);

			auto itName = listGames.first.begin();
			auto itPath = listGames.second.begin();

			std::advance(itPath, numChosen - 1);
			std::advance(itName, numChosen - 1);

			ptrManager->removeGame(*itPath);
			COUT << *itName << " has removed from manager." << std::endl;

			return;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Menu::Game::list() {
	auto listGames = ptrManager->listGames();
	WaitPromptInput controlWait(CHAR_TO_WSTR("== Added games in manager (Enter to continue...) =="), listGames.first);

	try {
		controlWait.start();
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Game::add() {
	AlphabeticalInput controlGameName(CHAR_TO_WSTR("Set game name : "), {});
	auto tmpGame = new Models::Game();
	STRING str;

	//Name Segment
	while (1) {
		try {
			controlGameName.start();
			str = controlGameName.getInputValue();
			tmpGame->setName(str);
			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	// Choosing process segment
	while (1) {
		try {
			WinAPI::Process::Types::mapProcessList listProc;
			WinAPI::Process::getProcesses(listProc);
			auto controlProcess = NumericalInputMapFormat(CHAR_TO_WSTR("Select game process name : "), listProc);

			controlProcess.start();
			str = controlProcess.getStringValue();
			tmpGame->setProcessName(str);

			Filesystem::pathT pathGame;
			WinAPI::Process::getPathProcess(pathGame, std::stoi(controlProcess.getInputValue()));
			tmpGame->setPathToGame(pathGame);

			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	// Path to save files Segment
	while (1) {
		try {
			auto controlPath = PathInput(CHAR_TO_WSTR("Set the path to save files stored : "));

			controlPath.start();
			str = controlPath.getInputValue();
			tmpGame->setPathToSaveFiles(str);
			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	// Extensions of save file Segment
	while (1) {
		try {
			std::set<Filesystem::pathT> setPaths;
			std::list<STRING> listExtensions;

			// For picking up the unique values
			for (auto& it : std::filesystem::directory_iterator(tmpGame->getData().pathSave))
				if(!it.path().extension().empty()) setPaths.insert(it.path().extension());
			for (auto& it : setPaths)
				listExtensions.push_back(it);

			auto controlExtension = NumericalInput(CHAR_TO_WSTR("What is the extension of save files? : "), listExtensions);
			controlExtension.start();
			str = controlExtension.getInputValue();

			auto it = listExtensions.begin();
			std::advance(it, std::stoi(str) - 1);

			tmpGame->setExtensionOfSave(*it);
			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	try {
		ptrManager->createGame(*tmpGame);
		COUT << "Game " << tmpGame->getData().nameGame << " Added." << std::endl;
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Flows::game() {
	std::list<STRING> menuList = { 
		CHAR_TO_WSTR("List games"), 
		CHAR_TO_WSTR("Add game"), 
		CHAR_TO_WSTR("Remove game"), 
		CHAR_TO_WSTR("Exit") 
	};

	NumericalInput control(CHAR_TO_WSTR("== Game Menu =="), menuList);
	STRING str;

	while (1) {
		try {
			control.start();
			str = control.getInputValue();

			switch (std::stoi(str)) {
			case 1:
				Menu::Game::list();
				break;
			case 2:
				Menu::Game::add();
				break;
			case 3:
				Menu::Game::remove();
				break;
			case 4:
				return;
			default:
				throw Exceptions::invalid_input("Unknown option");
			}
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

int main(void) {
	ptrManager = std::make_unique<Manager::Manager>();
	std::list<STRING> menuList = { 
		CHAR_TO_WSTR("Game"), 
		CHAR_TO_WSTR("Snapshot"), 
		CHAR_TO_WSTR("Settings"), 
		CHAR_TO_WSTR("Exit") 
	};

	Menu::NumericalInput control(CHAR_TO_WSTR("== Main Menu =="), menuList);
	STRING str;
	bool isExit = false;

	while (!isExit) {
		try {
			control.start();
			str = control.getInputValue();

			switch (std::stoi(str)) {
			case 1:
				Menu::Flows::game();
				break;
			case 2:
				Menu::Flows::snapshot();
				break;
			case 3:
				// GOTO SETTINGS
				break;
			case 4:
				isExit = true;
				break;
			default:
				throw Exceptions::invalid_input("Unknown option");
			}
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	return 0;
}

void Menu::AlphabeticalInput::_determineError() {
	if (!ISALPHA(input)) 
		throw Exceptions::invalid_input("Only Alphabetical value is allowed.");
	if (!ISFILENAMEAVAILABLE(input))
		throw Exceptions::invalid_input("Game name includes invalid character(s).");
}

void Menu::BaseInputFormat::_getInput()
{
	try {
		CIN.ignore(CIN.rdbuf()->in_avail());
		std::getline(CIN, input);
		_determineError();
		CIN.clear();
	}
	catch (const std::ios::failure& e) {
		throw Exceptions::Exception(e);
	}
	catch (const Exceptions::Exception& e) {
		throw e;
	}
}

void Menu::BaseInputFormat::start()
{
	_print();
	_getInput();
}

const STRING Menu::BaseInputFormat::getInputValue() const
{
	return this->input;
}

void Menu::PathInput::_determineError()
{
	if (!std::filesystem::exists(this->input))
		throw Exceptions::invalid_input("Directory does not exists.");
}
