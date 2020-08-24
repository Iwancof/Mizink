#include "../kernel_modules.h"

int read_char_and_next(char* dest, char** str);
char contain_format_specification(char c);
int read_list_byinteg(void** list_ptr, char *dest);
int read_list_bychar(void** list_ptr, char *dest);
void write_va_list_value(void** list_ptr, int count);

char* format(char* base, ...) {
  void *list = &base + 1;
  char ret[256], rch;
  int count;

  for(count = 0; ;) {
    if(read_char_and_next(&rch, &base) == 0) {
      break;
    }
    if(rch == '%') {
      char spe_char;
      if(read_char_and_next(&spe_char, &base) == 0) {
	break;
      }
      char spe = contain_format_specification(spe_char);
      if(spe != 0 ) {
	switch(spe) {
	  case 'c': {
	    count += read_list_bychar(&list, &ret[count]);
	    break;
	  }
	  case 'd': {
	    count += read_list_byinteg(&list, &ret[count]);
	    break;
	  }
	  case 'n': {
	    write_va_list_value(&list, count);
	    break;
	  }
	}
        continue;
      }
    }
    ret[count] = rch;
    count++;
  }
  ret[count] = 0;
  char *heap = malloc(sizeof(char) * (count + 1));
  memcpy(heap, ret, count + 1);
  return heap;
}

int read_list_bychar(void** list_ptr, char* dest) {
  // list_ptr: va_list
  // dest: string
  // result: size of string
  char c = *(char*)(*list_ptr);
  *list_ptr += sizeof(char);
  dest[0] = c;
  return 1;
}

char num_chars[] = "0123456789ABCDEF";

int read_list_byinteg(void** list_ptr, char *dest) {
  char ret_tmp[16];
  int value = *(int*)(*list_ptr);
  *list_ptr += sizeof(int);

  int len;
  for(len = 0;value != 0;len++) {
    ret_tmp[len] = num_chars[value % 10];
    value /= 10;
  }

  for(int i = 0;i < len;i++) {
    dest[i] = ret_tmp[len - i - 1];
  }
  return len;
}

void write_va_list_value(void** list_ptr, int count) {
  int *write_ptr = **(unsigned int ***)(list_ptr);
  *list_ptr += sizeof(int*);

  *write_ptr = count;
}


char contain_format_specification(char c) {
  switch(c) {
    case 'c':
    case 'C':
      return 'c';
    case 'd':
    case 'D':
      return 'd';
    case 'x':
    case 'X':
      return 'x';
    case 'f':
    case 'F':
      return 'f';
    case 'n':
    case 'N':
      return 'n';
    default:
      return 0;
  }
}


int read_char_and_next(char* dest, char** str) {
  switch(**str) {
    case 0: {
      return 0; // End of string.
    }
    default: {
      *dest = **str;
      (*str)++;
      return 1; // read char.
    }
  }
}



