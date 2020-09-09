int IX(int x, int y, int z, int N)
{
	return ((x) + (y) * N + (z) * N * N);
}

void mul(__global float* in, __global float* out, int N) {
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);
	for (int iter = 0; iter < 4; iter++) {
		out[IX(i, j, k, N)] = (in[IX(i, j, k, N)] + 1 > 15 ? 1 : 2);
	}
}

__kernel void helloworld(__global float* in, __global float* out, __global int* N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);
	out[IX(i, j, k, N[0])] = in[IX(i, j, k, N[0])];
}

__kernel void helloworld2(__global float* in, __global float* out, __global int* N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);

	out[IX(i, j, k, N[0])] = in[IX(i, j, k, N[0])];

	mul(out, out, N[0]);
}


