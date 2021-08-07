#include "manager.hpp"
#include "../macros/utility.hpp"

STRING Manager::Manager::getTmpFileName(const std::time_t timeCreated, const Hash::SHA256 hash) const noexcept {
	Filesystem::pathT pathFile;
	pathFile.concat(std::to_string(timeCreated)).concat("_").concat(hash.getString());
	return pathFile.native();
}

Filesystem::pathT Manager::Manager::getGameLoaderFileName() const {
	if (this->currGame == nullptr) throw Exceptions::programming_error("Game is not Loaded");
	Filesystem::pathT pathFile = GAME_DIRECTORY;
	auto hashGame = this->currGame->getHash().getString();
	auto nameGame = this->currGame->getData().getValue().nameGame;
	pathFile.concat(hashGame).concat("_").concat(nameGame).concat(GAME_FILE_EXTENSION);

	return pathFile;
}

Filesystem::pathT Manager::Manager::getSnapshotLoaderFileName() const {
	if (this->idxSnapshot == nullptr) throw Exceptions::programming_error("Snapshot is not Loaded");
	Filesystem::pathT pathFile = SNAPSHOT_DIRECTORY;
	auto hashSnapshot = this->idxSnapshot->getHash().getString();
	auto nameSnapshot = this->idxSnapshot->getData().getValue().nameSave;
	pathFile.concat(hashSnapshot).concat("_").concat(nameSnapshot).concat(SNAPSHOT_FILE_EXTENSION);

	return pathFile;
}

Filesystem::pathT Manager::Manager::getSnapshotSaveFileCopyFileName(const Data::Structures::Snapshot& data) const {
	Filesystem::pathT pathFile = SNAPSHOT_COPY_DIRECTORY;

	auto hashSnapshot = data.hashSnapshot.getString();
	Filesystem::pathT pathFileName = data.pathSave;
	pathFileName = pathFileName.filename();

	pathFile.concat(hashSnapshot).concat("_").concat(pathFileName.native());
	return pathFile;
}

Hash::SHA256 Manager::Manager::getSnapshotSaveFileHash(const Filesystem::pathT& pathToFile) const {
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

void Manager::Manager::createTmpFile(const Models::Game& data) {
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

void Manager::Manager::createTmpFile(Models::Snapshot& snapshot) {
	try {
		auto timeCreated = Utility::getUnixTime();
		Filesystem::pathT pathFile = SNAPSHOT_DIRECTORY;
		auto hashSaveFile = getSnapshotSaveFileHash(snapshot.getData().pathSave);
		auto hashGame = this->currGame->getHash();

		snapshot.setHashGame(hashGame);
		snapshot.setHashSnapshot(hashSaveFile);
		snapshot.setTime(timeCreated);

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
	catch (const std::filesystem::filesystem_error& e) {
		throw Exceptions::Exception(e);
	}
	catch (const Exceptions::Exception& e) {
		throw e;
	}
}

Manager::Manager::Manager() {
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

Filesystem::listPathT Manager::Manager::iterateGameData() noexcept {
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

bool Manager::Manager::isGameLoaded() { return this->currGame != nullptr; }

// Need a function that load a game temporary
void Manager::Manager::loadGame(Filesystem::pathT& path) {
	try {
		if(isGameLoaded())
			this->unloadGame(Code::SAVE);
		this->currGame = std::make_unique<GameLoaderT>(path, Loader::Code::OPEN);
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}

void Manager::Manager::unloadGame(const Code& typeOpen) {
	if (!isGameLoaded())
		throw Exceptions::programming_error("No game is opened.");

	if (typeOpen == Code::SAVE)
		this->currGame->writeData();
	this->currGame.release();
}

void Manager::Manager::createGame(const Models::Game& data) {
	try {
		if (isGameLoaded()) unloadGame(Code::SAVE);
		createTmpFile(data);
		this->currGame->renameData(getGameLoaderFileName());
	}
	catch (const Exceptions::Exception& e) {
		throw Exceptions::create_game_failed(e.what());
	}
}

void Manager::Manager::addSnapshot(const STRING& nameSnapshot, const Filesystem::pathT& pathSnapshot) {
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

std::list<STRING> Manager::Manager::listGames(Filesystem::listPathT& paths){
	std::list<STRING> filenames;
	
	for (auto& it: paths) {
		try {
			// This unload currently loaded game.
			loadGame(it);
			filenames.push_back(this->currGame->getData().getValue().nameGame);
		}
		catch (...) { }
	}

	return filenames;
}
