#include "fork.hpp"
#include <unistd.h>
#include "../devlib/log.hpp"
#include "../devlib/string.hpp"
#include "../devlib/configuration.hpp"
void boot::fork()
{
    //Set the default configuration value if it hasn't been initialized
    if(dev::getCFG()["fork"].size() == 0)
    {
        dev::getCFG()["fork"] = "true";
        dev::getCFG().flush();
    }

    //Check if forking is allowed!
    if(dev::getCFG().tryBool("fork"))
    {
        pid_t pid = ::fork();
        if(pid < 0)
        {
            dev::getLogger().put("Attempt at fork() failed! This program will terminate now!");
            throw dev::exception("ERR_FORK_FAILED", 0);
        }
        else if(pid == 0)
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            dev::getLogger().put("Fork successful! My PID is " + dev::toString(getpid()) + " and my pgid is " + dev::toString(getpgid));
            return;
        }
    }
}
