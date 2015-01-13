//#include "../thread.hpp"

//#ifdef POSIX
//#ifndef CXX11_SUPPORTED
//#include <pthread.h>
//#endif
//#endif
//#ifdef CXX11_SUPPORTED
//#include <thread>
//#include <functional>

////If C++11 Threading is supported, use it!
//#ifdef CXX11_SUPPORTED
//dev::Thread::Thread(std::function<void ()> func)
//{
//    std::thread(func);
//}

//dev::Thread::Thread(void (*func)())
//{
//    std::thread([&](){func();});
//}

//#endif
