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
		void _getInput();
	protected:
		virtual void _print() {
			std::cout << "baseinput";
			return;
		}
		STRING title;
		STRING input;

		virtual void _determineError() { }
	public:
		BaseInputFormat() {};
		BaseInputFormat(const STRING& title) { this->title = title; }
		void start();
		const STRING getInputValue() const;
	};

	template <class T>
	class InputFormat : public BaseInputFormat {
	protected:
		virtual void _print() { 
			std::cout << "input";
			return;
		}
		T selections;
		
		virtual void _determineError() {
			if (!ISNUMBER(input))
				throw Exceptions::invalid_input("Only Numerical value is allowed.");
			if (this->selections.size() < std::stoi(input))
				throw Exceptions::invalid_input("Unknown Option.");
		}
	public:
		InputFormat() = delete;
		InputFormat(const STRING& title, const T& selections) : BaseInputFormat(title) { this->selections = selections; }
	};

	template <class T, class U>
	class NumericalInputMapFormat : public InputFormat<std::map<T, U>> {
	private:
		virtual void _print() {
			for (auto& it : this->selections) {
				std::cout << it.first << ". ";
				COUT << it.second << std::endl;
			}
			COUT << this->title << std::endl;
		}
	protected:
		virtual void _determineError() {
			if (!ISNUMBER(this->input))
				throw Exceptions::invalid_input("Only Numerical value is allowed.");
			if (this->selections.find(std::stoi(this->input)) == this->selections.end())
				throw Exceptions::invalid_input("Unknown Option");
		}
	public:
		NumericalInputMapFormat() = delete;
		NumericalInputMapFormat(const STRING& title, const std::map<T, U>& selections) : InputFormat<std::map<T, U>>(title, selections){ }
		STRING getStringValue() const {
			return this->selections.find(std::stoi(this->getInputValue()))->second;
		}
	};

	class ListInputFormat : public InputFormat<std::list<STRING>> {
	private:
		virtual void _print() {
			unsigned int i = 0;
			for (auto& it : selections) {
				std::cout << ++i << ". ";
				COUT << it << std::endl;
			}
			COUT << title << std::endl;
		}
	public:
		ListInputFormat(const STRING& title, const std::list<STRING>& selections) : InputFormat(title, selections) { }
	};

	class NumericalInput : public ListInputFormat {
	public:
		NumericalInput(const STRING& title, const std::list<STRING>& selections) : ListInputFormat(title, selections) { }
	};

	class AlphabeticalInput : public ListInputFormat {
	public:
		AlphabeticalInput(const STRING& title, const std::list<STRING>& selections) : ListInputFormat(title, selections) { }
	private:
		void _determineError();
	};

	class PathInput : public ListInputFormat {
	public:
		PathInput(const STRING& title) : ListInputFormat(title, {}) { }
	private:
		void _determineError();
	};

	class WaitPromptInput : public ListInputFormat {
	public:
		WaitPromptInput(const STRING& title, const std::list<STRING>& selections) : ListInputFormat(title, selections) { }
		WaitPromptInput(const STRING& title) : ListInputFormat(title, {}) { }
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
		void add();
		void edit();
		void remove();
	}
}

int main(void);

#endif