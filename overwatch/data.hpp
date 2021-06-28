#ifndef DATA_H
#define DATA_H

#include "global.hpp"
#include "hash.hpp"
#include "utility.hpp"
#include "reader.hpp"
#include "fs.hpp"
#include "exception_class.hpp"

namespace Data {

	namespace Contexts {
		class Game {
		public:
			Hash::SHA256 hash;
			strNameT nameGame;
			strProcT nameProc;
			strPathT pathGame;
			strPathT pathSave;
			strProcT strExtenison;

			Game();
			Game(const Game& _ref);
			Game& operator=(const Game& _ref);
		private:
			void _COPY(const Game& _ref);
			void _CLEAR();
		};
	}
	
	namespace DataManager {
		enum class Code {
			OPEN = 1,
			WRITE = 2,
			MODIFY = 3
		};

		template <class T>
		class Manager : public ExceptionClass{
		private:
			T data;
			Reader::Serializer io;
			Filesystem::Filesystem fs;
			Code requestCode;

		public:
			Manager() = delete;
			Manager(const Filesystem::pathT& path, const Code& requestCode) : fs(path, Filesystem::Code::CHECK), io(path, 0, Reader::Code::READ) {
				// TODO
			}
		};
	}

	namespace Models{
		class Game {
		};
	}
}

#endif