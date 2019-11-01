#include <utils/Thread.h>
#include <binder/ProcessState.h>
#include <binder/IPCThreadState.h>
#include <iostream>

using namespace android;

class ThreadTest : public Thread
{
public:
    ThreadTest() : Thread(false), mCounter(0) {}

    void print()
    {
        std::cout << __func__ << std::endl;
    }

    virtual bool threadLoop()
    {
        std::cout << "Loop " << mCounter << std::endl;
        ++mCounter;
        return true;
    }
private:
    int32_t mCounter;
};

int main(void)
{
    std::cout << "Thread Test" << std::endl;

    sp<ThreadTest> t = new ThreadTest();
    t->print();
    status_t res = t->run(String8::format("KK-Thread").string());
    if(res != OK) {
        std::cerr << "Error in starting thread" << std::endl;
    }

    while(true);

    return 0;
}
