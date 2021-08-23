#include "data/models.hpp"
#include "menu.hpp"
#include "win/winapi.hpp"

// WinAPI macro blocks the keyword max()
#undef max

#include <set>
#include <numeric>

void Menu::Snapshot::list() {
	auto listPaths = ptrManager->iterateGameData();
	auto listGames = ptrManager->listGames(listPaths);
	WaitPromptInput controlWait(CHAR_TO_WSTR("== Snapshot lists (Enter to Continue...) =="), listGames);

	if (!ptrManager->isGameLoaded()) {
		std::cout << Exceptions::no_game_loaded("").what() << std::endl;
		return;
	}

	try {
		controlWait.start();
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Snapshot::select_game() {
	auto listPaths = ptrManager->iterateGameData();
	auto listGames = ptrManager->listGames(listPaths);
	NumericalInput controlNum(CHAR_TO_WSTR("== Choose game to change snapshot status =="), listGames);
	STRING str;
	unsigned long long numChosen;

	while (1) {
		try {
			controlNum.start();
			str = controlNum.getInputValue();
			auto it = listPaths.begin();

			numChosen = std::stoi(str);
			if (numChosen < 0 || numChosen > listPaths.size())
				throw Exceptions::invalid_input("Unknown Option.");
			std::advance(it, numChosen + 1);

			ptrManager->loadGame(*it);
			COUT << "Game " << ptrManager->getCurrentGameName() << " is Loaded." << std::endl;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Menu::Flows::snapshot() {
	std::list<STRING> menuList = { CHAR_TO_WSTR("Select game"), CHAR_TO_WSTR("List snapshots"), CHAR_TO_WSTR("Add snapshot"), CHAR_TO_WSTR("Remove snapshot"), CHAR_TO_WSTR("Exit") };
	STRING str;

	while (1) {
		try {
			STRING nameGame = CHAR_TO_WSTR("None");
			try {
				STRING nameGame = ptrManager->getCurrentGameName();
			}
			catch (...) {}

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
				//Menu::Snapshot::add();
				break;
			case 4:
				//Menu::Snapshot::remove();
				break;
			case 5:
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
	auto listPaths = ptrManager->iterateGameData();
	auto listGames = ptrManager->listGames(listPaths);
	STRING str;

	if (listGames.empty()) {
		WaitPromptInput controlWait(CHAR_TO_WSTR("== No games to shown =="));
		controlWait.start();
		return;
	}

	while (1) {
		NumericalInput controlNum(CHAR_TO_WSTR("Choose game to remove : "), listGames);
		try {
			controlNum.start();
			str = controlNum.getInputValue();
			auto numChosen = std::stoi(str);

			auto it = listPaths.begin();
			if (numChosen < 0 || numChosen > listPaths.size())
				throw Exceptions::invalid_input("Unknown Option.");
			std::advance(it, numChosen - 1);

			STRING nameGame = ptrManager->getCurrentGameName();
			ptrManager->removeGame(*it);
			COUT << nameGame << " has removed from manager." << std::endl;

			return;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

void Menu::Game::list() {
	auto listPaths = ptrManager->iterateGameData();
	auto listGames = ptrManager->listGames(listPaths);
	WaitPromptInput controlWait(CHAR_TO_WSTR("== Added games in manager (Enter to continue...) =="), listGames);

	try {
		controlWait.start();
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Game::add() {
	std::list<STRING> menuList = {};
	AlphabeticalInput controlAlpha(CHAR_TO_WSTR("Set game name : "), menuList);
	auto tmpGame = new Models::Game();
	STRING str;

	//Name Segment
	while (1) {
		try {
			controlAlpha.start();
			str = controlAlpha.getInputValue();
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
			auto controlNum = NumericalInputMapFormat(CHAR_TO_WSTR("Select game process name : "), listProc);

			controlNum.start();
			str = controlNum.getStringValue();
			tmpGame->setProcessName(str);

			Filesystem::pathT pathGame;
			WinAPI::Process::getPathProcess(pathGame, std::stoi(controlNum.getInputValue()));
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

			auto controlAlpha = NumericalInputMapFormat(CHAR_TO_WSTR("What is the extension of save files? : "), listExtensions);
			controlAlpha.start();
			str = controlAlpha.getStringValue();
			tmpGame->setExtensionOfSave(str);
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
	std::list<STRING> menuList = { CHAR_TO_WSTR("List games"), CHAR_TO_WSTR("Add game"), CHAR_TO_WSTR("Remove game"), CHAR_TO_WSTR("Exit") };
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
	std::list<STRING> menuList = { CHAR_TO_WSTR("Game"), CHAR_TO_WSTR("Snapshot"), CHAR_TO_WSTR("Settings"), CHAR_TO_WSTR("Exit") };
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

Menu::InputFormat::InputFormat(const STRING& title, const std::list<STRING>& selections) {
	this->selections = selections;
	this->title = title;
}

void Menu::InputFormat::_determineError() {
	if (!ISNUMBER(input))
		throw Exceptions::invalid_input("Only Numerical value is allowed.");
}

void Menu::InputFormat::_print() {
	unsigned int i = 0;
	for (auto& it : selections) {
		std::cout << ++i << ". ";
		COUT << it << std::endl;
	}
	COUT << title << std::endl;
}

void Menu::AlphabeticalInput::_determineError() {
	if (!ISALPHA(input)) 
		throw Exceptions::invalid_input("Only Alphabetical value is allowed.");
	if (!ISFILENAMEAVAILABLE(input))
		throw Exceptions::invalid_input("Game name includes invalid character(s).");
}

void Menu::NumericalInputMapFormat::_print()
{
	for (auto& it : selections) {
		std::cout << it.first << ". ";
		COUT << it.second << std::endl;
	}
	COUT << title << std::endl;
}

void Menu::NumericalInputMapFormat::_determineError()
{
	if (!ISNUMBER(input))
		throw Exceptions::invalid_input("Only Numerical value is allowed.");
	if (selections.find(std::stoi(input)) == selections.end())
		throw Exceptions::invalid_input("Unknown Option");
}

Menu::NumericalInputMapFormat::NumericalInputMapFormat(const STRING& title, const std::map<unsigned long long, STRING>& selections)
{
	this->title = title;
	this->selections = selections;
}

Menu::NumericalInputMapFormat::NumericalInputMapFormat(const STRING& title, const std::list<STRING>& selections)
{
	unsigned long long i = 1;
	this->title = title;
	for (auto& it : selections) {
		this->selections.insert({ i, it });
		i++;
	}
}

STRING Menu::NumericalInputMapFormat::getStringValue() const
{
	return this->selections.find(std::stoi(getInputValue()))->second;
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
