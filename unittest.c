#include <stdio.h>
#include <apply.h>
#include <stdarg.h>


#define len(x) (sizeof(x)/sizeof(x[0]))


int sumi3(int a, int b, int c)
{
	return a + b + c;
}


int sumv(int a, int last, ...)
{
	int sum = a;
	va_list arg;

	va_start(arg, last);
	while (!last) {
		sum += va_arg(arg, int);
		last = va_arg(arg, int);
	}
	va_end(arg);

	return sum;
}


float sum5f(float a, float b, float c, float d, float e)
{
	return a + b + c + d + e;
}


double sum5d(double a, double b, double c, double d, double e)
{
	return a + b + c + d + e;
}


float verify10f(
		float ret,
		float f1, float f2, float f3, float f4, float f5,
		float f6, float f7, float f8, float f9, float fa)
{
	if (f1 != fa || f2 != f9 || f3 != f8 || f4 != f7 || f5 != f6)
		return -ret;
	else
		return ret;
}


int sum_mixed_on_the_stack(
		int g1, float s1, long g2, double s2,
		int g3, float s3, long g4, double s4,
		int g5, float s5, long g6, double s6,
		int g7, float s7, long g8, double s8,
		int g9, float s9, long ga, double sa,
		int gb, float sb, long gc, double sc,
		int gd, float sd, long ge, double se,
		int gf, float sf, long g0, double s0)
{
	return (
		(g1 == (int)s1) + (g2 == (long)s2) +
		(g3 == (int)s3) + (g4 == (long)s4) +
		(g5 == (int)s5) + (g6 == (long)s6) +
		(g7 == (int)s7) + (g8 == (long)s8) +
		(g9 == (int)s9) + (ga == (long)sa) +
		(gb == (int)sb) + (gc == (long)sc) +
		(gc == (int)sc) + (ge == (long)se) +
		(gf == (int)sf) + (g0 == (long)s0));
}


int test_sum_of_ints(void)
{
	int ret;
	int err = 0;
	arg_t args[] = {
		{ T_INT, (value_t)1 },
		{ T_INT, (value_t)10 },
		{ T_INT, (value_t)100 }
	};

	ret = applyi(sumi3, args, len(args));
	if (111 == ret)
		printf("ok\ttest_sum_of_ints()\n");
	else {
		printf("fail\tgot %d instead of 111\n", ret);
		err = 1;
	}

	return err;
}


int test_sum_of_floats(void)
{
	float ret;
	int err = 0;
	arg_t args[] = {
		{ T_FLOAT, (value_t)1.0f },
		{ T_FLOAT, (value_t)10.f },
		{ T_FLOAT, (value_t)100.f },
		{ T_FLOAT, (value_t)1000.f },
		{ T_FLOAT, (value_t)10000.f }
	};

	ret = applyf(sum5f, args, len(args));
	if (11111.f == ret)
		printf("ok\ttest_sum_of_floats()\n");
	else {
		printf("fail\tgot %f instead of 0.11111\n", ret);
		err = 1;
	}

	return err;
}


int test_sum_of_doubles(void)
{
	double ret;
	int err = 0;
	arg_t args[] = {
		{ T_DOUBLE, (value_t)1.0 },
		{ T_DOUBLE, (value_t)10. },
		{ T_DOUBLE, (value_t)100. },
		{ T_DOUBLE, (value_t)1000. },
		{ T_DOUBLE, (value_t)10000. }
	};
	ret = applyd(sum5d, args, len(args));
	if (11111. == ret)
		printf("ok\ttest_sum_of_doubles()\n");
	else {
		printf("fail\tgot %g instead of 0.11111\n", ret);
		err = 1;
	}

	return err;
}


int test_verify_many_floats(void)
{
	float ret;
	int err = 0;
	arg_t args[] = {
		{ T_FLOAT, (value_t)42.0f },
		{ T_FLOAT, (value_t)1.0f },
		{ T_FLOAT, (value_t)10.f },
		{ T_FLOAT, (value_t)100.f },
		{ T_FLOAT, (value_t)1000.f },
		{ T_FLOAT, (value_t)10000.f },
		{ T_FLOAT, (value_t)10000.f },
		{ T_FLOAT, (value_t)1000.f },
		{ T_FLOAT, (value_t)100.f },
		{ T_FLOAT, (value_t)10.f },
		{ T_FLOAT, (value_t)1.0f }
	};

	ret = applyf(verify10f, args, len(args));
	if (42.f == ret)
		printf("ok\ttest_verify_many_floats()\n");
	else {
		printf("fail\tgot %f instead of 42.f\n", ret);
		err = 1;
	}

	return err;
}


int test_mixed_on_the_stack(void)
{
	int ret;
	int err = 0;
	arg_t args[] = {
		{ T_INT,    (value_t)1 },
		{ T_FLOAT,  (value_t)1.f },
		{ T_LONG,   (value_t)2L },
		{ T_DOUBLE, (value_t)2. },
		{ T_INT,    (value_t)3 },
		{ T_FLOAT,  (value_t)3.f },
		{ T_LONG,   (value_t)4L },
		{ T_DOUBLE, (value_t)4. },
		{ T_INT,    (value_t)5 },
		{ T_FLOAT,  (value_t)5.f },
		{ T_LONG,   (value_t)6L },
		{ T_DOUBLE, (value_t)6. },
		{ T_INT,    (value_t)7 },
		{ T_FLOAT,  (value_t)7.f },
		{ T_LONG,   (value_t)8L },
		{ T_DOUBLE, (value_t)8. },
		{ T_INT,    (value_t)9 },
		{ T_FLOAT,  (value_t)9.f },
		{ T_LONG,   (value_t)10L },
		{ T_DOUBLE, (value_t)10. },
		{ T_INT,    (value_t)11 },
		{ T_FLOAT,  (value_t)11.f },
		{ T_LONG,   (value_t)12L },
		{ T_DOUBLE, (value_t)12. },
		{ T_INT,    (value_t)13 },
		{ T_FLOAT,  (value_t)13.f },
		{ T_LONG,   (value_t)14L },
		{ T_DOUBLE, (value_t)14. },
		{ T_INT,    (value_t)15 },
		{ T_FLOAT,  (value_t)15.f },
		{ T_LONG,   (value_t)16L },
		{ T_DOUBLE, (value_t)16. }
	};

	ret = applyi(sum_mixed_on_the_stack, args, len(args));
	if (len(args)/2 == ret)
		printf("ok\ttest_mixed_on_the_stack()\n");
	else {
		printf("fail\tgot %d instead of 16\n", ret);
		err = 1;
	}

	return err;
}


int test_sum_varargs(void)
{
	int ret;
	int err = 0;

	arg_t args[] = {
		{ T_INT, (value_t)1 },
		{ T_INT, (value_t)0 },
		{ T_INT, (value_t)10 },
		{ T_INT, (value_t)0 },
		{ T_INT, (value_t)100 },
		{ T_INT, (value_t)0 },
		{ T_INT, (value_t)1000 },
		{ T_INT, (value_t)0 },
		{ T_INT, (value_t)10000 },
		{ T_INT, (value_t)1 }
	};
	ret = applyi(sumv, args, len(args));
	if (11111 == ret)
		printf("ok\ttest_sum_varargs\n");
	else {
		printf("fail\ttest_sum_varargs got %d instead of 111\n", ret);
		err = 1;
	}

	return err;
}


int main(int argc, char **argv)
{
	int err = 0;

	err += test_sum_of_ints();
	err += test_sum_of_floats();
	err += test_sum_of_doubles();
	err += test_verify_many_floats();
	err += test_mixed_on_the_stack();
	err += test_sum_varargs();

	return err;
}
