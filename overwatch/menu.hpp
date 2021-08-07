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
		std::string title;
		std::list<STRING> selections;
		
		virtual void _determineError();
	public:
		InputFormat() = delete;
		InputFormat(const std::string& title, const std::list<STRING>& selections);
	};

	class NumericalInputMapFormat : public BaseInputFormat{
	private:
		void _print();
	protected:
		std::string title;
		std::map<unsigned long long, STRING> selections;

		virtual void _determineError();
	public:
		NumericalInputMapFormat() = delete;
		NumericalInputMapFormat(const std::string& title, const std::map<unsigned long long, STRING>& selections);
		NumericalInputMapFormat(const std::string& title, const std::list<STRING>& selections);
		STRING getStringValue() const;
	};

	class NumericalInput : public InputFormat {
	public:
		NumericalInput(const std::string& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	};
	class AlphabeticalInput : public InputFormat {
	public:
		AlphabeticalInput(const std::string& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	private:
		void _determineError();
	};
	class PathInput : public InputFormat {
	public:
		PathInput(const std::string& title) : InputFormat(title, {}) {}
	private:
		void _determineError();
	};

	class WaitPromptInput : public InputFormat {
	private:
		void _determineError();
	public:
		WaitPromptInput(const std::string& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	};

	namespace Game {
		void add();
		void list();
	}

	namespace Flows {
		void game();
	}
}

int main(void);

#endif