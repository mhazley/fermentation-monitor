#ifndef FW_VERSION_H
#define FW_VERSION_H

#define ST_HELPER(x)      #x
#define ST(x)             ST_HELPER(x)

#define FW_VERSION_MAJOR  0
#define FW_VERSION_MINOR  2
#define FW_VERSION_PATCH  0

#define FW_VERSION_STR    ( ST(FW_VERSION_MAJOR) "." ST(FW_VERSION_MINOR) "." ST(FW_VERSION_PATCH) )

#endif // FW_VERSION_H
