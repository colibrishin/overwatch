#include "data/models.hpp"
#include "menu.hpp"
#include "win/winapi.hpp"

void Menu::Game::add() {
	std::list<STRING> menuList = {};
	AlphabeticalInput controlAlpha("Set game name : ", menuList);
	NumericalInput controlNum("Choose process name : ", menuList);
	auto tmpGame = new Models::Game();
	STRING str;

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

	while (1) {
		try {
			WinAPI::Process::getProcesses(menuList);
			controlNum = NumericalInput("Choose process name : ", menuList);

			controlNum.start();
			str = controlNum.getInputValue();
			tmpGame->setProcessName(str);
			break;
		}
		catch (const Exceptions::Exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

}

void Menu::Flows::game() {
	std::list<STRING> menuList = { L"List games", L"Add game", L"Remove game", L"Exit" };
	NumericalInput control("== Game Menu ==", menuList);
	STRING str;

	while (1) {
		try {
			control.start();
			str = control.getInputValue();

			switch (std::stoi(str)) {
			case 1:
				//GOTO LIST
				break;
			case 2:
				//GOTO ADD
				break;
			case 3:
				//GOTO REMOVE
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
	std::list<STRING> menuList = { L"Game", L"Snapshot", L"Settings", L"Exit" };
	Menu::NumericalInput control("== Main Menu ==", menuList);
	STRING str;

	//Test::CreateDummyGame(std::move(manager));

	try {
		control.start();
		str = control.getInputValue();

		switch (std::stoi(str)) {
		case 1:
			Menu::Game::add();
			break;
		case 2:
			// GOTO SNAPSHOT
			break;
		case 3:
			// GOTO SETTINGS
			break;
		case 4:
			// GOTO EXIT
			break;
		default:
			throw Exceptions::invalid_input("Unknown option");
		}
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}

Menu::InputFormat::InputFormat(const std::string& title, const std::list<STRING>& selections) {
	this->selections = selections;
	this->title = title;
}

void Menu::InputFormat::start() {
	_print();
	_getInput();
}

const STRING Menu::InputFormat::getInputValue() {
	return this->input;
}

void Menu::InputFormat::_determineError() {
	if (!ISNUMBER(input)) throw Exceptions::invalid_input("Only Numerical value is allowed.");
}

void Menu::InputFormat::_print() {
	unsigned int i = 0;
	for (auto& it : selections) {
		std::cout << ++i << ". ";
		std::wcout << it << std::endl;
	}
	std::cout << title << std::endl;
}

void Menu::InputFormat::_getInput() {
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

void Menu::AlphabeticalInput::_determineError() {
	if (!ISALPHA(input)) throw Exceptions::invalid_input("Only Alphabetical value is allowed.");
}