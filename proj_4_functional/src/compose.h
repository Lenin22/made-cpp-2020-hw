#pragma once
#include <type_traits>
#include <stdexcept>

template <class Tp, class... Us>
struct type_taker {
  typedef typename Tp::result_type type;
};

template <class Head>
auto compose(Head func) {
  return func;
}

template <class Head, class... Types>
auto compose(Head func, Types... funcs) {
  if (!std::is_same<typename Head::argument_type, typename type_taker<Types...>::type>::value) {
    throw std::invalid_argument("func input should be other type\n");
  } else {
    return [=](auto x) { return func(compose(funcs...)(x)); };
  }
}
