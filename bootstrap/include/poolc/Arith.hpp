#ifndef POOLC_ARITH_HPP_LOCK
#define POOLC_ARITH_HPP_LOCK

enum sign_t { sign_plus, sign_minus };
enum arith_op { op_add, op_sub, op_mul, op_div, op_mod };
enum arith_unary { unary_inc, unary_dec, unary_post_inc, unary_post_dec };
enum logical_op { op_and, op_or, op_eq, op_neq, op_lt, op_le, op_gt, op_ge };
enum logical_unary { unary_not };

#endif //POOLC_ARITH_HPP_LOCK
