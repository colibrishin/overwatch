#include "data/models.hpp"
#include "menu.hpp"
#include "win/winapi.hpp"

// WinAPI macro blocks the keyword max() -> std::streamsize
#undef max

#include <set>
#include <numeric>

void Menu::Game::remove() {
	auto listPaths = ptrManager->iterateGameData();
	auto listGames = ptrManager->listGames(listPaths);
	STRING str;

	if (listGames.empty()) {
		WaitPromptInput controlWait("== No games to shown ==");
		controlWait.start();
		return;
	}

	NumericalInput controlNum("Choose game to remove : ", listGames);
	try {
		controlNum.start();
		str = controlNum.getInputValue();

		auto it = listPaths.begin();
		std::advance(it, std::stoi(str) - 1);

		// TODO : What if currGame is same as the one want to be removed?
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Game::list() {
	auto listPaths = ptrManager->iterateGameData();
	auto listGames = ptrManager->listGames(listPaths);
	WaitPromptInput controlWait("== Added games in manager (Enter to continue...) ==", listGames);

	try {
		controlWait.start();
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Menu::Game::add() {
	std::list<STRING> menuList = {};
	AlphabeticalInput controlAlpha("Set game name : ", menuList);
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
			auto controlNum = NumericalInputMapFormat("Select game process name : ", listProc);

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
			auto controlPath = PathInput("Set the path to save files stored : ");

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

			auto controlAlpha = NumericalInputMapFormat("What is the extension of save files? : ", listExtensions);
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
	// TODO: L"" type is not following with STRING.
	std::list<STRING> menuList = { CHAR_TO_WSTR("List games"), CHAR_TO_WSTR("Add game"), CHAR_TO_WSTR("Remove game"), CHAR_TO_WSTR("Exit") };
	NumericalInput control("== Game Menu ==", menuList);
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
				//EXIT
				break;
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
	Menu::NumericalInput control("== Main Menu ==", menuList);
	STRING str;
	bool isExit = false;

	//Test::CreateDummyGame(std::move(manager));

	while (!isExit) {
		try {
			control.start();
			str = control.getInputValue();

			switch (std::stoi(str)) {
			case 1:
				Menu::Flows::game();
				break;
			case 2:
				// GOTO SNAPSHOT
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

Menu::InputFormat::InputFormat(const std::string& title, const std::list<STRING>& selections) {
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
		std::wcout << it << std::endl;
	}
	std::cout << title << std::endl;
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
		std::wcout << it.second << std::endl;
	}
	std::cout << title << std::endl;
}

void Menu::NumericalInputMapFormat::_determineError()
{
	if (!ISNUMBER(input))
		throw Exceptions::invalid_input("Only Numerical value is allowed.");
	if (selections.find(std::stoi(input)) == selections.end())
		throw Exceptions::invalid_input("Unknown Option");
}

Menu::NumericalInputMapFormat::NumericalInputMapFormat(const std::string& title, const std::map<unsigned long long, STRING>& selections)
{
	this->title = title;
	this->selections = selections;
}

Menu::NumericalInputMapFormat::NumericalInputMapFormat(const std::string& title, const std::list<STRING>& selections)
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
		std::getline(CIN, input);
		_determineError();
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

void Menu::WaitPromptInput::_determineError() { 
	// Not really good way to solve this
	CIN.ignore(std::numeric_limits<std::streamsize>().max(), NULL_CHAR);
}

void Menu::PathInput::_determineError()
{
	if (!std::filesystem::exists(this->input))
		throw Exceptions::invalid_input("Directory does not exists.");
}
