#include "manager.hpp"
#include "../macros/utility.hpp"

STRING Manager::Manager::getTmpFileName(const std::time_t timeCreated, const Hash::SHA256 hash) const noexcept {
	Filesystem::pathT pathFile;
	pathFile.concat(std::to_string(timeCreated)).concat("_").concat(hash.getString()).concat(TMP_FILE_EXTENSION);
	return pathFile.native();
}
Filesystem::pathT Manager::Manager::getGameLoaderFileName() const {
	if (this->currGame == nullptr) throw Exceptions::no_game_loaded("");
	Filesystem::pathT pathFile = GAME_DIRECTORY;
	auto hashGame = this->currGame->getHash().getString();
	auto nameGame = this->currGame->getData().nameGame;
	pathFile.concat(hashGame).concat("_").concat(nameGame).concat(GAME_FILE_EXTENSION);

	return pathFile;
}
Filesystem::pathT Manager::Manager::getSnapshotLoaderFileName() const {
	if (this->currSnapshot == nullptr) throw Exceptions::no_game_loaded("");
	Filesystem::pathT pathFile = SNAPSHOT_DIRECTORY;
	auto hashSnapshot = this->currSnapshot->getHash().getString();
	auto nameSnapshot = this->currSnapshot->getData().nameSave;
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
void Manager::Manager::createTmpFile(const Models::Map& data) {
	try {
		auto hashableMap = data.getHashable();

		this->currSnapshotMap = std::make_unique<MapLoaderT>(currGame->getPath(), GAMEDATASIZE, Loader::Code::WRITE);
		this->currSnapshotMap->setData(data);
		this->currSnapshotMap->writeData();
	}
	catch (const Exceptions::Exception& e) {
		throw e;
	}
}
void Manager::Manager::createTmpFile(const Models::Game& data) {
	auto timeCreated = Utility::getUnixTime();
	Filesystem::pathT pathFile = GAME_DIRECTORY;
	auto hashableGame = data.getHashable();

	pathFile.concat(getTmpFileName(timeCreated, hashableGame->getHash()));

	try {
		this->currGame = std::make_unique<GameLoaderT>(pathFile, Loader::Code::NEW);
		this->currGame->setData(data);
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

		this->currSnapshot = std::make_unique<SnapshotLoaderT>(pathFile, Loader::Code::NEW);
		this->currSnapshot->setData(snapshot);
		this->currSnapshot->writeData();
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
		dirSnapshotCopy = Filesystem::Filesystem(SNAPSHOT_COPY_DIRECTORY, Filesystem::Code::CREATE);
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
}
Filesystem::listPathT Manager::Manager::iterateValidGameData() noexcept {
	auto list = dirGame.iteratePath();

	for (auto it = list.begin(); it != list.end(); ++it) {
		if (it->extension() == GAME_FILE_EXTENSION) {
			try {
				this->loadTemporaryGame(*it);
			}
			catch (...) {
				list.erase(it);
			}
		}
	}
	if(isTmpGameLoaded()) unloadTemporaryGame();

	return list;
}
Filesystem::listPathT Manager::Manager::iterateGamePaths() noexcept {
	return dirGame.iteratePath();
}
bool Manager::Manager::isGameLoaded() const noexcept { return this->currGame != nullptr; }
bool Manager::Manager::isTmpGameLoaded() const noexcept { return this->tmpGame != nullptr; }
void Manager::Manager::moveCurrentToTmp() {
	this->tmpGame = std::move(this->currGame);
	this->tmpSnapshotMap = std::move(this->currSnapshotMap);
}
void Manager::Manager::moveTmpToCurrent() {
	this->currGame = std::move(this->tmpGame);
	this->currSnapshotMap = std::move(this->tmpSnapshotMap);
}
STRING Manager::Manager::getCurrentGameName() const {
	if (!isGameLoaded())
		throw Exceptions::no_game_loaded("");

	STRING nameGame = this->currGame->getData().nameGame;
	return nameGame;
}
void Manager::Manager::appendCurrentSnapshotToMap() {
	this->mapSnapshots.insert({ currSnapshot->getHash(), currSnapshot->getData() });
}
const Models::Game Manager::Manager::getCurrentGame() {
	if (!isGameLoaded()) {
		throw Exceptions::no_game_loaded("");
	}
	return Models::Game::Game(this->currGame->getData());
}
void Manager::Manager::addSnapshot(Models::Snapshot& data) {
	if (!isGameLoaded()) {
		throw Exceptions::no_game_loaded("");
	}
	try {
		Filesystem::pathT path = data.getData().pathSave;
		STRING originalExtension = path.extension();
		STRING newExtension = getCurrentGame().getData().strExtenison;

		if (originalExtension != newExtension) {
			throw Exceptions::invalid_input("Save file extension is mismatch with defined extension in game.");
		}

		createTmpFile(data);
		this->currSnapshot->renameData(getSnapshotLoaderFileName());
		appendCurrentSnapshotToMap();
	}
	catch (const Exceptions::Exception& e) {
		throw Exceptions::add_snapshot_failed(e.what());
	}
}
void Manager::Manager::loadTemporaryGame(Filesystem::pathT& path) {
	try {
		if(!isTmpGameLoaded()) moveCurrentToTmp();
		this->currGame = std::make_unique<GameLoaderT>(path, Loader::Code::OPEN);
		this->currSnapshotMap = std::make_unique<MapLoaderT>(path, GAMEDATASIZE, Loader::Code::OPEN);
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}
Manager::Manager::~Manager()
{
	unloadMap();
	this->unloadGame(Code::SAVE);
}
void Manager::Manager::unloadTemporaryGame() {
	try {
		this->currGame->writeData();
		this->currGame.release();

		this->currSnapshotMap->writeData();
		this->currSnapshotMap.release();

		moveTmpToCurrent();
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}
void Manager::Manager::loadGame(Filesystem::pathT& path) {
	try {
		if(isGameLoaded())
			this->unloadGame(Code::SAVE);
		this->currGame = std::make_unique<GameLoaderT>(path, Loader::Code::OPEN);
		loadMap(path);
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}
void Manager::Manager::unloadGame(const Code& typeOpen) {
	if (!isGameLoaded())
		throw Exceptions::no_game_loaded("");

	if (typeOpen == Code::SAVE) {
		this->currGame->writeData();
		this->currSnapshotMap->writeData();
	}

	this->currGame.release();
	this->currSnapshotMap.release();
}
void Manager::Manager::loadMap(Filesystem::pathT& path) {
	try {
		if (!isGameLoaded()) {
			throw Exceptions::no_game_loaded("");
		}
		this->currSnapshotMap = std::make_unique<MapLoaderT>(path, GAMEDATASIZE, Loader::Code::OPEN);
		this->mapSnapshots = Models::Map(this->currSnapshotMap->getData()).getMapAsGeneric();
	}
	catch (const Exceptions::Exception& e) {
		std::cout << e.what() << std::endl;
	}
}
void Manager::Manager::updateMap() const
{
	if (!isGameLoaded()) {
		throw Exceptions::no_game_loaded("");
	}
	Models::Map tmpMap(this->mapSnapshots);

	this->currSnapshotMap->setData(tmpMap);
	this->currSnapshotMap->writeData();
}
void Manager::Manager::unloadMap() {
	try {
		if (!isGameLoaded()) {
			throw Exceptions::no_game_loaded("");
		}
		updateMap();
		this->currSnapshotMap->writeData();
		this->currSnapshotMap.release();
	}
	catch (const Exceptions::Exception& e) {
		throw e;
	}
}
void Manager::Manager::createGame(const Models::Game& data) {
	auto tmpMap = new Models::Map();
	try {
		if (isGameLoaded()) moveCurrentToTmp();

		createTmpFile(data);
		createTmpFile(*tmpMap);
		this->currGame->renameData(getGameLoaderFileName());

		moveTmpToCurrent();
	}
	catch (const Exceptions::Exception& e) {
		throw Exceptions::create_game_failed(e.what());
	}
}
Manager::NameAndPathListT Manager::Manager::listGames(){
	NameAndPathListT files;
	Filesystem::listPathT paths = iterateGamePaths();

	for (auto& it: paths) {
		try {
			loadTemporaryGame(it);

			files.first.push_back(this->currGame->getData().nameGame);
			files.second.push_back(it);
		}
		catch (...) { }
	}
	if(isTmpGameLoaded()) unloadTemporaryGame();

	return files;
}
void Manager::Manager::removeGame(Filesystem::pathT& path) {
	// Possible security problem
	try {
		loadTemporaryGame(path);
		if (isGameLoaded()) {
			if (isTmpGameLoaded()) {
				if (this->currGame->getHash() == this->tmpGame->getHash()) {
					// If Current game and Tmp game both are same, close both of them.
					unloadGame(Code::CLOSE);
					moveTmpToCurrent();
					unloadGame(Code::CLOSE);
				}
			}
			else {
				unloadGame(Code::CLOSE);
				moveTmpToCurrent();
			}
		}

		Filesystem::Filesystem fs(path, Filesystem::Code::CHECK);
		fs.remove();
	}
	catch (const Exceptions::Exception& e) {
		throw e;
	}
}
const Models::MiniSnapshotMapT& Manager::Manager::listSnapshots() const
{
	if (!isGameLoaded()) {
		throw Exceptions::no_game_loaded("");
	}
	return mapSnapshots;
}
