/*
 *  Copyright (C) 2012 David Aguilar, New-style BSD
 */
#include <assert.h>
#include "apply.h"

#ifdef __x86_64__

#define x64 __asm__ __volatile__
#define xmin(a, b) ((a < b) ? a : b)

/*
 *  INT args are passed in:
 *    rdi(edi),   rsi(esi),   rdx(edx),
 *    rcx(ecx),   r8,         r9
 *  rax is the return register.
 *
 *  SSE (float/double) args are passed in:
 *    xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
 *  xmm0 is the return value register.
 */
#define GPR_REGISTERS 6
#define SSE_REGISTERS 8

/* This function actually does the C call.
* Steps:
* - Determine the number of stack args
* - Preload the values into a local stack
* - Load appropriate values into registers and
*   onto the machine's real stack
* - Call the function
* - Copy the return value from the stack onto
*   the passed pointer
*/
static void ccall(
		void *result,
		const type_t retval, /* Return type */
		const void *func, /* Function to call */
		const arg_t *args, /* Arguments */
		const size_t size)
{
	/* return values */
	int int_result;
	double double_result;
	float float_result;
	long long_result;

	/* temporary vars */
	size_t i, idx;
	int gpr_count = 0;
	int sse_count = 0;
	int stack_count = 0;
	int sse_register_count = 0;

	/* these args get passed directly in the registers */
	long gpr_args[GPR_REGISTERS];
	value_t sse_args[SSE_REGISTERS];

	/* these args get passed on the stack */
	value_t stack_args[size - GPR_REGISTERS];

	for (i = 0; i < size; i++) {
		switch (args[i].type) {
		case T_INT:
		case T_LONG:
		case T_PTR:
			if (gpr_count < GPR_REGISTERS)
				gpr_args[gpr_count++] = args[i].value.l;
			else
				/* arrange for this to go onto the stack */
				stack_args[stack_count++].l = args[i].value.l;
			break;

		/*  pass a float or a double:
		 *  on x86-64 these are both passed in
		 *  SSE registers, hence the similar treatment.
		 *  The "magic" is in the union used -- this
		 *  reconciles the types at the C level so that
		 *  the assembly can just glue it together */
		case T_FLOAT:
			if (sse_count < SSE_REGISTERS)
				/* set the float(f) field for the sse_arg */
				sse_args[sse_count++].f = args[i].value.f;
			else
				/* pass this on the stack */
				stack_args[stack_count++].f = args[i].value.f;
			break;
		/*  double -- the magic is in the .d vs .f */
		case T_DOUBLE:
			if (sse_count < SSE_REGISTERS)
				/* set the double(d) field */
				sse_args[sse_count++].d = args[i].value.d;
			else
				/* pass this on the stack */
				stack_args[stack_count++].d = args[i].value.d;
			break;
		default:
			assert(0);
			return;
		}
	}

	sse_register_count = xmin(sse_count, SSE_REGISTERS);

	/* push any stack args onto the stack, in right-to-left order */
	if (stack_count % 2) {
		/* keep the stack 16-byte aligned */
		x64("subq $8, %%rsp"
			: /* no output */
			: /* no input */
			: "%rsp" /* %rsp is clobbered */);
	}
	/* process stack args in reverse order (see the amd64 ABI documentation) */
	for (i = 0; i < stack_count; i++) {
		idx = stack_count - i - 1;
		x64("pushq %0" : : "m"(stack_args[idx].l));
	}

	/* mov long arguments into appropriate registers */
	/* : CPP(this) ptr is passed in %rdi */
	x64(
		"movq %0, %%rdi\n\t"
		"movq %1, %%rsi\n\t"
		"movq %2, %%rdx\n\t"
		"movq %3, %%rcx\n\t"
		"movq %4, %%r8\n\t"
		"movq %5, %%r9"
		: /* no memory output */
		: /* memory inputs */
		"m"(gpr_args[0]), "m"(gpr_args[1]), "m"(gpr_args[2]),
		"m"(gpr_args[3]), "m"(gpr_args[4]), "m"(gpr_args[5])
		: /* clobbered registers */
		"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9");

	/* mov SSE arguments into the appropriate registers */
	x64(
		"movq %0, %%xmm0\n\t"
		"movq %1, %%xmm1\n\t"
		"movq %2, %%xmm2\n\t"
		"movq %3, %%xmm3\n\t"
		"movq %4, %%xmm4\n\t"
		"movq %5, %%xmm5\n\t"
		"movq %6, %%xmm6\n\t"
		"movq %7, %%xmm7\n\t"
		: /* no memory output */
		: /* memory inputs */
		"m"(sse_args[0]), "m"(sse_args[1]),
		"m"(sse_args[2]), "m"(sse_args[3]),
		"m"(sse_args[4]), "m"(sse_args[5]),
		"m"(sse_args[6]), "m"(sse_args[7])
		: /* clobbered registers */
		"%xmm0", "%xmm1", "%xmm2", "%xmm3",
		"%xmm4", "%xmm5", "%xmm6", "%xmm7");

	x64(/* call the actual function */
		"mov %0, %%eax\n\t"
		"callq *%1"
		: /* no output */
		: /* number of sse registers used in %al */
		"m"(sse_register_count),
		/* da func */
		"m"(func));

	x64(/* get back the return values */
		"movl %%eax, %0\n\t"
		"movq %%rax, %1\n\t"
		"movq %%xmm0, %2\n\t"
		"movd %%xmm0, %3"
		: /* copy retVal to result variables */
		"=m"(int_result),
		"=m"(long_result),
		"=m"(double_result),
		"=m"(float_result));

	switch(retval) {
	case T_PTR:
	case T_LONG:
	case T_INT:
		*(long *)result = long_result;
		break;
	case T_FLOAT:
		*(float *)result = float_result;
		break;
	case T_DOUBLE:
		*(double *)result = double_result;
		break;
	default:
		assert(0);
		break;
	}
}

#endif


void *apply(const void *func, const arg_t *args, size_t size)
{
	void *result;
	ccall(&result, T_PTR, func, args, size);
	return result;
}

float applyf(const void *func, const arg_t *args, size_t size)
{
	float result;
	ccall(&result, T_FLOAT, func, args, size);
	return result;
}

double applyd(const void *func, const arg_t *args, size_t size)
{
	double result;
	ccall(&result, T_DOUBLE, func, args, size);
	return result;
}


long applyl(const void *func, const arg_t *args, size_t size)
{
	long result;
	ccall(&result, T_LONG, func, args, size);
	return result;
}


int applyi(const void *func, const arg_t *args, size_t size)
{
	long result;
	ccall(&result, T_INT, func, args, size);
	return (int)result;
}
