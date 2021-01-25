
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

#include "dataseg.h"
#include "memmgr.h"



#define MAX 4096
#define BS 32

/// @brief print a log message if level <= mm_loglevel. The variadic argument is a printf format
///        string followed by its parametrs
#define LOG(level, ...) mm_log(level, __VA_ARGS__)
#define ROUND_UP(w)        (((w) + BS - 1)/BS*BS)      ///< round up to the multiple of BS - to avoid padding when malloc() allocates memory
static int  mm_loglevel    = 1;                        ///< log level (0: off; 1: info; 2: verbose)
/// @brief print a log message. Do not call directly; use LOG() instead
/// @param level log level of message.
/// @param ... variadic parameters for vprintf function (format string with optional parameters)
static void mm_log(int level, ...)
{
  if (level > mm_loglevel) return;

  va_list va;
  va_start(va, level);
  const char *fmt = va_arg(va, const char*);

  if (fmt != NULL) vfprintf(stdout, fmt, va);

  va_end(va);

  fprintf(stdout, "\n");
}




typedef struct {
  char type; 
  int index;       /* index */
  int size;        /* byte size of malloc/realloc/calloc */
} trace_op;

void print_trace(trace_op t){
    printf("%c %d %d\n", t.type, t.index, t.size);
}

trace_op* read_trace(char *dir, char *fname)
{
  FILE *tracefile;
  char path[1024];
  char line[1024];
  int size;
  int num_malloc;

  strncpy(path, dir, 1024);
  strncat(path, fname, 1024);
  tracefile = fopen(path, "r");

  fgets(line, 1024, tracefile);
  sscanf(line, "%d %d", &size, &num_malloc );
  trace_op *op_lists = (trace_op *)malloc(size*sizeof(trace_op));

  for(int i=0; i<size ; i++){
    fgets(line, 1024, tracefile);
    switch(line[0]) {
      case 'm':
        sscanf(line, "%c %d %d", &(op_lists[i].type), &(op_lists[i].index), &(op_lists[i].size) );
        break;
      case 'f':
        sscanf(line, "%c %d ", &(op_lists[i].type), &(op_lists[i].index));
        op_lists[i].size = -1;
        break;
      case 'r':
        sscanf(line, "%c %d %d", &(op_lists[i].type), &(op_lists[i].index), &(op_lists[i].size) );
        break;
      case 'c':
        sscanf(line, "%c %d %d", &(op_lists[i].type), &(op_lists[i].index), &(op_lists[i].size) );
        break;
      default:
        printf("illegal character\n");
        printf("%s\n", line);
        exit(1);
    }
  }
  fclose(tracefile);
  return op_lists;
}




int main(int argc, char *argv[]) {
  FILE *tracefile;
  char path[1024];
  char line[1024];
  int size;
  int num_malloc;

  strncpy(path, argv[2], 1024);
  strncat(path, argv[3], 1024);
  tracefile = fopen(path, "r");
  fgets(line, 1024, tracefile);
  fclose(tracefile);
  sscanf(line, "%d %d", &size, &num_malloc );




  ds_setloglevel(1);
  mm_setloglevel(1);
  srand(time(0));
  ds_allocate(32*1024*1024);


  if(argc != 4) {
    printf("3 arguments needed, current: %d\n", argc);
    assert(0);
  }

  mm_init(atoi(argv[1])); //ap_FirstFit, ap_NextFit, ap_BestFit
  

  trace_op *op_lists = read_trace(argv[2], argv[3]);
  char** malloc_pointers = (char**)(malloc(num_malloc*sizeof(char*)));



  for(int i=0;i<size;i++){
    mm_check();
    switch(op_lists[i].type) {
      case 'm':
        malloc_pointers[op_lists[i].index] = mm_malloc(op_lists[i].size);
        break;
      case 'f':
        mm_free(malloc_pointers[op_lists[i].index]);
        break;
      case 'r':
        malloc_pointers[op_lists[i].index] = mm_realloc(malloc_pointers[op_lists[i].index],op_lists[i].size);
        break;
      case 'c':
        malloc_pointers[op_lists[i].index] = mm_calloc(op_lists[i].size, 1);
        break;
      default:
        printf("illegal op character\n");
        exit(1);
    }
  }

  return 0;
}

