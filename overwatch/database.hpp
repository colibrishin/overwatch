#ifndef DATABASE_H
#define DATABASE_H

#include <memory>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <functional>
#include <utility>

#include "hash.hpp"
#include "utility.hpp"
#include "unicode.hpp"

using pathT = std::filesystem::path;

namespace Database {
	namespace Constants {
		constexpr const unsigned short INSTANCE_MAX_NAME = 500 + 1;
	}

	namespace Types {
		using NameInstanceT = ENCODING_TYPE[Constants::INSTANCE_MAX_NAME];
	}

	namespace Enums {
		enum FILE_MODE {
			NULL_MODE = 0,
			READ_MODE = 1,
			WRITE_MODE = 2,
			RW_MODE = 3
		};
	}

	template<class T>
	class Instance {
	public:
		T data;
		Types::NameInstanceT name;
		SHA256::Hash chksum;
		Instance();
		Instance(const Instance<T>&);
		Instance(const T& val, const Types::NameInstanceT _nameInstance);
		Instance<T>& operator=(const Instance<T>& _ref);
	};

	template <class T>
	class Validator {
	private:
		bool isValid;
	protected:
		inline bool isfileValid() const noexcept 
			{ return this->isValid; }
	public:
		Validator() 
			{ this->isValid = false; };
		Validator(const Validator& _ref) 
			{ this->isValid = _ref.isValid; }
		Validator& operator=(const Validator& _ref) { 
			this->isValid = _ref.isValid; 
			return *this; 
		}
		Validator(const Instance<T>& _targetData);
	};


	class Filesystem {
	private:
		bool isPathExist;
		bool isFileExist;
		Enums::FILE_MODE fileMode;
		pathT path;
	protected:
		inline void setFileExist(bool _SET) noexcept { this->isFileExist = _SET; }
	public:
		Filesystem() { 
			this->path = "";
			isPathExist = isFileExist = false; 
			fileMode = Enums::FILE_MODE::NULL_MODE;
		}
		Filesystem(const Filesystem& _ref) {
			this->path = _ref.path; 
			this->fileMode = _ref.fileMode; 
			this->isFileExist = _ref.isFileExist; 
			this->isPathExist = _ref.isPathExist;
		}
		Filesystem& operator=(const Filesystem& _ref) {
			this->path = _ref.path;
			this->fileMode = _ref.fileMode;
			this->isFileExist = _ref.isFileExist;
			this->isPathExist = _ref.isPathExist;
			return *this; 
		}
		Filesystem(const pathT& _path, const Enums::FILE_MODE _fileMode, const bool _isPathExist, const bool _isFileExist) {
			this->path = _path;
			this->fileMode = _fileMode;
			this->isFileExist = _isFileExist;
			this->isPathExist = _isPathExist;
		}
		
		Filesystem(const pathT& _targerfileName, Enums::FILE_MODE _fileMode);
		void moveFile(const pathT & _newfileName);
		inline const pathT& getfilePath() const noexcept { return this->path; }
		inline bool doesPathExist() const noexcept { return this->isPathExist; }
		inline bool doesFileExist() const noexcept { return this->isFileExist; }
	};

	template<class T>
	class Typist : public Filesystem {
	private:
		std::fstream f;
		bool isWriteSuccess;
		// TODO
		void writeFile(const Instance<T>& _ref, const std::streamsize posSeek);
	protected:
		Typist() : Filesystem() { this->isWriteSuccess = false; }
		Typist(const Typist& _ref) : Filesystem(_ref) { this->isWriteSuccess = _ref.isWriteSuccess; }
		Typist& operator=(const Typist& _ref) {
			this->isWriteSuccess = _ref.isWriteSuccess;
			this->Filesystem::Filesystem(_ref);
			return *this;
		}
		
		Typist(const pathT& _targetfileName) : Filesystem(_targetfileName, Database::Enums::FILE_MODE::READ_MODE) {}

		Typist(const Instance<T>& val, const pathT& _filePathToWrite) : Filesystem(_filePathToWrite, Database::Enums::FILE_MODE::WRITE_MODE)
			{ this->writeFile(val, 0); }
		Typist(const Instance<T>& val, const pathT& _filePathToWrite, const std::streamsize _posToSeek) : Filesystem(_filePathToWrite, Database::Enums::FILE_MODE::WRITE_MODE) 
			{ this->writeFile(val, _posToSeek); }

		const bool isWriteSuccessful() const noexcept { return this->isWriteSuccess; }
	};

	template <class T>
	class Loader : public Typist<T> {
	private:
		std::ifstream f;

		void loadFile(const std::streamsize _posToSeek);
	protected:
		bool isLoaded;
		Instance<T> instance;
	public:
		Loader() : Typist<T>() { this->isLoaded = false; }
		Loader(const Loader<T>& _ref) : Typist<T>(_ref) { 
			this->instance = _ref.instance; 
			this->isLoaded = _ref.isLoaded; 
		}
		Loader& operator=(const Loader& _ref) { 
			this->Typist<T>::Typist(_ref);
			this->instance = _ref.instance;
			this->isLoaded = _ref.isLoaded;
			return *this;
		}

		Loader(const Instance<T>& val, const pathT& _filePathToWrite) : Typist<T>(val, _filePathToWrite) 
			{ if (this->isLoaded = this->isWriteSuccessful()) this->instance = val; }
		Loader(const Instance<T>& val, const pathT& _filePathToWrite, const std::streamsize _posToSeek) : Typist<T>(val, _filePathToWrite, _posToSeek) 
			{ if (this->isLoaded = this->isWriteSuccessful()) this->instance = val; }

		Loader(const pathT& _targetfileName) : Typist<T>(_targetfileName) {
			this->isLoaded = false;
			this->loadFile(0);
		}
		Loader(const pathT& _targetfileName, const std::streamsize _posToSeek) : Typist<T>(_targetfileName) {
			this->isLoaded = false;
			this->loadFile(_posToSeek);
		}

		inline const Instance<T>& readInstance() { return this->instance; }
		inline bool doesfileLoaded() { return this->isLoaded; }
	};

	template<class T>
	class Schema : public Loader<T>, public Validator<T>  {
	private:
		bool _isGoodtoGo(Database::Enums::FILE_MODE _achieveMode) noexcept;
	protected:
		bool good2go;
	public:
		Schema() : Validator<T>(), Loader<T>() { this->good2go = false; };
		Schema(const Schema<T>& _ref) : Loader<T>(_ref), Validator<T>(_ref) {
			this->good2go = _ref.good2go;
		}
		Schema<T>& operator=(const Schema<T>& _ref) {
			this->Loader<T>::Loader(_ref);
			this->Validator<T>::Validator(_ref);
			this->good2go = _ref.good2go;
			return *this;
		}

		Schema(const Instance<T>& val, const pathT& _filePathToWrite) :Loader<T>(val, _filePathToWrite), Validator<T>(this->readInstance())
			{ this->_isGoodtoGo(Database::Enums::FILE_MODE::WRITE_MODE); }
		Schema(const Instance<T>& val, const pathT & _filePathToWrite, const std::streamsize _posToSeek) : Loader<T>(val, _filePathToWrite, _posToSeek), Validator<T>(this->readInstance())
			{ this->_isGoodtoGo(Database::Enums::FILE_MODE::WRITE_MODE); }

		Schema(const pathT _targetfileName) : Loader<T>(_targetfileName), Validator<T>(this->readInstance()) 
			{ this->_isGoodtoGo(Database::Enums::FILE_MODE::READ_MODE); }
		Schema(const pathT _targetfileName, const std::streamsize _posToSeek) : Loader<T>(_targetfileName, _posToSeek), Validator<T>(this->readInstance()) 
			{ this->_isGoodtoGo(Database::Enums::FILE_MODE::READ_MODE); }

		inline bool isGoodtoGo() const noexcept { return this->good2go; }
		void excludeFromDatabase(const pathT& _pathExcludeFolder);
	};
}

namespace Data {
	namespace Constants {
		constexpr const unsigned short MAX_PROCESS_NAME_LENGTH = 32767 + 1;
		// There's same macro in winapi.hpp

		constexpr const unsigned char MAX_SNAPSHOT_COUNT = 255;

		constexpr const char* GAME_FOLDER = "games/";
		constexpr const char* SNAPSHOT_FOLDER = "snapshots/";
		constexpr const char* EXCLUDE_FOLDER = "excludes/";
		constexpr const char* DATA_FILE_EXTENSION = ".dat";
	}

	namespace Types {
		using NameProcT = ENCODING_TYPE[Constants::MAX_PROCESS_NAME_LENGTH];
		using PathFileT = ENCODING_TYPE[Constants::MAX_PROCESS_NAME_LENGTH];

		namespace MapT {
			template<unsigned long long T>
			class CharT {
			public:
				CharT(const ENCODING_TYPE* src) { UT_NULLbreakCopy(value, src, T); }
				ENCODING_TYPE& operator[](unsigned long long idx) { return this->value[idx]; }
				ENCODING_TYPE value[T];
			};

			struct SHA256T {
				SHA256T(const SHA256::Hash src) { hash = src; }
				char& operator[](unsigned int idx) { return this->hash[idx]; }
				bool operator==(const SHA256::Hash compare) const {
					short rtn = std::strncmp(this->hash.c_str(), compare, SHA256::HASH_LENGTH);
					return !(SHA256::ABS(rtn) == 0 ? true : false);
				}
				bool operator==(const std::string& compare) const {
					return (hash == compare ? true : false);
				}
				bool operator==(const SHA256T& compare) const {
					return (hash == compare.hash ? true : false);
				}
				std::string hash;

				struct UnorderedHash {
					std::size_t operator()(const SHA256T& val) const { return std::hash<std::string>{}(val.hash); }
				};
			};
		}

		class ctxGame {
		public:
			Types::NameProcT nameProc;
			Types::PathFileT pathGame;
			Types::PathFileT pathSave;
			Types::NameProcT strExtension;

			ctxGame() {
				UT_memset(this->nameProc, NULL_CHAR, sizeof(this->nameProc));
				UT_memset(this->pathGame, NULL_CHAR, sizeof(this->pathGame));
				UT_memset(this->pathSave, NULL_CHAR, sizeof(this->pathSave));
				UT_memset(this->strExtension, NULL_CHAR, sizeof(this->strExtension));
			}
			ctxGame(const ctxGame& _ref) {
				UT_NULLbreakCopy(this->nameProc, _ref.nameProc, sizeof(this->nameProc));
				UT_NULLbreakCopy(this->pathGame, _ref.pathGame, sizeof(this->pathGame));
				UT_NULLbreakCopy(this->pathSave, _ref.pathSave, sizeof(this->pathSave));
				UT_NULLbreakCopy(this->strExtension, _ref.strExtension, sizeof(this->strExtension));
			}
			ctxGame& operator=(const ctxGame& _ref) { 
				UT_NULLbreakCopy(this->nameProc, _ref.nameProc, sizeof(this->nameProc));
				UT_NULLbreakCopy(this->pathGame, _ref.pathGame, sizeof(this->pathGame));
				UT_NULLbreakCopy(this->pathSave, _ref.pathSave, sizeof(this->pathSave));
				UT_NULLbreakCopy(this->strExtension, _ref.strExtension, sizeof(this->strExtension));
				return *this;
			}
		};

		class ctxSnapshot {
		public:
			SHA256::Hash hashGame;
			std::time_t dateAdded;
			Types::NameProcT nameSave;
			Types::PathFileT pathOrigin;
			SHA256::Hash hashSave;

			ctxSnapshot() {
				memset(this->hashGame, '\0', sizeof(this->hashGame));
				this->dateAdded = 0;
				UT_memset(this->nameSave, NULL_CHAR, sizeof(this->nameSave));
				UT_memset(this->pathOrigin, NULL_CHAR, sizeof(this->pathOrigin));
				memset(this->hashSave, '\0', sizeof(this->hashSave));
			}
			ctxSnapshot(const ctxSnapshot& _ref) {
				UtilityString::_charNULLbreakCopy(this->hashGame, _ref.hashGame, sizeof(this->hashGame));
				this->dateAdded = _ref.dateAdded;
				UT_NULLbreakCopy(this->nameSave, _ref.nameSave, sizeof(this->nameSave));
				UT_NULLbreakCopy(this->pathOrigin, _ref.pathOrigin, sizeof(this->pathOrigin));
				UtilityString::_charNULLbreakCopy(this->hashSave, _ref.hashSave, sizeof(this->hashSave));
			}
			ctxSnapshot& operator=(const ctxSnapshot& _ref){
				UtilityString::_charNULLbreakCopy(this->hashGame, _ref.hashGame, sizeof(this->hashGame));
				this->dateAdded = _ref.dateAdded;
				UT_NULLbreakCopy(this->nameSave, _ref.nameSave, sizeof(this->nameSave));
				UT_NULLbreakCopy(this->pathOrigin, _ref.pathOrigin, sizeof(this->pathOrigin));
				UtilityString::_charNULLbreakCopy(this->hashSave, _ref.hashSave, sizeof(this->hashSave));
				return *this; 
			}
		};

		class ctxMap {
		public:
			SHA256::Hash hashSnapshot[Constants::MAX_SNAPSHOT_COUNT];
			Types::NameProcT nameSave[Constants::MAX_SNAPSHOT_COUNT];
			std::time_t dateAdded[Constants::MAX_SNAPSHOT_COUNT];
			unsigned char occupied;

			ctxMap() {
				for (unsigned long long i = 0; i < Constants::MAX_SNAPSHOT_COUNT; ++i) {
					memset(this->hashSnapshot[i], '\0', sizeof(this->hashSnapshot[0]));
					UT_memset(this->nameSave[i], NULL_CHAR, sizeof(this->nameSave[0]));
					this->dateAdded[i] = 0;
				}
				this->occupied = 0;
			}

			ctxMap(const ctxMap& _ref) {
				for (unsigned long long i = 0; i < Constants::MAX_SNAPSHOT_COUNT; ++i) {
					UtilityString::_charNULLbreakCopy(this->hashSnapshot[i], _ref.hashSnapshot[i], sizeof(this->hashSnapshot[0]));
					this->dateAdded[i] = _ref.dateAdded[i];
					UT_NULLbreakCopy(this->nameSave[i], _ref.nameSave[i], sizeof(this->nameSave[0]));
				}
				this->occupied = _ref.occupied;
			}
			ctxMap& operator=(const ctxMap& _ref) {
				for (unsigned long long i = 0; i < Constants::MAX_SNAPSHOT_COUNT; ++i) {
					UtilityString::_charNULLbreakCopy(this->hashSnapshot[i], _ref.hashSnapshot[i], sizeof(this->hashSnapshot[0]));
					this->dateAdded[i] = _ref.dateAdded[i];
					UT_NULLbreakCopy(this->nameSave[i], _ref.nameSave[i], sizeof(this->nameSave[0]));
				}
				this->occupied = _ref.occupied;
				return *this;
			}
		};
	}

	namespace Models {
		using mapTGame = std::unordered_map<Types::MapT::SHA256T, Types::MapT::CharT<Constants::MAX_PROCESS_NAME_LENGTH>, Types::MapT::SHA256T::UnorderedHash>;
		class Game;

		class Snapshot {
			bool isLoaded;
			std::shared_ptr<Database::Schema<Types::ctxSnapshot>> info;
			Game* parentGame;
		public:
			Snapshot() { 
				this->isLoaded = false; 
				this->info = nullptr; 
				this->parentGame = nullptr;
			}
			Snapshot(const Snapshot& _ref) { 
				this->info = std::make_shared<Database::Schema<Types::ctxSnapshot>>(*_ref.info.get());
				this->isLoaded = _ref.isLoaded; 
				this->parentGame = _ref.parentGame;
			}
			Snapshot& operator=(const Snapshot& _ref) {
				this->info = std::make_shared<Database::Schema<Types::ctxSnapshot>>(*_ref.info.get());
				this->isLoaded = _ref.isLoaded;
				this->parentGame = _ref.parentGame;
				return *this;
			}

			const bool good() const noexcept;
			void createSnapshot(const Types::ctxSnapshot& _val, Game* _parentGame);
			void loadSnapshot(const pathT& _targerName, Game* _parentGame);
			inline const char* getHash() const noexcept { return this->info->readInstance().chksum; }
			inline const std::time_t getTimeAdded() const noexcept { return this->info->readInstance().data.dateAdded; }
			inline const char* getGameHash() const noexcept { return this->info->readInstance().data.hashGame; }
			inline const char* getSaveHash() const noexcept { return this->info->readInstance().data.hashSave; }
			inline const wchar_t* getNameSave() const noexcept { return this->info->readInstance().data.nameSave; }
			inline const wchar_t* getPathOrigin() const noexcept { return this->info->readInstance().data.pathOrigin; }
		};

		class Game {
			bool isLoaded;
			std::shared_ptr<Database::Schema<Types::ctxGame>> info;
			mapTGame mapSnapshot;
			Snapshot currSnapshot;

			//Database::Schema<Types::ctxGame> loadGame(const pathT _targetfileName);
			//Database::Schema<Types::ctxMap> loadMap();
			void loadMap(const Database::Instance<Types::ctxMap>& _RawMapData);
			void buildMap();
			bool validateMap();
			//Snapshot loadSnapshot(const pathT &_targetfileName);

			void deleteSnapshotFromMap(const SHA256::Hash _hashSnapshot);
		public:
			Game() { 
				this->isLoaded = false; 
				this->info = nullptr; 
			}
			Game(const Game& _ref) {
				this->isLoaded = _ref.isLoaded;
				this->info = std::make_shared<Database::Schema<Types::ctxGame>>(*_ref.info.get());
				this->mapSnapshot = _ref.mapSnapshot;
				this->currSnapshot = _ref.currSnapshot; 
			}
			Game& operator=(const Game& _ref) {
				this->isLoaded = _ref.isLoaded;
				this->info = std::make_shared<Database::Schema<Types::ctxGame>>(*_ref.info.get());
				this->mapSnapshot = _ref.mapSnapshot;
				this->currSnapshot = _ref.currSnapshot;
				return *this;
			}
			//inline const bool good() const noexcept{ return this->currSnapshot.good() && this->isLoaded; }
			void createGame(const Types::ctxGame& _val, const Database::Types::NameInstanceT _nameGame);
			void loadGame(const pathT& _targerName);
			inline const bool good() const noexcept { return this->isLoaded; }
			inline const char* getHash() const noexcept { return this->info->readInstance().chksum; }
			inline const ENCODING_TYPE* getName() const noexcept { return this->info->readInstance().name; }
			inline const mapTGame& accessMap() const noexcept { return this->mapSnapshot; }
			void loadSnapshot(const mapTGame::const_iterator& _ItMap) noexcept;
			std::filesystem::path getFilePath() { return this->info->getfilePath(); }
		};
	}

	namespace Utility {
		inline std::filesystem::recursive_directory_iterator getGameDirectoryIterator() { return std::filesystem::recursive_directory_iterator(Data::Constants::GAME_FOLDER); }
	}
}

#endif