#include <ios>
#include <iostream>
#include <cstring>
#include <filesystem>

#include "database.hpp"
#include "utility.hpp"

template<class T>
Database::Instance<T>::Instance() {
	std::memset((char*)(&this->data), 0, sizeof this->data);
	UT_memset(this->name, 0, sizeof(this->name));
	SHA256::calc_hash((char*)(&this->data), this->chksum);
}

template<class T>
Database::Instance<T>::Instance(const Instance<T>& _ref) {
	this->data = _ref.data;
	UT_NULLbreakCopy(this->name, _ref.name, sizeof(this->name));
	UtilityString::_charNULLbreakCopy(this->chksum, _ref.chksum, sizeof(this->chksum));
}

template<class T>
Database::Instance<T>& Database::Instance<T>::operator=(const Instance<T>& _ref) {
	this->data = _ref.data;
	UT_NULLbreakCopy(this->name, _ref.name, sizeof(this->name));
	UtilityString::_charNULLbreakCopy(this->chksum, _ref.chksum, sizeof(this->chksum));
	return *this;
}

template<class T>
Database::Instance<T>::Instance(const T& val, const Database::Types::NameInstanceT _nameInstance) {
	this->data = val;
	UT_NULLbreakCopy(this->name, _nameInstance, sizeof(this->name));
	SHA256::calc_hash((char*)(&this->data), this->chksum);
}

Database::Filesystem::Filesystem(const pathT& path, Database::Enums::FILE_MODE mode) {
	this->isFileExist = this->isPathExist = false;
	this->fileMode = mode;
	this->path = path;
	if (mode == Database::Enums::FILE_MODE::NULL_MODE) {
		this->path = "";
		this->fileMode = mode;
		return;
	} if (mode & Database::Enums::FILE_MODE::READ_MODE) {
		this->isPathExist = std::filesystem::exists(path.parent_path());
		this->isFileExist = std::filesystem::exists(path);
	} if (mode & Database::Enums::FILE_MODE::WRITE_MODE) {
		if (!std::filesystem::exists(path.parent_path()))
			std::filesystem::create_directories(path.parent_path());
		this->isFileExist = std::filesystem::exists(path);
		this->isPathExist = true;
// FILESYSTEM EXP
	}
}

void Database::Filesystem::moveFile(const pathT & _newfileName) {
	std::filesystem::rename(this->path, _newfileName);
// FILESYSTEM EXP
	this->path = _newfileName;
}

template<class T>
void Database::Loader<T>::loadFile(const std::streamsize posSeek){
	Instance<T> tmp;
	std::streamsize sz;
	
// EXP
	if (!this->doesFileExist()) return;
	this->f.open(this->getfilePath(), std::ios::binary);
// EXP
	if (!this->f.is_open()) return;
	this->f.seekg(0, std::ios::end);
	sz = this->f.tellg();
// EXP
	if (sz - posSeek < sizeof(Instance<T>)) return;
	this->f.seekg(posSeek, std::ios::beg);
	this->f.read((char*)(&tmp), sizeof(Instance<T>));

	this->instance = tmp;

	this->isLoaded = true;
	this->f.close();
}

template<class T>
void Database::Typist<T>::writeFile(const Instance<T>& _ref, const std::streamsize _posSeek) {

	// EXP
	if (!this->doesFileExist()) this->f.open(this->getfilePath(), (std::ios::binary | std::ios::out));
	else this->f.open(this->getfilePath(), (std::ios::binary | std::ios::in | std::ios::out));
// EXP
	if (!this->f.is_open()) return;
	this->f.seekp(_posSeek, std::ios::beg);
// EXP
	this->f.write((char*)(&_ref), sizeof(_ref));
	this->setFileExist(this->isWriteSuccess = true);
	this->f.close();
}

template<class T>
Database::Validator<T>::Validator(const Database::Instance<T>& val){
	this->isValid = false;
	SHA256::Hash tmpHash;

	SHA256::calc_hash((const char*)(&val.data), tmpHash);
	if (SHA256::compare_hash(val.chksum, tmpHash)) this->isValid = true;
}

template<class T>
bool Database::Schema<T>::_isGoodtoGo(Database::Enums::FILE_MODE mode) noexcept {
	this->good2go = false;
	if (mode & Database::Enums::FILE_MODE::READ_MODE) {
		if (this->Filesystem::doesFileExist() && this->Loader<T>::doesfileLoaded()) this->good2go = true && this->isfileValid();
	} if (mode & Database::Enums::FILE_MODE::WRITE_MODE) {
		if (this->Filesystem::doesFileExist()) this->good2go = true && this->isfileValid();
	}
	return this->good2go;
}

template<class T>
void Database::Schema<T>::excludeFromDatabase(const pathT& _pathExcludeFolder) {
	this->Filesystem::renameFileORFolder(_pathExcludeFolder);
}

void Data::Models::Game::loadMap(const Database::Instance<Data::Types::ctxMap>& dataMap) {
	for (int i = 0; i < dataMap.data.occupied; ++i) {
		this->mapSnapshot.insert({ dataMap.data.hashSnapshot[i],
			dataMap.data.nameSave[i] });
	}
	// Add EXCEPTION
}

const bool Data::Models::Snapshot::good() const noexcept {
	// Standalone block
	if (this->parentGame == nullptr) return false;
	return (this->getGameHash() == this->parentGame->getHash()) && this->isLoaded; 
}

bool Data::Models::Game::validateMap() {
	Snapshot tmp;
	unsigned long long count = 0;
	for (auto& it : this->mapSnapshot) {
		tmp.loadSnapshot(pathT(Data::Constants::SNAPSHOT_FOLDER).append(it.second.value).concat(L"_").concat(it.first.hash).concat(Data::Constants::DATA_FILE_EXTENSION), this);
// Placeholder
		if (!tmp.good()) return false;
		else ++count;
	}
	return (count == this->mapSnapshot.size());
}

void Data::Models::Game::buildMap() {
	Snapshot tmpSnap;
	Data::Types::ctxMap tmpMap;
	unsigned long long count = 0;
	pathT path;

	for (auto& it : std::filesystem::directory_iterator(Data::Constants::SNAPSHOT_FOLDER)) {
		if (it.path().extension() == Data::Constants::DATA_FILE_EXTENSION) {
			tmpSnap.loadSnapshot(it.path(), this);
			if (tmpSnap.good()) {
				if (it.path().filename() != (path = pathT(tmpSnap.getNameSave()).concat(L"_").concat(tmpSnap.getHash()).concat(Data::Constants::DATA_FILE_EXTENSION)))
					this->info->moveFile(Data::Constants::SNAPSHOT_FOLDER / path);

				tmpMap.dateAdded[count++] = tmpSnap.getTimeAdded();
				std::memcpy(tmpMap.hashSnapshot[count], tmpSnap.getHash(), sizeof(tmpMap.hashSnapshot[count]));
				_wmemcpy(tmpMap.nameSave[count], tmpSnap.getNameSave(), sizeof(tmpMap.nameSave[count]));
			}
		}
	}
	tmpMap.occupied = count;

	auto tmpInst = Database::Instance<Types::ctxMap>(tmpMap, this->getName());
	Database::Schema<Types::ctxMap>(tmpInst, this->info->getfilePath(), sizeof Database::Instance<Types::ctxGame>);
}

void Data::Models::Game::createGame(const Data::Types::ctxGame& _val, const Database::Types::NameInstanceT _nameGame) {
	pathT new_path = pathT(Data::Constants::GAME_FOLDER) / _nameGame;
	this->info = std::make_shared <Database::Schema<Types::ctxGame>>(Database::Instance<Types::ctxGame>(_val, _nameGame), new_path += Data::Constants::DATA_FILE_EXTENSION);
	this->isLoaded = this->info->isGoodtoGo();
	this->info->moveFile(new_path = pathT(Data::Constants::GAME_FOLDER).append(this->getName()).concat(L"_").concat(this->getHash()).concat(Data::Constants::DATA_FILE_EXTENSION));

	auto tmp = new Database::Schema<Types::ctxMap>(Database::Instance<Types::ctxMap>(), new_path, sizeof Database::Instance<Types::ctxGame>);
}

void Data::Models::Game::loadGame(const pathT& _targetName) {
	pathT new_path = pathT(Data::Constants::GAME_FOLDER) / _targetName;
	this->info = std::make_shared<Database::Schema<Types::ctxGame>>(new_path += Data::Constants::DATA_FILE_EXTENSION);
	if (this->isLoaded = this->info->isGoodtoGo()) {
		if (this->info->getfilePath() != (new_path = pathT(Data::Constants::GAME_FOLDER).append(this->getName()).concat(L"_").concat(this->getHash()).concat(Data::Constants::DATA_FILE_EXTENSION)))
			this->info->moveFile(new_path);
		this->loadMap(Database::Schema<Types::ctxMap>(new_path, sizeof Database::Instance<Types::ctxGame>).readInstance());
		if (!this->validateMap()) {
			std::cout << "WARNING : Mismatch between database and snapshot tracker found. Rebuilding snapshot tracker..." << std::endl;
			this->buildMap();
		}
	}
}

void Data::Models::Snapshot::createSnapshot(const Data::Types::ctxSnapshot& _val, Game* _parentGame) {
	pathT new_path = pathT(Data::Constants::SNAPSHOT_FOLDER) / _val.nameSave;
	this->info = std::make_shared <Database::Schema<Types::ctxSnapshot>>(Database::Instance<Types::ctxSnapshot>(_val, _val.nameSave), new_path += Data::Constants::DATA_FILE_EXTENSION);
	this->isLoaded = this->info->isGoodtoGo();
	this->info->moveFile(new_path = pathT(Data::Constants::SNAPSHOT_FOLDER).append(this->getNameSave()).concat(L"_").concat(this->getHash()).concat(Data::Constants::DATA_FILE_EXTENSION));
	this->parentGame = _parentGame;
}

void Data::Models::Snapshot::loadSnapshot(const pathT& _targetName, Game* _parentGame) {
	pathT new_path = Data::Constants::SNAPSHOT_FOLDER / _targetName;
	this->info = std::make_shared<Database::Schema<Types::ctxSnapshot>>(new_path += Data::Constants::DATA_FILE_EXTENSION);
	if (this->isLoaded = this->info->isGoodtoGo()) {
		if (this->info->getfilePath() != (new_path = pathT(Data::Constants::SNAPSHOT_FOLDER).append(this->getNameSave()).concat(L"_").concat(this->getHash()).concat(Data::Constants::DATA_FILE_EXTENSION)))
			this->info->moveFile(new_path);
		if (!this->good()) {
			std::cout << "ERROR : Orphan Snapshot Found. This will be excluded from database." << std::endl;
// TODO
			this->isLoaded = false;
		}
	}
}

void Data::Models::Game::loadSnapshot(const mapTGame::const_iterator& _ItMap) noexcept {
	std::wcout << L"NOTICE : Loading Snapshot " << _ItMap->second.value << L" for " << this->getName() << L" Started" << std::endl;
	this->currSnapshot.loadSnapshot(pathT(Data::Constants::SNAPSHOT_FOLDER).append(_ItMap->second.value).concat(L"_").concat(_ItMap->first.hash).concat(Data::Constants::DATA_FILE_EXTENSION), this);
}