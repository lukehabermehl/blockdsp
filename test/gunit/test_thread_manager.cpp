#include "gtest/gtest.h"
#include "blockdsp.h"
#include <aputils/autil_thread_manager.hpp>
#include <aputils/aputils.h>

class ThreadManagerTestFixture : public ::testing::Test 
{
public:
	int GetInt() {
		int ret = 0;
		mtx.lock();
		ret = value;
		mtx.unlock();
		return value;
	}
	void Increment() {
		mtx.lock();
		value += 1;
		mtx.unlock();
	}

	static void ThreadWokerFunc(void *ctx) {
		ThreadManagerTestFixture *fixture = (ThreadManagerTestFixture *)ctx;
		fixture->Increment();
	}

protected:
	std::mutex mtx;
	int value;

	void SetUp() {
		value = 0;
	}
};

TEST_F(ThreadManagerTestFixture, test_thread_dispatch)
{
	int count = 4;
	for (int i=0; i<count; i++) {
		AUtilDispatchThread(ThreadManagerTestFixture::ThreadWokerFunc, this);
	}

	sleep(2);
	EXPECT_EQ(count, GetInt());
}