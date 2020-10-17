int IX(int x, int y, int z, int N)
{
	return ((x) + (y) * N + (z) * N * N);
}

void set_bnd(int b, __global float* x, int N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);

	if (j > 0 && j < N - 1 && i > 0 && i < N - 1) {
		// x[IX(i, j, 0    , N)] = b == 3 ? -x[IX(i, j, 1    , N)] : x[IX(i, j, 1    , N)];
		// x[IX(i, j, N - 1, N)] = b == 3 ? -x[IX(i, j, N - 2, N)] : x[IX(i, j, N - 2, N)];
	}

	if (k > 0 && k < N - 1 && i > 0 && i < N - 1) {
		x[IX(i, 0    , k, N)] = b == 2 ? -x[IX(i, 1    , k, N)] : x[IX(i, 1    , k, N)];
		//x[IX(i, N - 1, k, N)] = b == 2 ? -x[IX(i, N - 2, k, N)] : x[IX(i, N - 2, k, N)];
	}


	if (k > 0 && k < N - 1 && j > 0 && j < N - 1) {
		// x[IX(0    , j, k, N)] = b == 1 ? -x[IX(1    , j, k, N)] : x[IX(1    , j, k, N)];
		// x[IX(N - 1, j, k, N)] = b == 1 ? -x[IX(N - 2, j, k, N)] : x[IX(N - 2, j, k, N)];
	}

	// if (i == 0 && j == 0 && k == 0) {
	// 	x[IX(0    , 0    , 0    , N)] = 0.33f * (x[IX(1    , 0    , 0    , N)] + x[IX(0    , 1    , 0    , N)] + x[IX(0    , 0    , 1    , N)]);
	// }

	// if (i == 0 && j == N - 1 && k == 0) {
	// 	x[IX(0    , N - 1, 0    , N)] = 0.33f * (x[IX(1    , N - 1, 0    , N)] + x[IX(0    , N - 2, 0    , N)] + x[IX(0    , N - 1, 1    , N)]);
	// }

	// if (i == 0 && j == 0 && k == N - 1) {
	// 	x[IX(0    , 0    , N - 1, N)] = 0.33f * (x[IX(1    , 0    , N - 1, N)] + x[IX(0    , 1    , N - 1, N)] + x[IX(0    , 0    , N - 2, N)]);
	// }

	// if (i == 0 && j == N - 1 && k == N - 1) {
	// 	x[IX(0    , N - 1, N - 1, N)] = 0.33f * (x[IX(1    , N - 1, N - 1, N)] + x[IX(0    , N - 2, N - 1, N)] + x[IX(0    , N - 1, N - 2, N)]);
	// }

	// if (i == N - 1 && j == 0 && k == 0) {
	// 	x[IX(N - 1, 0    , 0    , N)] = 0.33f * (x[IX(N - 2, 0    , 0    , N)] + x[IX(N - 1, 1    , 0    , N)] + x[IX(N - 1, 0    , 1    , N)]);
	// }

	// if (i == N - 1 && j == N - 1 && k == 0) {
	// 	x[IX(N - 1, N - 1, 0    , N)] = 0.33f * (x[IX(N - 2, N - 1, 0    , N)] + x[IX(N - 1, N - 2, 0    , N)] + x[IX(N - 1, N - 1, 1    , N)]);
	// }

	// if (i == N - 1 && j == 0 && k == N - 1) {
	// 	x[IX(N - 1, 0    , N - 1, N)] = 0.33f * (x[IX(N - 2, 0    , N - 1, N)] + x[IX(N - 1, 1    , N - 1, N)] + x[IX(N - 1, 0    , N - 2, N)]);
	// }

	// if (i == N - 1 && j == N - 1 && k == N - 1) {
	// 	x[IX(N - 1, N - 1, N - 1, N)] = 0.33f * (x[IX(N - 2, N - 1, N - 1, N)] + x[IX(N - 1, N - 2, N - 1, N)] + x[IX(N - 1, N - 1, N - 2, N)]);
	// }
}

void lin_solve(int b, __global float* x, __global float* x0, float a, float c, int iter, int N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);

	float cRecip = 1.0 / c;
	for (int m = 0; m < iter; m++) {
		if (k > 0 && k < N - 1 && j > 0 && j < N - 1 && i > 0 && i < N - 1) {
			x[IX(i, j, k, N)] = (x0[IX(i, j, k, N)] + a * (
			                         x[IX(i + 1, j    , k    , N)]
			                         + x[IX(i - 1, j    , k    , N)]
			                         + x[IX(i    , j + 1, k    , N)]
			                         + x[IX(i    , j - 1, k    , N)]
			                         + x[IX(i    , j    , k + 1, N)]
			                         + x[IX(i    , j    , k - 1, N)]
			                     )) * cRecip;
		}
		set_bnd(b, x, N);
	}
}

__kernel void addDensity(const int x, const int y, const int z, const float amount, __global float* density, const int N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);
	if (x == i && y == j && z == k) {
		density[IX(x, y, z, N)] += amount;
	}
}

__kernel void addVelocity(const int x, const int y, const int z, const float amountX, const float amountY, const float amountZ, __global float* Vx, __global float* Vy, __global float* Vz, const int N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);

	if (x == i && y == j && z == k) {
		int index = IX(x, y, z, N);
		Vx[index] += amountX;
		Vy[index] += amountY;
		Vz[index] += amountZ;
	}
}

__kernel void diffuse(const int b, __global float* x, __global float* x0, const float diff, const float dt, const int iter, const int N)
{
	float a = dt * diff * (N - 2) * (N - 2);
	lin_solve(b, x, x0, a, 1 + 6 * a, iter, N);
}

__kernel void project(__global float* velocX, __global float* velocY, __global float* velocZ, __global float* p, __global float* div, const int iter, const int N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);

	if (k > 0 && k < N - 1 && j > 0 && j < N - 1 && i > 0 && i < N - 1) {
		div[IX(i, j, k, N)] = -0.5f * (
		                          velocX[IX(i + 1, j    , k    , N)]
		                          - velocX[IX(i - 1, j    , k    , N)]
		                          + velocY[IX(i    , j + 1, k    , N)]
		                          - velocY[IX(i    , j - 1, k    , N)]
		                          + velocZ[IX(i    , j    , k + 1, N)]
		                          - velocZ[IX(i    , j    , k - 1, N)]
		                      ) / N;
		p[IX(i, j, k, N)] = 0;
	}

	set_bnd(0, div, N);
	set_bnd(0, p, N);
	lin_solve(0, p, div, 1, 6, iter, N);

	if (k > 0 && k < N - 1 && j > 0 && j < N - 1 && i > 0 && i < N - 1) {
		velocX[IX(i, j, k, N)] -= 0.5f * (  p[IX(i + 1, j    , k    , N)]
		                                    - p[IX(i - 1, j    , k    , N)]) * N;
		velocY[IX(i, j, k, N)] -= 0.5f * (  p[IX(i    , j + 1, k    , N)]
		                                    - p[IX(i    , j - 1, k    , N)]) * N;
		velocZ[IX(i, j, k, N)] -= 0.5f * (  p[IX(i    , j    , k + 1, N)]
		                                    - p[IX(i    , j    , k - 1, N)]) * N;
	}

	set_bnd(1, velocX, N);
	set_bnd(2, velocY, N);
	set_bnd(3, velocZ, N);
}

__kernel void advect(const int b, __global float* d, __global float* d0,  __global float* velocX, __global float* velocY, __global float* velocZ, const float dt, const int N)
{
	const int i = get_global_id(0);
	const int j = get_global_id(1);
	const int k = get_global_id(2);

	float i0, i1, j0, j1, k0, k1;

	float dtx = dt * (N - 2);
	float dty = dt * (N - 2);
	float dtz = dt * (N - 2);

	float s0, s1, t0, t1, u0, u1;
	float tmp1, tmp2, tmp3, x, y, z;

	float Nfloat = N - 2;
	float ifloat = i;
	float jfloat = j;
	float kfloat = k;

	if (k > 0 && k < N - 1 && j > 0 && j < N - 1 && i > 0 && i < N - 1) {
		tmp1 = dtx * velocX[IX(i, j, k, N)];
		tmp2 = dty * velocY[IX(i, j, k, N)];
		tmp3 = dtz * velocZ[IX(i, j, k, N)];
		x    = ifloat - tmp1;
		y    = jfloat - tmp2;
		z    = kfloat - tmp3;

		if (x < 0.5f) x = 0.5f;
		if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
		i0 = (int)(x);
		i1 = i0 + 1.0f;

		if (y < 0.5f) y = 0.5f;
		if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
		j0 = (int)(y);
		j1 = j0 + 1.0f;

		if (z < 0.5f) z = 0.5f;
		if (z > Nfloat + 0.5f) z = Nfloat + 0.5f;
		k0 = (int)(z);
		k1 = k0 + 1.0f;

		s1 = x - i0;
		s0 = 1.0f - s1;

		t1 = y - j0;
		t0 = 1.0f - t1;

		u1 = z - k0;
		u0 = 1.0f - u1;

		int i0i = i0;
		int i1i = i1;

		int j0i = j0;
		int j1i = j1;

		int k0i = k0;
		int k1i = k1;

		d[IX(i, j, k, N)] =

		    s0 * ( t0 * (u0 * d0[IX(i0i, j0i, k0i, N)]
		                 + u1 * d0[IX(i0i, j0i, k1i, N)])
		           + ( t1 * (u0 * d0[IX(i0i, j1i, k0i, N)]
		                     + u1 * d0[IX(i0i, j1i, k1i, N)])))
		    + s1 * ( t0 * (u0 * d0[IX(i1i, j0i, k0i, N)]
		                   + u1 * d0[IX(i1i, j0i, k1i, N)])
		             + ( t1 * (u0 * d0[IX(i1i, j1i, k0i, N)]
		                       + u1 * d0[IX(i1i, j1i, k1i, N)])));

	}
	set_bnd(b, d, N);
}