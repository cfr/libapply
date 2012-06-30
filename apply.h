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


/* This union lets us treat floats and doubles in a uniform fashion.
 * This is needed since x86-64 passes both floats and doubles
 * in the same registers (xmm0-xmm7) */
union sse_t {
	float   f;
	double  d;
};

/*  this will handle parameters that are passed on the
 *  stack... */
union value_t {
	/* we'll use longs to hold ints for sanity's sake */
	long l;
	int i;
	float f;
	double d;
	void *p;
};

struct arg_t {
	int type;
	union value_t value;
};

void apply(const void *func, const struct arg_t *args, size_t size);

float applyf(const void *func, const struct arg_t *args, size_t size);

double applyd(const void *func, const struct arg_t *args, size_t size);

long applyl(const void *func, const struct arg_t *args, size_t size);

int applyi(const void *func, const struct arg_t *args, size_t size);

#ifdef __cplusplus
};
#endif

#endif
