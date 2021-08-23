#include "models.hpp"

Models::Game::Game(const Data::Structures::Game originalData) {
	Game(originalData.nameGame, originalData.nameProc, originalData.pathGame, originalData.pathSave, originalData.strExtenison);
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

Models::Snapshot::Snapshot(const Data::Structures::Snapshot originalData) {
	Snapshot(originalData.nameSave, originalData.dateAdded, originalData.hashGame, originalData.hashSnapshot, originalData.pathSave, originalData.pathCopy);
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