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
	const static T_SNAPSHOT_SIZE_IN_MAP MAX_SNAPSHOTS_IN_MAP = std::numeric_limits<T_SNAPSHOT_SIZE_IN_MAP>().max();

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

			void _UPDATE_HASH() noexcept {
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

			void clearValue() noexcept {
				memcpy(&this->data, 0, sizeof(T));
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

	namespace Structures{
		typedef struct Game {
			strNameT nameGame;
			strProcT nameProc;
			strPathT pathGame;
			strPathT pathSave;
			strProcT strExtenison;

			void _COPY(const Game& _ref) {
				MEMNULLCPY(nameGame, _ref.nameGame, sizeof(nameGame) / sizeof(CHAR_T));
				MEMNULLCPY(nameProc, _ref.nameProc, sizeof(nameProc) / sizeof(CHAR_T));
				MEMNULLCPY(pathGame, _ref.pathGame, sizeof(pathGame) / sizeof(CHAR_T));
				MEMNULLCPY(pathSave, _ref.pathSave, sizeof(pathSave) / sizeof(CHAR_T));
				MEMNULLCPY(strExtenison, _ref.strExtenison, sizeof(strExtenison) / sizeof(CHAR_T));
			}
			
			void _CLEAR() {
				MEMSET(nameGame, NULL_CHAR, sizeof(nameGame) / sizeof(CHAR_T));
				MEMSET(nameProc, NULL_CHAR, sizeof(nameProc) / sizeof(CHAR_T));
				MEMSET(pathGame, NULL_CHAR, sizeof(pathGame) / sizeof(CHAR_T));
				MEMSET(pathSave, NULL_CHAR, sizeof(pathSave) / sizeof(CHAR_T));
				MEMSET(strExtenison, NULL_CHAR, sizeof(strExtenison) / sizeof(CHAR_T));
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
				MEMNULLCPY(this->nameSave, _ref.nameSave, sizeof(this->nameSave) / sizeof(CHAR_T));
				MEMNULLCPY(this->pathCopy, _ref.pathCopy, sizeof(this->pathCopy) / sizeof(CHAR_T));
				MEMNULLCPY(this->pathSave, _ref.pathSave, sizeof(this->pathSave) / sizeof(CHAR_T));
			}

			void _CLEAR() {
				this->hashGame.clear();
				this->hashSnapshot.clear();
				this->dateAdded = 0;
				MEMSET(this->nameSave, NULL_CHAR, sizeof(this->nameSave) / sizeof(CHAR_T));
				MEMSET(this->pathCopy, NULL_CHAR, sizeof(this->pathCopy) / sizeof(CHAR_T));
				MEMSET(this->pathSave, NULL_CHAR, sizeof(this->pathSave) / sizeof(CHAR_T));
			}

			Snapshot() { _CLEAR(); }
			Snapshot(const Snapshot& _ref) { _COPY(_ref); }
			Snapshot& operator=(const Snapshot& _ref) {
				_COPY(_ref);
				return *this;
			}
		};
		
		typedef struct MiniSnapshot {
			Hash::SHA256 hashSnapshot;
			strNameT nameSave;
			std::time_t dateAdded;

			MiniSnapshot() { _CLEAR(); }
			MiniSnapshot(const MiniSnapshot& _ref) { _COPY(_ref); }
			MiniSnapshot(const Snapshot& _ref) {
				hashSnapshot = _ref.hashSnapshot;
				MEMNULLCPY(this->nameSave, _ref.nameSave, sizeof(_ref.nameSave) / sizeof(CHAR_T));
				this->dateAdded = _ref.dateAdded;
			}
			MiniSnapshot& operator=(const MiniSnapshot& _ref) {
				_COPY(_ref);
				return *this;
			}
			void _COPY(const MiniSnapshot& _ref) {
				hashSnapshot = _ref.hashSnapshot;
				MEMNULLCPY(this->nameSave, _ref.nameSave, sizeof(_ref.nameSave) / sizeof(CHAR_T));
				this->dateAdded = _ref.dateAdded;
			}

			void _CLEAR() {
				hashSnapshot.clear();
				MEMSET(this->nameSave, NULL_CHAR, sizeof(this->nameSave) / sizeof(CHAR_T));
				this->dateAdded = 0;
			}
		};

		typedef struct Map {
			Form::Hashable_Data<MiniSnapshot> snapshots[MAX_SNAPSHOTS_IN_MAP];
			bool mapUsed[MAX_SNAPSHOTS_IN_MAP];
			Map() { _CLEAR(); }
			Map(const Map& _ref) { _COPY(_ref); }
			Map& operator=(const Map& _ref) {
				_COPY(_ref);
				return *this;
			}
			void _COPY(const Map& _ref) {
				memcpy(this->snapshots, _ref.snapshots, sizeof(snapshots));
				memcpy(this->mapUsed, _ref.mapUsed, sizeof(mapUsed));
			}

			void _CLEAR() {
				memset(this->snapshots, 0, sizeof(snapshots));
				memset(this->mapUsed, 0, sizeof(mapUsed));
			}
		};
	}
}

#endif