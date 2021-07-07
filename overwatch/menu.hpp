#ifndef MENU_H
#define MENU_H

#include <string>
#include <iostream>
#include <limits>
#include <memory>
#include <cmath>

#include "database.hpp"
#include "utility.hpp"
#include "unicode.hpp"
#include "exception.hpp"

namespace Menu {
	namespace Constants {
		constexpr const char* STR_TIP_ENTER_CONTINUE = "Press Enter to continue";
		constexpr const char* STR_ERR_UNKNOWN_OPTION = "ERROR : Unknown Option";
		constexpr const char* STR_ERR_SYS_INPUT = "ERROR : System Input Error occurred";
		constexpr const char* STR_ERR_BLANK_NOT_ALLOWED = "ERROR : This cannot be blank";

		namespace Returns {

			constexpr const unsigned short MAIN_OPT_GAME = 2;
			constexpr const unsigned short MAIN_OPT_SNAPSHOT = 4;
			constexpr const unsigned short MAIN_OPT_SETTINGS = 8;
			constexpr const unsigned short MAIN_OPT_BACK = 16;

			constexpr const unsigned short GAME_OPT_LIST = 2;
			constexpr const unsigned short GAME_OPT_ADD = 4;
			constexpr const unsigned short GAME_OPT_EDIT = 8;
			constexpr const unsigned short GAME_OPT_REM = 16;
			constexpr const unsigned short GAME_OPT_BACK = 32;

			constexpr const unsigned short SNAP_OPT_LIST = 2;
			constexpr const unsigned short SNAP_OPT_REM = 4;
			constexpr const unsigned short SNAP_OPT_BACK = 8;

			constexpr const unsigned short SETT_OPT_DAEMON = 2;
			constexpr const unsigned short SETT_OPT_CLEAR = 4;
			constexpr const unsigned short SETT_OPT_ORPHAN = 8;
			constexpr const unsigned short SETT_OPT_BLACKLIST = 16;
			constexpr const unsigned short SETT_OPT_BACK = 32;
		}
	}

	namespace Enums {
		using ReturnMenu = Exception::baseReturn;
		enum TMenuTReturns {
			NIL = -1,
			INT = 1,
			STR = 2,
			FAIL = 4,
			QUIT = 8
		};
	}

	namespace Types {
		class MenuInputT {
		private:
			STRING buf;
			ENCODING_TYPE charMagic[2] = { '\0', '\0' };
			unsigned long long intVal = 0;
			unsigned long long limit = 0;
			short STATUS = Enums::TMenuTReturns::NIL;
		
		public:
			MenuInputT() = delete;
			MenuInputT(unsigned long long limit, const wchar_t* q) {
				wchar_t* tmp = const_cast<wchar_t*>(q);
				UT_memcpy(this->charMagic, tmp, sizeof(this->charMagic));
				this->limit = limit;
			}
			void input() {
				STATUS = Enums::TMenuTReturns::NIL;
				
				if (!static_cast<bool>(std::getline(CIN, buf))) return;

				CIN.ignore(ULLONG_MAX);
				if(UT_isNumber(buf)){
					STATUS = Enums::TMenuTReturns::INT;
					intVal = std::stoi(buf);
					if (intVal <= 0 && this->limit <= intVal) 
						STATUS += Enums::TMenuTReturns::FAIL;
				}
				else {
					STATUS = Enums::TMenuTReturns::STR;
					if (buf.empty()) STATUS += Enums::TMenuTReturns::FAIL;
					if (buf == charMagic) STATUS += Enums::TMenuTReturns::QUIT;
				}
			}

			unsigned long long getInt() { return intVal;  }
			STRING getString() { return buf; }
			unsigned short getStatus(){ return STATUS; }
		};
	}

	namespace Utility {
		unsigned long long numToBitmask(unsigned long long);
	}

	namespace Main {
		std::unique_ptr<Data::Models::Game> main(Menu::Enums::ReturnMenu& status, std::unique_ptr<Data::Models::Game> g);
	}

	namespace Game {
		std::unique_ptr<Data::Models::Game> main(Menu::Enums::ReturnMenu& rtn, std::unique_ptr<Data::Models::Game> g);
		std::unique_ptr<Data::Models::Game> list(Menu::Enums::ReturnMenu& rtn, std::unique_ptr<Data::Models::Game> g);
		std::unique_ptr<Data::Models::Game> add(Menu::Enums::ReturnMenu& rtn, std::unique_ptr<Data::Models::Game> g);
		std::unique_ptr<Data::Models::Game> edit(Menu::Enums::ReturnMenu rtn, std::unique_ptr<Data::Models::Game> g);
	
		namespace MenusAdd {
			Menu::Enums::ReturnMenu setNameInstance(Menu::Types::MenuInputT& io, STRING& nameInstance);
			Menu::Enums::ReturnMenu setNameProc(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG);
			Menu::Enums::ReturnMenu setPathGame(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG);
			Menu::Enums::ReturnMenu setPathSave(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG);
			Menu::Enums::ReturnMenu setStrExtension(Menu::Types::MenuInputT& io, Data::Types::ctxGame& ctxG);
		}
	}
}

/*intVal >= static_cast<long long>(std::numeric_limits<T>().max()) &&
						(this->limit < intVal && this->limit <= 0))*/

#endif