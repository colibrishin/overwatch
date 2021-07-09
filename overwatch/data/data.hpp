#ifndef DATA_H
#define DATA_H

#include <numeric>
#include <cstdarg>

#include "../macros/global.hpp"
#include "../hash/hash.hpp"
#include "../macros/utility.hpp"
#include "../io/reader.hpp"
#include "../filesystem/fs.hpp"
#include "../exception_class.hpp"

namespace Data {
	using T_SNAPSHOT_SIZE_IN_MAP = unsigned char;
	const static unsigned char MAX_SNAPSHOTS_IN_MAP = std::numeric_limits<T_SNAPSHOT_SIZE_IN_MAP>().max();

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
				MEMSET(nameGame, NULL_CHAR, sizeof(nameGame));
				MEMSET(nameProc, NULL_CHAR, sizeof(nameProc));
				MEMSET(pathGame, NULL_CHAR, sizeof(pathGame));
				MEMSET(pathSave, NULL_CHAR, sizeof(pathSave));
				MEMSET(strExtenison, NULL_CHAR, sizeof(strExtenison));
			}

			Game() { _CLEAR(); }
			Game(const Game& _ref) { _COPY(_ref);  }
			Game& operator=(const Game& _ref) {
				_COPY(_ref);
				return *this;
			}
		} Game;

		typedef struct Snapshot {
			Hash::SHA256 hashGame;
			Hash::SHA256 hashSnapshot;
			std::time_t dateAdded;
			strNameT nameSave;
			strPathT pathSave;

			void _COPY(const Snapshot& _ref) {
				_MEMNULLCPY(this->hashGame, _ref.hashGame, sizeof(this->hashGame));
				_MEMNULLCPY(this->hashSnapshot, _ref.hashSnapshot, sizeof(this->hashSnapshot));
				this->dateAdded = _ref.dateAdded;
				MEMNULLCPY(this->nameSave, _ref.nameSave, sizeof(this->nameSave));
				MEMNULLCPY(this->pathSave, _ref.pathSave, sizeof(this->pathSave));
			}

			void _CLEAR() {
				_MEMSET(this->hashGame, '\0', sizeof(this->hashGame));
				_MEMSET(this->hashSnapshot, '\0', sizeof(this->hashSnapshot));
				this->dateAdded = 0;
				MEMSET(this->nameSave, NULL_CHAR, sizeof(this->nameSave));
				MEMSET(this->pathSave, NULL_CHAR, sizeof(this->pathSave));
			}

			Snapshot() { _CLEAR(); }
			Snapshot(const Snapshot& _ref) { _COPY(_ref); }
			Snapshot& operator=(const Snapshot& _ref) {
				_COPY(_ref);
				return *this;
			}
		};

		typedef struct Map {
			Hash::SHA256 hashSnapshots[MAX_SNAPSHOTS_IN_MAP];
			strNameT nameSave[MAX_SNAPSHOTS_IN_MAP];
			std::time_t dateAdded[MAX_SNAPSHOTS_IN_MAP];
			T_SNAPSHOT_SIZE_IN_MAP occuiped;

			void _COPY(const Map& _ref) {
				for (int i = 0; i < MAX_SNAPSHOTS_IN_MAP; ++i) {
					_MEMNULLCPY(this->hashSnapshots[i], _ref.hashSnapshots[i], sizeof(this->hashSnapshots[0]));
					MEMNULLCPY(this->nameSave[i], _ref.nameSave[i], sizeof(this->nameSave[0]));
					this->dateAdded[i] = _ref.dateAdded[i];
				}
				this->occuiped = _ref.occuiped;
			}

			void _CLEAR() {
				for (int i = 0; i < MAX_SNAPSHOTS_IN_MAP; ++i) {
					_MEMSET(this->hashSnapshots[i], NULL, sizeof(this->hashSnapshots[0]));
					MEMSET(this->nameSave[i], NULL_CHAR, sizeof(this->nameSave[i]));
					this->dateAdded[i] = 0;
				}
				this->occuiped = 0;
			}
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
			
			void setValue(const T& _NewData) noexcept {
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