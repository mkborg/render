/*
    Define "_GNU_SOURCE" to enable "%a" scanf extension:
    gcc -O2 -o consume_memory consume_memory.c -D_GNU_SOURCE
*/

#include <stdio.h>	// fprintf(), stderr, stdout, sscanf(), getchar()
#include <stdlib.h>	// malloc(), free(), abort()

#define PAGE_SIZE 0x1000

int parse_long_long(long long *p_ll, const char *s)
{
  char *p_suffix = NULL;
  int result;
  int success = 0; // 0 == OK, -1 == ERROR

  *p_ll = 0;
  fprintf(stdout, "s='%s'\n", s);

  result = sscanf(s, "%Li%m[kKmMgG]", p_ll, &p_suffix);
  //fprintf(stdout, "result=%i\n", result);
  if (2 == result) { // number with suffix[es]
    char *p = p_suffix;
    char c;

    if (NULL == p_suffix) { // this must not happen
	abort();
    }
    fprintf(stdout, "result=%d ll=%016LX/%Li p_suffix=%p/'%s'\n", result, *p_ll, *p_ll, p_suffix, p_suffix);

    for(; 0 != (c = *p); ++p) {
      switch (c) {
        case 'k':
        case 'K':
          *p_ll *= 1024;
          break;

        case 'm':
        case 'M':
          *p_ll *= (1024*1024);
          break;

        case 'g':
        case 'G':
          *p_ll *= (1024*1024*1024);
          break;

        default:
          fprintf(stderr, "unsupport suffix '%c'\n", c);
	  success = -1;
          break;
      }
    }
  } else if (1 != result) { // error
    fprintf(stderr, "string '%s' is not a number\n", s);
    success = -1;
  }

  if (p_suffix) {
    free(p_suffix);
  }

  fprintf(stdout, "ll=%016LX/%Li\n", *p_ll, *p_ll);
  return success;
}

int main(int argc, char *argv[])
{
    char *p_mem;
    char *p;

    if (2!=argc) {
	fprintf(stderr, "Usage: %s mem_size\n", argv[0]);
	return -1;
    }
    char *mem_size_s = argv[1];
    unsigned long long mem_size_ull;
    if (0 != parse_long_long(&mem_size_ull, mem_size_s)) {
	// message was already printed in parse_long_long()
	return -1;
    }
    p_mem = p = malloc(mem_size_ull);
    if (NULL == p) {
	fprintf(stderr, "malloc(%llu) failure\n", mem_size_ull);
	return -1;
    }

    // modify one byte of each page of allocated virtual memory to make OS map it to physical memory
    for(;;) {
	*p = 0;
	if (mem_size_ull < PAGE_SIZE) {
	    break;
	} else {
	    p += PAGE_SIZE;
	    mem_size_ull -= PAGE_SIZE;
	}
    }
    fprintf(stdout, "memory has been allocated, hit [enter] key to release it\n");
    getchar(); // wait for user
    free(p_mem);
    fprintf(stdout, "finished\n");
    return 0;
}
