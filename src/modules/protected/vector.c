#include "../kernel_modules.h"
#define NULL 0

rk_vec* list_new() {
  return (rk_vec*)NULL;
}

void list_add(rk_vec **list, void* element_ptr, int size) {
  if(*list == (rk_vec*)NULL) {
    // 初期化
    *list = (rk_vec*)malloc(sizeof(rk_vec) + 10);
    (*list)->next = list_new();
    (*list)->element = (void*)malloc(size+ 10);
    memcpy((*list)->element, element_ptr, size);
    return;
  }
  // 最後まで移動する
  rk_vec *cur = *list;
  while(cur->next != NULL) {
    cur = cur->next;
  }
  cur->next = (rk_vec*)malloc(sizeof(rk_vec));
  cur = cur->next;
  // 新しい要素を確保して移動
  cur->next = list_new();
  cur->element = (void*)malloc(size);
  memcpy(cur->element, element_ptr, size);

  return;
}

rk_vec* get_vec_at(rk_vec *list,const int index) {
  rk_vec *cur = list;
  if(cur == NULL) {
    return (rk_vec*)NULL;
  }
  for(int i = 0;i < index;i++) {
    cur = cur->next;
    if(cur == NULL) { // 次がなかった
      return (rk_vec*)NULL;
    }
  }
  return cur;
}
void* get_at(rk_vec *list, const int index) {
  rk_vec *ptr = get_vec_at(list, index);
  if(ptr == NULL) {
    return NULL;
  } else {
    return ptr->element;
  }
}
void* list_erase(rk_vec** list, const int index) {
  if(index == 0) {
    rk_vec* tmp = *list;
    void* ret = tmp->element;
    *list = (*list)->next;
    free(tmp);
    return ret;
  }
  rk_vec *prev = get_vec_at(*list, index - 1);
  if(prev->next == NULL) {
    // 要素がなかった
    return (void*)NULL;
  }
  rk_vec* tmp = prev->next;
  void* ret = tmp->element;
  prev->next = prev->next->next;
  free(tmp);
  return ret;
}

rk_vec* list_copy(rk_vec* src, int size) {
  if(src == NULL) {
    return NULL;
  }
  rk_vec **cur = NULL, **ret = cur;
  while(src != NULL) {
    (*cur) = (rk_vec*)malloc(sizeof(rk_vec));
    (*cur)->element = (void*)malloc(size);
    memcpy((*cur)->element, src->element, size);
    cur = &((*cur)->next);

    src = src->next;
  }
  (*cur) = NULL;
  return *ret;
}
void list_free(rk_vec* src) {
  while(src != NULL) {
    rk_vec *next = src->next;
    free(src->element);
    free(src);
    src = next;
  }
}

void ts_show_all(rk_vec *list) {
  rk_vec *cur = list;
  char buf[] = "--------";
  int i = 0;
  while(cur != NULL) {
    itoa(*(int*)(cur->element), buf);
    rk_draw_str(0, i++, 0x010F, buf);
    cur = cur->next;
  }
}


