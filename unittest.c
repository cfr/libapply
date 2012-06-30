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

int main(int argc, char **argv)
{
	int err = 0;

	err += test_sum_of_ints();
	err += test_sum_of_floats();
	err += test_verify_many_floats();

	return err;
}
