#include <stdlib.h>

#include <utils/RefBase.h>
#include <utils/Log.h>
#include <binder/TextOutput.h>

#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <binder/MemoryHeapBase.h>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include "binder_client_05.h"


IMPLEMENT_META_INTERFACE(Calculator, "Calculator_Demo");

int main(int argc, char* argv[])
{
    sp<IBinder> binder = defaultServiceManager()->getService(String16("Calculator"));
    if(binder == nullptr)
    {
        std::cout << "Binder NULL" << std::endl;
        return -1;
    }
    sp<ICalculator> calculator = interface_cast<ICalculator>(binder);
    std::cout << "Add (3, 2) = " << calculator->add(3, 2) << std::endl;
    std::cout << "Sub (3, 2) = " << calculator->sub(3, 2) << std::endl;
    std::cout << "Mul (3, 2) = " << calculator->mul(3, 2) << std::endl;
    std::cout << "Div (3, 2) = " << calculator->div(3, 2) << std::endl;
    std::cout << "Mod (3, 2) = " << calculator->mod(3, 2) << std::endl;
    
    // Reading data from shared memory
    char buffer[1024];
    std::cout << "Buf size = " << calculator->buf(1024, buffer) << std::endl;
    std::cout << "Buffer contents = " << buffer << std::endl;

    return 0;
}
