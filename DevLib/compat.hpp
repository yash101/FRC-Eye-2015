#ifndef COMPAT_HPP
#define COMPAT_HPP
//POSIX detection, now made simple!
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
#include <unistd.h>
#if defined(_POSIX_VERSION)
#define POSIX
#else
#undef POSIX
#endif
#endif
#if __cplusplus > 199711L
#define CXX11_SUPPORTED
#endif
#endif // COMPAT_HPP
