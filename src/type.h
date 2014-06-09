#ifndef _TYPE_H_
#define _TYPE_H_

typedef unsigned  char   u8;
typedef unsigned  short u16;
typedef unsigned  int   u32;
typedef unsigned  int   u4char_t;

typedef unsigned int address_t;

#ifdef _WINDOWS
typedef unsigned int pthread_t;
typedef void* HANDLE;

#define getopt  pgetopt
#define optarg  poptarg
#define optind  poptind
#define opterr  popterr
#define ftello  ftell
#define getcwd  _getcwd

#define S_ISDIR(x) x&S_IFDIR
#endif

#endif
