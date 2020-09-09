__kernel void helloworld(__global float* in, __global float* out)
{
	const int num = get_global_id(0);
	out[num] = in[num] / 2;
}