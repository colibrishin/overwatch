#ifndef DATA_H
#define DATA_H

#include <cstdarg>

#include "../macros/global.hpp"
#include "../hash/hash.hpp"
#include "../macros/utility.hpp"
#include "../io/reader.hpp"
#include "../filesystem/fs.hpp"
#include "../exception_class.hpp"

namespace Data {
	namespace Structures{
		typedef struct Game {
			strNameT nameGame;
			strProcT nameProc;
			strPathT pathGame;
			strPathT pathSave;
			strProcT strExtenison;

			void _COPY(const Game& _ref) {
				MEMNULLCPY(nameGame, _ref.nameGame, sizeof(nameGame));
				MEMNULLCPY(nameProc, _ref.nameProc, sizeof(nameProc));
				MEMNULLCPY(pathGame, _ref.pathGame, sizeof(pathGame));
				MEMNULLCPY(pathSave, _ref.pathSave, sizeof(pathSave));
				MEMNULLCPY(strExtenison, _ref.strExtenison, sizeof(strExtenison));
			}
			
			void _CLEAR() {
				MEMSET(nameGame, NULL, sizeof(nameGame));
				MEMSET(nameProc, NULL, sizeof(nameProc));
				MEMSET(pathGame, NULL, sizeof(pathGame));
				MEMSET(pathSave, NULL, sizeof(pathSave));
				MEMSET(strExtenison, NULL, sizeof(strExtenison));
			}

			Game() { _CLEAR(); }
			Game(const Game& _ref) { _COPY(_ref);  }
			Game& operator=(const Game& _ref) {
				_COPY(_ref);
				return *this;
			}
		} Game;

		typedef struct Snapshot {
			
		};

		typedef struct MiniSnapshot{

		};

		typedef struct Map {

		};
	}

	namespace Form {
		template<class T>
		class Hashable_Data {
		private:
			Hash::SHA256 hash;
			T data;

			void _COPY(const Hashable_Data<T>& _ref) noexcept {
				memcpy(this->hash, _ref.hash, sizeof(this->hash));
				this->data = _ref.data;
			}

			void _CLEAR() noexcept {
				memset(this->hash, 0, sizeof(this->hash));
				this->data._CLEAR();
			}

			void _UPDATE_HASH() noexcept{
				Hash::calcHash(reinterpret_cast<char*>(&this->data), this->hash);
			}

		public:
			Hashable_Data() {
				_CLEAR();
			}

			Hashable_Data(const Hashable_Data<T>& _ref) {
				_COPY(_ref);
			}

			Hashable_Data<T>& operator=(const Hashable_Data<T>& _ref) {
				_COPY(_ref);
				return *this;
			}
			
			void updateValue(const T& _NewData) noexcept {
				memcpy(&this->data, &_NewData, sizeof(this->data));
				_UPDATE_HASH();
			}

			bool validateValue() {
				Hash::SHA256 tmp;
				Hash::calcHash(reinterpret_cast<char*>(&this->data), tmp);

				return Hash::compareHash(tmp, this->hash);
			}

			const T getValue() const noexcept {
				return this->data;
			}
		};
	}
}

#endif