#ifndef _INCLUDED_CXX_11_NS_H_
#define _INCLUDED_CXX_11_NS_H_

# if __APPLE__
  // is Mavericks
# if (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_9) || (__cplusplus >= 201103L)
  # define cxx11_ns std
  # else
  # define cxx11ns std::tr1
  # endif                                                   // OSX ver
# else                                                      // not __APPLE__
  # if (__cplusplus >= 201103L) || defined(_MSC_VER)
  # define cxx11_ns std
  # else
  # define cxx11_ns std::tr1
  # endif
# endif                                                     // __APPLE__


#endif // _INCLUDED_CXX_11_NS_H_
