#include "data.hpp"

void Data::Contexts::Game::_COPY(const Game& _ref) {
	MEMNULLCPY(nameProc, _ref.nameProc, sizeof(nameProc));
	MEMNULLCPY(pathGame, _ref.pathGame, sizeof(pathGame));
	MEMNULLCPY(pathSave, _ref.pathSave, sizeof(pathSave));
	MEMNULLCPY(strExtenison, _ref.strExtenison, sizeof(strExtenison));
}

void Data::Contexts::Game::_CLEAR() {
	MEMSET(nameProc, NULL_CHAR, sizeof(nameProc));
	MEMSET(pathGame, NULL_CHAR, sizeof(pathGame));
	MEMSET(pathSave, NULL_CHAR, sizeof(pathSave));
	MEMSET(strExtenison, NULL_CHAR, sizeof(strExtenison));
}

Data::Contexts::Game::Game(const Data::Contexts::Game& _ref) {
	_COPY(_ref);
}

Data::Contexts::Game& Data::Contexts::Game::operator=(const Data::Contexts::Game& _ref) {
	_COPY(_ref);
	return *this;
}

Data::Contexts::Game::Game() {
	_CLEAR();
}