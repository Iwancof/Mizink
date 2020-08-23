#include "../kernel_modules.h"
#define NULL 0
#define SIZE_PER_BREAK	128

typedef union header HEADER;
void* memset(void* dst, char dt, unsigned int len);
HEADER* morecore(const int size);
void *malloc(const int size);
void* sbrk(const int size);
void free(void* ap);
int i = 0;

void* memcpy(void* dst, void* src, unsigned int len) {
  // 重複した領域には対応してない
  void* ret = dst;
  for(int i = 0;i < len;i++) {
    *(char*)dst = *(char*)src;
    dst++; src++;
  }
  return ret;
}

void* memset(void* dst, char dt, unsigned int len) {
  rk_draw_str(0, 3, 0x010F, "call memset");
  void* ret = dst;
  for(int i = 0;i < len;i++) {
    *(char*)dst = dt;
    dst++;
  }
  return ret;
}

union header {
  struct {
    union header *next;
    unsigned	size;
  };
  double _d;
};

static HEADER base;
static HEADER *allocp = NULL; // ここから調べたりする
static void* BREAK_VALUE = (void*)HEAP_START;

void *malloc(const int size) {
  HEADER *prev, *cur;
  unsigned int req_s = (size + sizeof(HEADER) - 1) / sizeof(HEADER) + 1;
  //req_s = (req_s <= 2) ? 3 : req_s;
  if((cur = allocp) == NULL) {
    // 初期化
    base.next = allocp = cur = &base;
    base.size = 0;
  }
  // 領域を探す
  for(prev = cur->next;;prev = cur, cur = cur->next) {
    if(cur->size >= req_s) { // 要求を満たしているので、この領域を返す
      if(cur->size == req_s) { // ザイズがちょうど
	prev->next = cur->next;
	// cur を飛ばすようにする
	//ret = cur;
      } else { // 切り出す
	cur->size -= req_s;
	cur += cur->size; // 切り出したところまで移動
	cur->size = req_s;
      }
      allocp = prev;
      return (char*)(cur + 1); // ヘッダ一個分ずらして返す
    }
    if(cur == allocp) { // 返せる領域がない
      cur = morecore(req_s);
      if(cur == NULL) { // 失敗
	kernel_panic();
      }
    }
  }
}

HEADER* morecore(int size) {
  int br_c = SIZE_PER_BREAK * ((size + SIZE_PER_BREAK - 1 ) / SIZE_PER_BREAK);
  HEADER* new = sbrk(br_c * sizeof(HEADER));
  if((int)new == NULL)
    return NULL;

  new->size = br_c;
  free((char*)(new + 1)); // ヘッダ一個分ずらして登録
  return allocp;
}

void free(void* ap) {
  HEADER *target = (HEADER*)ap - 1, *cur;
  for(cur = allocp; !(cur < target && target < cur->next); cur = cur->next)
    if(cur->next <= cur && (cur < target || target < cur->next))
      // 最後にターゲットがある場合
      break;

  // ... | cur | ... |target | ... | cur->next | ...
  // 大体はこのような配置になる

  if(target + target->size == cur->next) {
    // 前に連結可能
    target->size += cur->next->size;
    target->next = cur->next->next;
  } else {
    // 連結できないなら、次はcurの次になる
    target->next = cur->next;
  }
  
  if(cur + cur->size == target) {
    // 後ろに連結可能
    cur->size += target->size;
    cur->next = target->next;
  } else {
    cur->next = target;
  }
  allocp = cur;
}
void* sbrk(int size) { // sizeはバイト単位
  BREAK_VALUE += size;
  return BREAK_VALUE;
}


