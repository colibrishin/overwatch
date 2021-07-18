#ifndef MANAGER_H
#define MANAGER_H

#include <memory>
#include <list>
#include <iostream>
#include <map>
#include <ctime>

#include "data.hpp"
#include "loader.hpp"
#include "../data/models.hpp"
#include "../macros/global.hpp"
#include "../exception.hpp"
#include "../macros/utility.hpp"

namespace Manager {
	enum class Code {
		SAVE,
		CLOSE
	};

	using GameLoaderT = Loader::Loader<Data::Structures::Game>;
	using SnapshotLoaderT = Loader::Loader<Data::Structures::Snapshot>;
	using MapLoaderT = Loader::Loader<Data::Structures::Map>;

	using ptrGameLoaderT = std::unique_ptr<GameLoaderT>;
	using ptrSnapshotLoaderT = std::unique_ptr<SnapshotLoaderT>;
	using ptrMapLoaderT = std::unique_ptr<MapLoaderT>;

	class Manager {
	private:
		ptrGameLoaderT currGame;
		ptrSnapshotLoaderT idxSnapshot;
		ptrMapLoaderT currSnapshotMap;

		Filesystem::listPathT pathGames;
		std::map<STRING, Filesystem::pathT> mapSnapshots;

		Filesystem::Filesystem dirGame;
		Filesystem::Filesystem dirSnapshot;
		

		STRING getTmpFileName(const std::time_t timeCreated, const Hash::SHA256 hash) const noexcept{
			Filesystem::pathT pathFile;
			pathFile.concat(std::to_string(timeCreated)).concat("_").concat(hash.getString());
			return pathFile.native();
		}

		Filesystem::pathT getGameLoaderFileName() const noexcept {
			Filesystem::pathT pathFile = GAME_DIRECTORY;
			auto hashGame = this->currGame->getHash().getString();
			auto nameGame = this->currGame->getData().getValue().nameGame;
			pathFile.concat(hashGame).concat("_").concat(nameGame).concat(GAME_FILE_EXTENSION);

			return pathFile;
		}

		Filesystem::pathT getSnapshotLoaderFileName() const noexcept {
			Filesystem::pathT pathFile = SNAPSHOT_DIRECTORY;
			auto hashSnapshot = this->idxSnapshot->getHash().getString();
			auto nameSnapshot = this->idxSnapshot->getData().getValue().nameSave;
			pathFile.concat(hashSnapshot).concat("_").concat(nameSnapshot).concat(SNAPSHOT_FILE_EXTENSION);

			return pathFile;
		}

		Filesystem::pathT getSnapshotSaveFileCopyFileName(const Data::Structures::Snapshot& data) const noexcept {
			Filesystem::pathT pathFile = SNAPSHOT_COPY_DIRECTORY;

			auto hashSnapshot = data.hashSnapshot.getString();
			Filesystem::pathT pathFileName = data.pathSave;
			pathFileName = pathFileName.filename();

			pathFile.concat(hashSnapshot).concat("_").concat(pathFileName.native());
			return pathFile;
		}

		Hash::SHA256 getSnapshotSaveFileHash(const Filesystem::pathT& pathToFile) const {
			try {
				auto reader = new Reader::Serializer(pathToFile, 0, Reader::Code::READ);
				auto buf = new char[reader->getSizeFile()];
				reader->readFile(buf, reader->getSizeFile());
				auto digest = Hash::SHA256(buf);
				delete[] buf;

				return digest;
			}
			catch (const std::ios::failure& e) {
				throw Exceptions::Exception(e);
			}
		}

		void createTmpFile(const Models::Game& data) {
			auto timeCreated = Utility::getUnixTime();
			Filesystem::pathT pathFile = GAME_DIRECTORY;
			auto Hashable = data.getHashable();
			pathFile.concat(getTmpFileName(timeCreated, Hashable->getHash()));

			try {
				this->currGame = std::make_unique<GameLoaderT>(pathFile, Loader::Code::NEW);
				this->currGame->setData(*Hashable);
				this->currGame->writeData();
			}
			catch (const Exceptions::Exception& e) {
				throw e;
			}
		}

		void createTmpFile(Models::Snapshot& snapshot) {
			auto timeCreated = Utility::getUnixTime();
			Filesystem::pathT pathFile = SNAPSHOT_DIRECTORY;
			auto hashSaveFile = getSnapshotSaveFileHash(snapshot.getData().pathSave);
			auto hashGame = this->currGame->getHash();

			snapshot.setHashGame(hashGame);
			snapshot.setHashSnapshot(hashSaveFile);
			snapshot.setTime(timeCreated);

			try {
				auto pathCopyFile = getSnapshotSaveFileCopyFileName(snapshot.getData());
				snapshot.setPathToCopyOfFile(pathCopyFile);

				Filesystem::Filesystem pathSaveFile(snapshot.getData().pathSave, Filesystem::Code::CHECK);
				pathSaveFile.copy(pathCopyFile);
				pathFile.concat(getTmpFileName(timeCreated, snapshot.getData().hashSnapshot));

				auto Hashable = snapshot.getHashable();

				this->idxSnapshot = std::make_unique<SnapshotLoaderT>(pathFile, Loader::Code::NEW);
				this->idxSnapshot->setData(*Hashable);
				this->idxSnapshot->writeData();
			}
			catch (const std::filesystem::filesystem_error& e){
				throw Exceptions::Exception(e);
			}
			catch (const Exceptions::Exception& e) {
				throw e;
			}
		}
	public:
		Manager() {
			try {
				dirGame = Filesystem::Filesystem(GAME_DIRECTORY, Filesystem::Code::CREATE);
				dirSnapshot = Filesystem::Filesystem(SNAPSHOT_DIRECTORY, Filesystem::Code::CREATE);
			}
			catch (const std::filesystem::filesystem_error& e) {
				std::cout << e.what() << std::endl;
				throw Exceptions::Exception(e);
			}

			try {
				auto io = Reader::Serializer(CONFIG_FILE, 0, Reader::Code::READ);
				// TODO : Load Config file
			}
			catch (const std::ios::failure& e) {
				auto io = Reader::Serializer(CONFIG_FILE, 0, Reader::Code::WRITE);
				// TODO : Config file format
			}

			this->pathGames = iterateGameData();
		}

		const Filesystem::listPathT iterateGameData() noexcept {
			auto list = dirGame.iteratePath();

			for (auto it = list.begin(); it != list.end(); ++it) {
				if (it->extension() == GAME_FILE_EXTENSION) {
					try {
						this->loadGame(*it);
					}
					catch (...) {
						list.erase(it);
					}
				}
			}
			return list;
		}

		bool isGameLoaded() { return this->currGame != nullptr; }

		void loadGame(Filesystem::pathT& path) {
			try {
				this->currGame = std::make_unique<GameLoaderT>(path, Loader::Code::OPEN);
				this->unloadGame(Code::CLOSE);
			}
			catch (const Exceptions::Exception& e) {
				std::cout << e.what() << std::endl;
			}
		}

		void unloadGame(const Code& typeOpen) {
			if (!isGameLoaded())
				throw Exceptions::programming_error("No game is opened.");

			if (typeOpen == Code::SAVE)
				this->currGame->writeData();
			this->currGame.release();
		}

		void createGame(const Models::Game& data) {			
			try {
				if (isGameLoaded()) unloadGame(Code::SAVE);
				createTmpFile(data);
				this->currGame->renameData(getGameLoaderFileName());
			}
			catch (const Exceptions::Exception &e) {
				throw Exceptions::create_game_failed(e.what());
			}
		}

		void addSnapshot(const STRING& nameSnapshot, const Filesystem::pathT& pathSnapshot) {
			if (this->currGame == nullptr)
				throw Exceptions::programming_error("No game is opened.");
			try {
				auto snapshot = new Models::Snapshot(nameSnapshot, pathSnapshot);
				createTmpFile(*snapshot);
				this->idxSnapshot->renameData(getSnapshotLoaderFileName());

				// TODO : Add Snapshot to Map

				delete snapshot;
			}
			catch (const Exceptions::Exception& e) {
				throw Exceptions::add_snapshot_failed(e.what());
			}
		}
	};
}

#endif