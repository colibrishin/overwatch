#include <set>

#include "models.hpp"

Models::Game::Game(const Data::Structures::Game& originalData) {
	this->data = originalData;
}

Models::Game::Game(const STRING& nameGame, const STRING& nameProc, const STRING& pathGame, const STRING& pathSave, const STRING& ExtensionOfSave) {
	setName(nameGame);
	setProcessName(nameProc);
	setPathToGame(pathGame);
	setPathToSaveFiles(pathSave);
	setExtensionOfSave(ExtensionOfSave);
}

void Models::Game::setName(const STRING & nameGame) noexcept {
	MEMNULLCPY_S(data.nameGame, sizeof(data.nameGame) / sizeof(CHAR_T) , nameGame.c_str(), nameGame.size() + 1);
}
void Models::Game::setProcessName(const STRING& nameProc) noexcept {
	MEMNULLCPY_S(data.nameProc, sizeof(data.nameProc) / sizeof(CHAR_T), nameProc.c_str(), nameProc.size() + 1);
}
void Models::Game::setPathToGame(const STRING& pathGame) noexcept {
	MEMNULLCPY_S(data.pathGame, sizeof(data.pathGame) / sizeof(CHAR_T), pathGame.c_str(), pathGame.size() + 1);
}
void Models::Game::setPathToSaveFiles(const STRING& pathSave) noexcept {
	MEMNULLCPY_S(data.pathSave, sizeof(data.pathSave) / sizeof(CHAR_T), pathSave.c_str(), pathSave.size() + 1);
}
void Models::Game::setExtensionOfSave(const STRING& ExtensionOfSave) noexcept {
	MEMNULLCPY_S(data.strExtenison, sizeof(data.strExtenison) / sizeof(CHAR_T), ExtensionOfSave.c_str(), ExtensionOfSave.size() + 1);
}

Models::Snapshot::Snapshot(const Data::Structures::Snapshot& originalData) {
	this->data = originalData;
}

Models::Snapshot::Snapshot(const STRING& nameSave, const time_t& dateAdded, Hash::SHA256 hashGame, Hash::SHA256 hashSnapshot, const STRING& pathSave, const STRING& pathCopy) {
	setName(nameSave);
	setTime(dateAdded);
	setHashGame(hashGame);
	setHashSnapshot(hashSnapshot);
	setPathToFile(pathSave);
	setPathToCopyOfFile(pathCopy);
}

void Models::Snapshot::setName(const STRING& nameSave) noexcept {
	MEMNULLCPY_S(data.nameSave, sizeof(data.nameSave) / sizeof(CHAR_T), nameSave.c_str(), nameSave.size() + 1);
}
void Models::Snapshot::setTime(const std::time_t& dateAdded) noexcept {
	data.dateAdded = dateAdded;
}
void Models::Snapshot::setHashGame(const Hash::SHA256 hashGame) noexcept {
	this->data.hashGame = hashGame;
}
void Models::Snapshot::setHashSnapshot(const Hash::SHA256 hashSnapshot) noexcept {
	this->data.hashSnapshot = hashSnapshot;
}
void Models::Snapshot::setPathToFile(const STRING& pathSave) noexcept {
	MEMNULLCPY_S(data.pathSave, sizeof(data.pathSave) / sizeof(CHAR_T), pathSave.c_str(), pathSave.size() + 1);
}

void Models::Snapshot::setPathToCopyOfFile(const STRING& pathCopy) noexcept {
	MEMNULLCPY_S(data.pathCopy, sizeof(data.pathCopy) / sizeof(CHAR_T), pathCopy.c_str(), pathCopy.size() + 1);
}

Models::Map::Map(const Data::Structures::Map& originalData)
{
	this->data = originalData;
}

Models::Map::Map(const MiniSnapshotMapT& map)
{
	unsigned int i = 0;

	for (auto& it : map) {
		this->data.mapUsed[i] = true;
		this->data.snapshots[i] = it.second;
	}
}

Data::T_SNAPSHOT_SIZE_IN_MAP Models::Map::getSize()
{
	Data::T_SNAPSHOT_SIZE_IN_MAP sz = 0;
	for (int i = 0; i < Data::MAX_SNAPSHOTS_IN_MAP; ++i) {
		sz += (Data::T_SNAPSHOT_SIZE_IN_MAP)this->data.mapUsed[i];
	}
	return sz;
}

int Models::Map::getInsertPossibleIdx() {
	for (int i = 0; i < Data::MAX_SNAPSHOTS_IN_MAP; ++i) {
		if (this->data.mapUsed[i]) {
			return i;
		}
	}
	return -1;
}

bool Models::Map::isInsertPossible()
{
	if (getInsertPossibleIdx() == -1) return true;
	return false;
}

Models::MiniSnapshotMapT Models::Map::getMapAsGeneric()
{
	MiniSnapshotMapT map;

	for (int i = 0; i < Data::MAX_SNAPSHOTS_IN_MAP; ++i) {
		if (this->data.mapUsed[i]) {
			map.insert({ this->data.snapshots[i].getHash(), this->data.snapshots[i].getValue() });
		}
	}

	return map;
}

void Models::Map::updateMap(const MiniSnapshotMapT map)
{
	this->data._CLEAR();
	int idx = 0;

	for (auto& it : map) {
		this->data.mapUsed[idx] = true;
		this->data.snapshots[idx++] = it.second;
	}
}

bool Models::Map::insertSnapshot(const Data::Structures::Snapshot& snapshot)
{
	// Map data needs to be atomic
	int idx = getInsertPossibleIdx();
	Data::Structures::MiniSnapshot* tmpMini = new Data::Structures::MiniSnapshot(snapshot);

	if (idx != -1) {
		this->data.mapUsed[idx] = true;
		this->data.snapshots[idx].setValue(*tmpMini);
		return true;
	}
	return false;
}

bool Models::Map::insertSnapshot(const Data::Structures::MiniSnapshot& snapshot)
{
	// Map data needs to be atomic
	int idx = getInsertPossibleIdx();

	if (idx != -1) {
		this->data.mapUsed[idx] = true;
		this->data.snapshots[idx].setValue(snapshot);
		return true;
	}
	return false;
}

void Models::Map::removeSnapshot(unsigned int idx) {
	if (this->getSize() <= idx) {
		_ASSERT(0);
	}

	this->data.snapshots[idx].clearValue();
	this->data.mapUsed[idx] = false;
}
