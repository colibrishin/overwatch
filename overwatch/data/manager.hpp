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
		ptrGameLoaderT tmpGame;
		ptrGameLoaderT currGame;
		ptrSnapshotLoaderT idxSnapshot;
		ptrMapLoaderT currSnapshotMap;

		Filesystem::listPathT pathGames;
		std::map<STRING, Filesystem::pathT> mapSnapshots;

		Filesystem::Filesystem dirGame;
		Filesystem::Filesystem dirSnapshot;
		

		STRING getTmpFileName(const std::time_t timeCreated, const Hash::SHA256 hash) const noexcept;
		Filesystem::pathT getGameLoaderFileName() const;
		Filesystem::pathT getSnapshotLoaderFileName() const;
		Filesystem::pathT getSnapshotSaveFileCopyFileName(const Data::Structures::Snapshot& data) const;
		Hash::SHA256 getSnapshotSaveFileHash(const Filesystem::pathT& pathToFile) const;
		void createTmpFile(const Models::Game& data);
		void createTmpFile(Models::Snapshot& snapshot);

		bool isTmpGameLoaded() const noexcept;
		void moveTmpToCurrent();
		void moveCurrentToTmp();
	public:
		Manager();
		Filesystem::listPathT iterateGameData() noexcept;
		bool isGameLoaded() const noexcept;
		std::list<STRING> listGames(Filesystem::listPathT& path);

		void loadTemporaryGame(Filesystem::pathT& path);
		void unloadTemporaryGame();

		void loadGame(Filesystem::pathT& path);
		void unloadGame(const Code& typeOpen);
		void createGame(const Models::Game& data);
		void addSnapshot(const STRING& nameSnapshot, const Filesystem::pathT& pathSnapshot);
		void removeGame(Filesystem::pathT& path);
	};
}

#endif