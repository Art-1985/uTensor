#ifndef UTENSOR_CTX_H
#define UTENSOR_CTX_H

#include <stdio.h>

#include <functional>
#include <initializer_list>
#include <memory>
#include <unordered_map>

#include "src/uTensor/core/uTensorBase.hpp"
//#include <list>

class Ref_Record {
 public:
  uint8_t count;
  bool allow_incr;
  bool is_static;
  bool is_cacheable;
  S_TENSOR ptr;

  Ref_Record() {
    count = 0;
    is_static = false;
    is_cacheable = true;
    allow_incr = true;
    ptr = NULL;
  }
};

class Context : public uTensor {
 protected:
  std::vector<Operator*> op_list;
  bool del_onsight;

  std::unordered_map<TName, Ref_Record> rTable;
  std::unordered_map<TName, Operator*>
      opTable;  // all tensors alive  //kill all unused if malloc failed?
  // uint32_t m_size; //remaining memory size
  // void registerTensor(Tensor* t);
  // void gc(void); //garbage collector, delete any tracked unreferenced tensor

  void initTensors(const S_TList& t_list);
  void deinitTensors(const S_TList& t_list);

  void incrTNameListRef(const TNameList& t_list);
  void dcrListRef(S_TList t_list);
  void delTensor(TName t);
  // uint16_t incrRef(std::shared_ptr<Tensor> sptr);
  uint8_t dcrRef(TName name);
  bool isTracked(TName name);
  void cleanUpOp(Operator* op);
  Operator* registerOpTable(std::function<void*(void)> func, TName _name);
  // bool isTracked(Tensor* t);
  // uint16_t getRef();

 public:
  S_TENSOR add(Tensor* t, TName _name, uint8_t init_count = 0);
  S_TENSOR get(TName const& t_name);
  void push(Operator* op, TName _name, TNameList _inputs,
            TNameList _outputs);  // a place holder api to name an op
  void push(Operator* op, TNameList& _inputs, TNameList& _outputs);
  void push(Operator* op, std::initializer_list<TName> _inputs,
            std::initializer_list<TName> _outputs);
  uint32_t gc(void);
  int eval(void);

  // TODO: add a keep(int count = 1) funcion to make graph construction easier?

  Context() { del_onsight = true; }
};

#define hold(...) ([&]() { return (void*)(__VA_ARGS__); })

#endif  // UTENSOR_CTX_H
