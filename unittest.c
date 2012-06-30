#include <stdio.h>
#include <apply.h>


int sumi(int a, int b, int c)
{
	return a + b + c;
}


float sum5f(float a, float b, float c, float d, float e)
{
	return a + b + c + d + e;
}


float verify10f(
		float f1, float f2, float f3, float f4, float f5,
		float f6, float f7, float f8, float f9, float fa)
{
	if (f1 != fa || f2 != f9 || f3 != f8 || f4 != f7 || f5 != f6 )
		return 0.f;
	else
		return 42.f;
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
	struct arg_t args[] = {
		{ T_INT, (union value_t)1 },
		{ T_INT, (union value_t)10 },
		{ T_INT, (union value_t)100 }
	};

	ret = applyi(sumi, args, 3);
	if (111 == ret)
		printf("ok 1 test_sum_of_ints()\n");
	else {
		printf("error got %d instead of 111\n", ret);
		err = 1;
	}

	return err;
}


int test_sum_of_floats(void)
{
	float ret;
	int err = 0;
	struct arg_t args[] = {
		{ T_FLOAT, (union value_t)1.0f },
		{ T_FLOAT, (union value_t)10.f },
		{ T_FLOAT, (union value_t)100.f },
		{ T_FLOAT, (union value_t)1000.f },
		{ T_FLOAT, (union value_t)10000.f }
	};

	ret = applyf(sum5f, args, 5);
	if (11111.f == ret)
		printf("ok 2 test_sum_of_floats()\n");
	else {
		printf("error got %f instead of 0.11111\n", ret);
		err = 1;
	}

	return err;
}


int test_verify_many_floats(void)
{
	float ret;
	int err = 0;
	struct arg_t args[] = {
		{ T_FLOAT, (union value_t)1.0f },
		{ T_FLOAT, (union value_t)10.f },
		{ T_FLOAT, (union value_t)100.f },
		{ T_FLOAT, (union value_t)1000.f },
		{ T_FLOAT, (union value_t)10000.f },
		{ T_FLOAT, (union value_t)10000.f },
		{ T_FLOAT, (union value_t)1000.f },
		{ T_FLOAT, (union value_t)100.f },
		{ T_FLOAT, (union value_t)10.f },
		{ T_FLOAT, (union value_t)1.0f }
	};

	ret = applyf(verify10f, args, 10);
	if (42.f == ret)
		printf("ok 3 test_verify_many_floats()\n");
	else {
		printf("error got %f instead of 0.11111\n", ret);
		err = 1;
	}

	return err;
}


int test_mixed_on_the_stack(void)
{
	int ret;
	int err = 0;
	struct arg_t args[] = {
		{ T_INT,    (union value_t)1 },
		{ T_FLOAT,  (union value_t)1.f },
		{ T_LONG,   (union value_t)2L },
		{ T_DOUBLE, (union value_t)2. },
		{ T_INT,    (union value_t)3 },
		{ T_FLOAT,  (union value_t)3.f },
		{ T_LONG,   (union value_t)4L },
		{ T_DOUBLE, (union value_t)4. },
		{ T_INT,    (union value_t)5 },
		{ T_FLOAT,  (union value_t)5.f },
		{ T_LONG,   (union value_t)6L },
		{ T_DOUBLE, (union value_t)6. },
		{ T_INT,    (union value_t)7 },
		{ T_FLOAT,  (union value_t)7.f },
		{ T_LONG,   (union value_t)8L },
		{ T_DOUBLE, (union value_t)8. },
		{ T_INT,    (union value_t)9 },
		{ T_FLOAT,  (union value_t)9.f },
		{ T_LONG,   (union value_t)10L },
		{ T_DOUBLE, (union value_t)10. },
		{ T_INT,    (union value_t)11 },
		{ T_FLOAT,  (union value_t)11.f },
		{ T_LONG,   (union value_t)12L },
		{ T_DOUBLE, (union value_t)12. },
		{ T_INT,    (union value_t)13 },
		{ T_FLOAT,  (union value_t)13.f },
		{ T_LONG,   (union value_t)14L },
		{ T_DOUBLE, (union value_t)14. },
		{ T_INT,    (union value_t)15 },
		{ T_FLOAT,  (union value_t)15.f },
		{ T_LONG,   (union value_t)16L },
		{ T_DOUBLE, (union value_t)16. }
	};

	ret = applyi(sum_mixed_on_the_stack, args, 32);
	if (16 == ret)
		printf("ok 4 test_mixed_on_the_stack()\n");
	else {
		printf("error got %d instead of 16\n", ret);
		err = 1;
	}

	return err;
}


int main(int argc, char **argv)
{
	int err = 0;

	err += test_sum_of_ints();
	err += test_sum_of_floats();
	err += test_verify_many_floats();
	err += test_mixed_on_the_stack();

	return err;
}
