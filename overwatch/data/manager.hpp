#ifndef MANAGER_H
#define MANAGER_H

#include <memory>
#include <list>
#include <iostream>

#include "loader.hpp"
#include "../macros/global.hpp"
#include "../exception.hpp"

namespace Manager {
	using GameLoaderT = Loader::Loader<Data::Structures::Game>;
	using SnapshotLoaderT = Loader::Loader<Data::Structures::Snapshot>;
	using MapLoaderT = Loader::Loader<Data::Structures::Map>;

	class Manager {
		GameLoaderT* currGame;
		MapLoaderT* idxSnapshot;
		SnapshotLoaderT* currSnapshot;

		Filesystem::Filesystem dirGame;
		Filesystem::Filesystem dirSnapshot;
		
		Manager() : dirGame(GAME_DIRECTORY, Filesystem::Code::CHECK), dirSnapshot(SNAPSHOT_DIRECTORY, Filesystem::Code::CHECK) {
			// I am sorry
			try {
				dirGame = Filesystem::Filesystem(GAME_DIRECTORY, Filesystem::Code::CREATE);
				dirSnapshot = Filesystem::Filesystem(SNAPSHOT_DIRECTORY, Filesystem::Code::CREATE);
			}
			catch (const std::filesystem::filesystem_error& e) {
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

			iterateGameData();
		}

		std::list<Filesystem::pathT> iterateGameData() {
			auto list = dirGame.iteratePath();

			for (auto &it : list) {
				if (it.extension() == GAME_FILE_EXTENSION)
					loadGame(it);
			}
		}

		void loadGame(Filesystem::pathT& path) {
			try {
				this->currGame = new GameLoaderT(path, Loader::Code::OPEN);
			}
			catch (const Exceptions::Exception& e) {
				std::cout << e.what() << std::endl;
			}
		}
	};

}

#endif