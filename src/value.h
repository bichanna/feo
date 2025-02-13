#ifndef FVALUE_H
#define FVALUE_H

#include <float.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LIST_ELEM_INIT_CAP 2
#define LIST_GROW_FACTOR 2

struct FObject;

typedef enum ValueType {
  VAL_EMPTY,
  VAL_INTEGER,
  VAL_FLOAT,
  VAL_BOOL,
  VAL_OBJECT,
} ValueType;

typedef struct FValue {
  ValueType val_type;
  union {
    int64_t i;
    double f;
    bool b;
    struct FObject *obj;
  } val;
} FValue;

typedef struct FString {
  char *str;
} FString;

typedef struct FAtom {
  const char *str;
} FAtom;

typedef struct FList {
  FValue *arr;
  size_t len;
  size_t cap;
} FList;

typedef struct FFunc {
  const char *name;
  const uint8_t *inst;
  uint16_t arity;
} FFunc;

typedef struct FUpval {
  FValue value;
} FUpval;

typedef struct FClosure {
  uint8_t upval_count;
  FUpval **upvalues;
  FFunc *func;
} FClosure;

typedef enum ObjectType {
  OBJ_STRING,
  OBJ_ATOM,
  OBJ_LIST,
  OBJ_FUNC,
  OBJ_UPVAL,
  OBJ_CLOS,
} ObjectType;

typedef struct FObject {
  bool marked;
  ObjectType obj_type;
  union {
    FString fstr;
    FAtom fatom;
    FList flist;
    FFunc ffunc;
    FUpval fupval;
    FClosure fclos;
  } obj;
  struct FObject *next;
  void (*free_inner)(struct FObject *);
} FObject;

FValue create_empty_value(void);
FValue create_integer_value(int64_t i);
FValue create_float_value(double f);
FValue create_bool_value(bool b);
FValue create_object_value(FObject *obj);

FObject *alloc_string_object(char *str, FObject *prev);
void string_object_free(FObject *str_obj);
size_t string_object_utf8_len(FString *fstr);
int string_object_concat(FString *dest, FString *src);

FObject *alloc_atom_object(const char *str, FObject *prev);
void atom_object_free(FObject *atom_obj);
size_t atom_object_utf8_len(FAtom *fatom);

FObject *alloc_list_object_with_cap(size_t cap, FObject *prev);
FObject *alloc_list_object(FObject *prev);
void list_object_free(FObject *list_obj);
void list_object_grow_cap(FList *flist, int by);
void list_object_append(FList *flist, FValue elem);
int list_object_remove(FList *flist, size_t idx);
void list_object_pop(FList *flist);

FObject *alloc_func_object(uint16_t arity,
                           const char *name,
                           const uint8_t *inst,
                           FObject *prev);
void func_object_free(FObject *func_obj);

FObject *alloc_upval_object(FValue value, FObject *prev);
void upval_object_free(FObject *upval_obj);

FObject *alloc_clos_object(FUpval **upvalues,
                           uint8_t upval_count,
                           FFunc *func,
                           FObject *prev);
void clos_object_free(FObject *clos_obj);

#endif  // !FVALUE_H
