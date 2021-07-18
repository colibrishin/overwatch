#include "models.hpp"

Models::Game::Game(const STRING& nameGame, const STRING& nameProc, const STRING& pathGame, const STRING& pathSave, const STRING& ExtensionOfSave) {
	setName(nameGame);
	setProcessName(nameProc);
	setPathToGame(pathGame);
	setPathToSaveFiles(pathSave);
	setExtensionOfSave(ExtensionOfSave);
}

void Models::Game::setName(const STRING & nameGame) noexcept {
	MEMNULLCPY_S(data.nameGame, sizeof(data.nameGame) / sizeof(wchar_t) , nameGame.c_str(), nameGame.size() + 1);
}
void Models::Game::setProcessName(const STRING& nameProc) noexcept {
	MEMNULLCPY_S(data.nameProc, sizeof(data.nameProc) / sizeof(wchar_t), nameProc.c_str(), nameProc.size() + 1);
}
void Models::Game::setPathToGame(const STRING& pathGame) noexcept {
	MEMNULLCPY_S(data.pathGame, sizeof(data.pathGame) / sizeof(wchar_t), pathGame.c_str(), pathGame.size() + 1);
}
void Models::Game::setPathToSaveFiles(const STRING& pathSave) noexcept {
	MEMNULLCPY_S(data.pathSave, sizeof(data.pathSave) / sizeof(wchar_t), pathSave.c_str(), pathSave.size() + 1);
}
void Models::Game::setExtensionOfSave(const STRING& ExtensionOfSave) noexcept {
	MEMNULLCPY_S(data.strExtenison, sizeof(data.strExtenison) / sizeof(wchar_t), ExtensionOfSave.c_str(), ExtensionOfSave.size() + 1);
}

Models::Snapshot::Snapshot(const STRING& nameSave, const Filesystem::pathT& pathSave) {
	setName(nameSave);
	setPathToFile(pathSave.native());
}

void Models::Snapshot::setName(const STRING& nameSave) noexcept {
	MEMNULLCPY_S(data.nameSave, sizeof(data.nameSave) / sizeof(wchar_t), nameSave.c_str(), nameSave.size() + 1);
}
void Models::Snapshot::setTime(const std::time_t& dateAdded) noexcept {
	data.dateAdded = dateAdded;
}
void Models::Snapshot::setHashGame(Hash::SHA256 const hashGame) noexcept {
	this->data.hashGame = hashGame;
}
void Models::Snapshot::setHashSnapshot(Hash::SHA256 const hashSnapshot) noexcept {
	this->data.hashSnapshot = hashSnapshot;
}
void Models::Snapshot::setPathToFile(const STRING& pathSave) noexcept {
	MEMNULLCPY_S(data.pathSave, sizeof(data.pathSave) / sizeof(wchar_t), pathSave.c_str(), pathSave.size() + 1);
}

void Models::Snapshot::setPathToCopyOfFile(const STRING& pathCopy) noexcept {
	MEMNULLCPY_S(data.pathCopy, sizeof(data.pathCopy) / sizeof(wchar_t), pathCopy.c_str(), pathCopy.size() + 1);
}