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

	constexpr const std::streamsize GAMEDATASIZE = sizeof(Data::Structures::Game);
	constexpr const std::streamsize SNAPSHOTDATASIZE = sizeof(Data::Structures::Snapshot);
	constexpr const std::streamsize MAPDATASIZE = sizeof(Data::Structures::Map);

	using NameAndPathListT = std::pair<std::list<STRING>, std::list<Filesystem::pathT>>;

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
		ptrMapLoaderT tmpSnapshotMap;
		ptrMapLoaderT currSnapshotMap;

		ptrSnapshotLoaderT currSnapshot;

		Models::MiniSnapshotMapT mapSnapshots;

		Filesystem::Filesystem dirGame;
		Filesystem::Filesystem dirSnapshot;
		Filesystem::Filesystem dirSnapshotCopy;
		

		STRING getTmpFileName(const std::time_t timeCreated, const Hash::SHA256 hash) const noexcept;
		Filesystem::pathT getGameLoaderFileName() const;
		Filesystem::pathT getSnapshotLoaderFileName() const;
		Filesystem::pathT getSnapshotSaveFileCopyFileName(const Data::Structures::Snapshot& data) const;
		Hash::SHA256 getSnapshotSaveFileHash(const Filesystem::pathT& pathToFile) const;
		
		void createTmpFile(const Models::Map& data);
		void createTmpFile(const Models::Game& data);
		void createTmpFile(Models::Snapshot& snapshot);

		bool isTmpGameLoaded() const noexcept;
		void moveTmpToCurrent();
		void moveCurrentToTmp();
	public:
		~Manager();
		Manager();
		Filesystem::listPathT iterateGamePaths() noexcept;
		Filesystem::listPathT iterateValidGameData() noexcept;
		bool isGameLoaded() const noexcept;
		NameAndPathListT listGames();

		void loadTemporaryGame(Filesystem::pathT& path);
		void unloadTemporaryGame();

		void loadGame(Filesystem::pathT& path);
		void unloadGame(const Code& typeOpen);
		void createGame(const Models::Game& data);
		void removeGame(Filesystem::pathT& path);

		void loadMap(Filesystem::pathT& path);
		void unloadMap();
		void updateMap() const;

		const Models::MiniSnapshotMapT& listSnapshots() const;
		void addSnapshot(Models::Snapshot& data);
		void appendCurrentSnapshotToMap();

		const Models::Game getCurrentGame();
		STRING getCurrentGameName() const;
	};
}

#endif