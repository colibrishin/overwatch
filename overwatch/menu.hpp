#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <memory>
#include <string>
#include <cstdarg>
#include "data/manager.hpp"

static std::unique_ptr<Manager::Manager> ptrManager;
// This might cause some untrackable errors.

namespace Menu {

	class InputFormat {
	private:
		void _print();
		void _getInput();

	protected:
		std::string title;
		std::list<STRING> selections;
		STRING input;
		
		virtual void _determineError();
	public:
		InputFormat() = delete;
		InputFormat(const std::string& title, const std::list<STRING>& selections);

		void start();
		const STRING getInputValue();
	};

	class NumericalInput : public InputFormat {
	public:
		NumericalInput(const std::string& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	};
	class AlphabeticalInput : public InputFormat {
	private:
		void _determineError();
	public:
		AlphabeticalInput(const std::string& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	};

	namespace Game {
		void add();
	}

	namespace Flows {
		void game();
	}
}

int main(void);

#endif