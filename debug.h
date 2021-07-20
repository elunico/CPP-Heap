#ifndef DEBUG_H

#ifndef NDEBUG

#include <iostream>
#include <string>

#define filename                  \
  auto s = std::string(__FILE__); \
  auto idx = s.find_last_of('/'); \
  if (idx + 1 != s.npos) {        \
    s = s.substr(idx + 1);        \
  } else {                        \
    s = "<empty>";                \
  }

#define DEBUG_PREFIX \
  filename std::cerr << "[DEBUG " << s << ":" << __LINE__ << "]: "

#define SHOW_EXPR(x) \
  { DEBUG_PREFIX << #x << " = " << x << std::endl; }

#define SHOW_STR(v) \
  { DEBUG_PREFIX << "Message: " << #v << std::endl; }

#else

#define SHOW_STR(x) (void*)0;
#define SHOW_EXPR(x) (void*)0;
#define filename
#define DEBUG_PREFIX

#endif  // NDEBUG

#endif  // DEBUG_H
