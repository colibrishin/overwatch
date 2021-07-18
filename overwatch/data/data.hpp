#ifndef DATA_H
#define DATA_H

#include <numeric>
#include <cstdarg>

#include "../macros/global.hpp"
#include "../hash/hash.hpp"
#include "../macros/utility.hpp"
#include "../io/reader.hpp"
#include "../filesystem/fs.hpp"

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
				MEMNULLCPY(nameGame, _ref.nameGame, sizeof(nameGame) / sizeof(CHAR));
				MEMNULLCPY(nameProc, _ref.nameProc, sizeof(nameProc) / sizeof(CHAR));
				MEMNULLCPY(pathGame, _ref.pathGame, sizeof(pathGame) / sizeof(CHAR));
				MEMNULLCPY(pathSave, _ref.pathSave, sizeof(pathSave) / sizeof(CHAR));
				MEMNULLCPY(strExtenison, _ref.strExtenison, sizeof(strExtenison) / sizeof(CHAR));
			}
			
			void _CLEAR() {
				MEMSET(nameGame, NULL_CHAR, sizeof(nameGame) / sizeof(CHAR));
				MEMSET(nameProc, NULL_CHAR, sizeof(nameProc) / sizeof(CHAR));
				MEMSET(pathGame, NULL_CHAR, sizeof(pathGame) / sizeof(CHAR));
				MEMSET(pathSave, NULL_CHAR, sizeof(pathSave) / sizeof(CHAR));
				MEMSET(strExtenison, NULL_CHAR, sizeof(strExtenison) / sizeof(CHAR));
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
			strPathT pathCopy;
			strPathT pathSave;

			void _COPY(const Snapshot& _ref) {
				this->hashGame = _ref.hashGame;
				this->hashSnapshot = _ref.hashSnapshot;
				this->dateAdded = _ref.dateAdded;
				MEMNULLCPY(this->nameSave, _ref.nameSave, sizeof(this->nameSave) / sizeof(CHAR));
				MEMNULLCPY(this->pathCopy, _ref.pathCopy, sizeof(this->pathCopy) / sizeof(CHAR));
				MEMNULLCPY(this->pathSave, _ref.pathSave, sizeof(this->pathSave) / sizeof(CHAR));
			}

			void _CLEAR() {
				this->hashGame.clear();
				this->hashSnapshot.clear();
				this->dateAdded = 0;
				MEMSET(this->nameSave, NULL_CHAR, sizeof(this->nameSave) / sizeof(CHAR));
				MEMSET(this->pathCopy, NULL_CHAR, sizeof(this->pathCopy) / sizeof(CHAR));
				MEMSET(this->pathSave, NULL_CHAR, sizeof(this->pathSave) / sizeof(CHAR));
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
					hashSnapshots[i] = _ref.hashSnapshots[i];
					MEMNULLCPY(this->nameSave[i], _ref.nameSave[i], sizeof(this->nameSave[0]) / sizeof(CHAR));
					this->dateAdded[i] = _ref.dateAdded[i];
				}
				this->occuiped = _ref.occuiped;
			}

			void _CLEAR() {
				for (int i = 0; i < MAX_SNAPSHOTS_IN_MAP; ++i) {
					hashSnapshots[i].clear();
					MEMSET(this->nameSave[i], NULL_CHAR, sizeof(this->nameSave[i]) / sizeof(CHAR));
					this->dateAdded[i] = 0;
				}
				this->occuiped = 0;
			}
		};
	}

	namespace Form {
		template<class T>
		struct Hashable_Data {
		private:
			Hash::SHA256 hash;
			T data;

			void _COPY(const Hashable_Data<T>& _ref) noexcept {
				this->hash = _ref.hash;
				this->data = _ref.data;
			}

			void _CLEAR() noexcept {
				this->hash.clear();
				this->data._CLEAR();
			}

			void _UPDATE_HASH() noexcept{
				hash = Hash::SHA256(reinterpret_cast<char*>(&data));
			}

		public:
			Hashable_Data() {
				_CLEAR();
			}

			Hashable_Data(const Hashable_Data<T>& _ref) {
				_COPY(_ref);
			}

			Hashable_Data(const T& _ref) {
				this->data = _ref;
				_UPDATE_HASH();
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
				Hash::SHA256 tmp(reinterpret_cast<char*>(&this->data));
				return this->hash == tmp;
			}

			Hash::SHA256 getHash() const noexcept {
				return this->hash;
			}

			const T& getValue() const noexcept {
				return this->data;
			}
		};
	}
}

#endif