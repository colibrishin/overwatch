#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <memory>
#include <string>
#include <cstdarg>
#include "data/manager.hpp"

static std::unique_ptr<Manager::Manager> ptrManager;
// This might cause some untrackable errors.
// Do not includes menu.hpp anywhere excepts menu.cpp

namespace Menu {

	class BaseInputFormat {
	private:
		virtual void _print() { }
		void _getInput();
	protected:
		STRING title;
		STRING input;

		virtual void _determineError() { }
	public:
		BaseInputFormat() {};
		void start();
		const STRING getInputValue() const;
	};

	class InputFormat : public BaseInputFormat {
	private:
		void _print();
	protected:
		std::list<STRING> selections;
		
		virtual void _determineError();
	public:
		InputFormat() = delete;
		InputFormat(const STRING& title, const std::list<STRING>& selections);
	};

	class NumericalInputMapFormat : public BaseInputFormat{
	private:
		void _print();
	protected:
		std::map<unsigned long long, STRING> selections;

		virtual void _determineError();
	public:
		NumericalInputMapFormat() = delete;
		NumericalInputMapFormat(const STRING& title, const std::map<unsigned long long, STRING>& selections);
		NumericalInputMapFormat(const STRING& title, const std::list<STRING>& selections);
		STRING getStringValue() const;
	};

	class NumericalInput : public InputFormat {
	public:
		NumericalInput(const STRING& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	};
	class AlphabeticalInput : public InputFormat {
	public:
		AlphabeticalInput(const STRING& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	private:
		void _determineError();
	};
	class PathInput : public InputFormat {
	public:
		PathInput(const STRING& title) : InputFormat(title, {}) {}
	private:
		void _determineError();
	};

	class WaitPromptInput : public InputFormat {
	public:
		WaitPromptInput(const STRING& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
		WaitPromptInput(const STRING& title) : InputFormat(title, {}) { }
	};

	namespace Game {
		void add();
		void list();
		void remove();
	}

	namespace Flows {
		void game();
		void snapshot();
	}

	namespace Snapshot {
		void select_game();
		void list();
		//void add();
		//void remove();
	}
}

int main(void);

#endif