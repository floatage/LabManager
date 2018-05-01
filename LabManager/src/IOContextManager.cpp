#include "IOContextManager.h"

IOContextManager::IOContextManager()
	:hsLoop(1), ioLoop(1)
{
}

IOContextManager::~IOContextManager()
{
}

IOContextManager* IOContextManager::getInstance(void)
{
	static IOContextManager instance;
    return &instance;
}

void IOContextManager::init()
{
}

void IOContextManager::run()
{
	std::thread hsLoopThread([](io_context& loop) {
		auto dummy_work(new io_context::work(loop));
		loop.run(); 
	}, std::ref(hsLoop));

	std::thread ioLoopThread([](io_context& loop) { 
		auto dummy_work(new io_context::work(loop));
		loop.run(); 
	}, std::ref(ioLoop));

	workers.push_back(std::move(hsLoopThread));
	workers.push_back(std::move(ioLoopThread));
}

void IOContextManager::wait()
{
	for (auto& t : workers) {
		t.join();
	}
}
