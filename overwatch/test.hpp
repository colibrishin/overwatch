#ifndef TEST_H
#define TEST_H

#include "data/manager.hpp"

namespace Test {
	std::unique_ptr<Manager::Manager> CreateDummyGame(std::unique_ptr<Manager::Manager> manager);
	int main(void);
}
#endif