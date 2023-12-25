#ifndef DEFINES_HPP
#define DEFINES_HPP

// Debug logger
#ifdef DEBUG // In debug mode, perform a test on any action

#include <iostream>
#define CHECK_EXPR(expr) if( ! expr ) { std::cout << "Caught a negative result in the file: " << __FILE__ << "; Line: " << __LINE__ << '\n'; }

#else // Else, we don't add any overhead

#define CHECK_EXPR(expr) (expr)

#endif

#endif // !DEFINES_HPP