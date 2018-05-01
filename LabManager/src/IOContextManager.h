#ifndef IOCONTEXTMANAGER_H
#define IOCONTEXTMANAGER_H

#include "Common.h"
#include <vector>

class IOContextManager : public boost::noncopyable 
{
private:
	io_context hsLoop;
	io_context ioLoop;
	std::vector<std::thread> workers;

	~IOContextManager();

public:
	IOContextManager();

	inline io_context& getHSLoop() { return hsLoop; }
	inline io_context& getIOLoop() { return ioLoop; }

	void init();
	void run();
	void wait();

    static IOContextManager* getInstance(void);
};

#endif // !IOCONTEXTMANAGER
