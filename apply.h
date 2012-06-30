/*
 *  Copyright (C) 2012 David Aguilar, New-style BSD
 */
#ifndef __APPLY_H
#define __APPLY_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* function return types */
enum type_t {
	T_VOID = 0,
	T_PTR = 1,
	T_INT = 2,
	T_LONG = 3,
	T_FLOAT = 4,
	T_DOUBLE = 5
};

/*  this will handle parameters that are passed on the
 *  stack... */
typedef union u_value {
	/* we'll use longs to hold ints for sanity's sake */
	long l;
	int i;
	float f;
	double d;
	void *p;
} value_t;

typedef struct s_arg {
	int type;
	value_t value;
} arg_t;

void apply(const void *func, const arg_t *args, size_t size);

float applyf(const void *func, const arg_t *args, size_t size);

double applyd(const void *func, const arg_t *args, size_t size);

long applyl(const void *func, const arg_t *args, size_t size);

int applyi(const void *func, const arg_t *args, size_t size);

#ifdef __cplusplus
};
#endif

#endif
