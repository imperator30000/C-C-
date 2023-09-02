#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double delta = 1.e-8f;
typedef struct
{
	size_t rows;
	size_t cols;
	double **data;
} Matrix;

void destroy_matrix(Matrix *matrix)
{
	for (size_t i = 0; i < matrix->rows; i++)
	{
		free(matrix->data[i]);
	}
	free(matrix->data);
	free(matrix);
}

Matrix *create_matrix(size_t rows, size_t cols)
{
	Matrix *matrix = (Matrix *)malloc(sizeof(Matrix));
	if (matrix == NULL)
	{
		return NULL;
	}
	matrix->rows = rows;
	matrix->cols = cols;
	matrix->data = (double **)malloc(rows * sizeof(double *));
	if (matrix->data == NULL)
	{
		free(matrix);
		return NULL;
	}

	for (size_t i = 0; i < rows; i++)
	{
		matrix->data[i] = (double *)malloc(cols * sizeof(double));
		if (matrix->data[i] == NULL)
		{
			destroy_matrix(matrix);
			return NULL;
		}
	}
	return matrix;
}

void print_matrix(Matrix *matrix)
{
	for (int i = 0; i < matrix->rows; i++)
	{
		for (int j = 0; j < matrix->cols; j++)
		{
			printf("%g ", matrix->data[i][j]);
		}
		printf("\n");
	}
}

Matrix *i_matrix(size_t N)
{
	Matrix *result = create_matrix(N, N);
	for (int i = 0; i < N; i++)
	{
		result->data[i][i] = 1.;
	}
	return result;
}

Matrix *transpose(Matrix *s)
{
	Matrix *result = create_matrix(s->cols, s->rows);
	if (result == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < s->cols; i++)
	{
		for (int j = 0; j < s->rows; j++)
		{
			result->data[i][j] = s->data[j][i];
		}
	}

	return result;
}

double norm(Matrix *a)
{
	double ans = 0;
	for (int i = 0; i < a->rows; i++)
	{
		for (int j = 0; j < a->cols; j++)
		{
			ans += pow(a->data[i][j], 2);
		}
	}
	return pow(ans, 0.5);
}

void multiply_num_matrix(double n, Matrix *a, Matrix *result)
{
	for (int i = 0; i < a->cols; i++)
	{
		for (int j = 0; j < a->rows; j++)
		{
			result->data[i][j] = a->data[i][j] * n;
		}
	}
}

void subtract_left_down_angle(Matrix *a, Matrix *b)
{
	for (int i = 0; i < b->cols; i++)
	{
		for (int j = 0; j < b->cols; j++)
			a->data[a->rows - b->cols + i][a->rows - b->cols + j] -= b->data[i][j];
	}
}

Matrix *matrix_divide_num(Matrix *a, double num)
{
	Matrix *result = create_matrix(a->rows, a->cols);
	if (result == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < a->rows; i++)
	{
		for (int j = 0; j < a->cols; j++)
			result->data[i][j] = a->data[i][j] / num;
	}
	return result;
}

void multiply_matrices(Matrix *a, Matrix *b, Matrix *result)
{
	for (int i = 0; i < a->rows; i++)
	{
		for (int j = 0; j < b->cols; j++)
		{
			double sum = 0;
			for (int k = 0; k < a->cols; k++)
			{
				sum += a->data[i][k] * b->data[k][j];
			}
			result->data[i][j] = sum;
		}
	}
}
Matrix *copy(Matrix *a)
{
	Matrix *result = create_matrix(a->rows, a->cols);
	if (result == NULL)
	{
		return NULL;
	}
	for (int h = 0; h < a->rows; h++)
	{
		for (int h1 = 0; h1 < a->cols; h1++)
		{
			result->data[h][h1] = a->data[h][h1];
		}
	}
	return result;
}
void copy_(Matrix *a, Matrix *result)
{
	for (int h = 0; h < a->rows; h++)
	{
		for (int h1 = 0; h1 < a->cols; h1++)
		{
			result->data[h][h1] = a->data[h][h1];
		}
	}
}
Matrix *get_colum(Matrix *a, int i, int koef)
{
	Matrix *result = create_matrix(a->cols - i - koef, 1);
	if (result == NULL)
	{
		return NULL;
	}
	for (int j = i + koef; j < a->cols; j++)
	{
		result->data[j - i - koef][0] = a->data[j][i];
	}
	return result;
}

int convert_to_Hausholder_type(Matrix *a)
{
	for (int i = 0; i < a->cols - 2; i++)
	{
		Matrix *c = get_colum(a, i, 1);

		c->data[0][0] += norm(c);

		double omega = pow(norm(c), 2);
		if (omega == 0.)
		{
			destroy_matrix(c);
			continue;
		}
		double r = 2 / omega;
		Matrix *H = i_matrix(a->cols);
		if (H == NULL)
		{
			destroy_matrix(c);
			return 1;
		}
		Matrix *subH = create_matrix(c->rows, c->rows);
		if (subH == NULL)
		{
			destroy_matrix(c);
			destroy_matrix(H);
			return 1;
		}

		Matrix *c_T = transpose(c);
		if (c_T == NULL)
		{
			destroy_matrix(c);
			destroy_matrix(H);
			destroy_matrix(subH);
			return 1;
		}
		multiply_matrices(c, c_T, subH);

		destroy_matrix(c_T);
		destroy_matrix(c);

		multiply_num_matrix(r, subH, subH);
		subtract_left_down_angle(H, subH);

		destroy_matrix(subH);

		Matrix *A = create_matrix(a->rows, a->rows);

		if (A == NULL)
		{
			destroy_matrix(H);
			return 1;
		}
		multiply_matrices(H, a, A);
		multiply_matrices(A, H, a);

		destroy_matrix(A);
		destroy_matrix(H);
	}
	return 0;
}

Matrix *qr(Matrix *a, Matrix *Q)
{
	size_t n = a->cols;
	Matrix *r = copy(a);
	for (int i = 0; i < n - 1; i++)
	{
		Matrix *ai = transpose(get_colum(r, i, 0));
		double cur = ai->data[0][0] + copysign(norm(ai), ai->data[0][0]);
		if (cur == 0)
		{
			destroy_matrix(ai);
			continue;
		}
		Matrix *v = matrix_divide_num(ai, cur);
		destroy_matrix(ai);
		v->data[0][0] = 1;
		double omega = pow(norm(v), 2);
		if (omega == 0.)
		{
			destroy_matrix(v);
			continue;
		}
		Matrix *H = i_matrix(n);
		cur = 2 / omega;
		Matrix *vT = transpose(v);
		Matrix *vTv = create_matrix(vT->rows, v->cols);
		multiply_matrices(vT, v, vTv);
		Matrix *sub_vTv = create_matrix(vT->rows, v->cols);
		destroy_matrix(v);
		destroy_matrix(vT);
		multiply_num_matrix(cur, vTv, sub_vTv);
		destroy_matrix(vTv);
		subtract_left_down_angle(H, sub_vTv);
		destroy_matrix(sub_vTv);
		Matrix *sub_Q = create_matrix(r->rows, r->cols);
		multiply_matrices(Q, H, sub_Q);
		copy_(sub_Q, Q);
		destroy_matrix(sub_Q);
		Matrix *sub_A = create_matrix(r->rows, r->cols);
		multiply_matrices(H, r, sub_A);
		copy_(sub_A, r);
		destroy_matrix(sub_A);
		destroy_matrix(H);
	}
	return r;
}

int convergence(Matrix *a)
{
	int ans = 1;
	int c = 0;
	for (int i = 0; i < a->cols - 1; i++)
	{
		if (fabs(a->data[i + 1][i]) >= delta)
		{
			c++;
		}
		else
		{
			c = 0;
		}
		if (c == 3)
		{
			return 0;
		}
	}
	for (int i = 0; i < a->cols - 2; i++)
	{
		for (int j = 2 + i; j < a->cols; j++)
		{
			ans &= fabs(a->data[j][i]) < delta;
		}
	}
	return ans;
}
void output(FILE *out, Matrix *a, size_t n)
{
	for (int i = 0; i < n; i++)
	{
		if (i + 1 < n && fabs(a->data[i + 1][i]) >= delta)
		{
			double a1 = 1.;
			double a2 = -(a->data[i][i] + a->data[i + 1][i + 1]);
			double a3 = a->data[i][i] * a->data[i + 1][i + 1] - a->data[i][i + 1] * a->data[i + 1][i];
			double D = pow(a2, 2) - 4 * a1 * a3;
			if (D == 0)
			{
				fprintf(out, "%lg\n", (-a2 / (2 * a1)));
			}
			else if (D > 0)
			{
				fprintf(out, "%lg\n", (-a2 + pow(D, 0.5)) / (2 * a1));
				fprintf(out, "%lg\n", (-a2 - pow(D, 0.5)) / (2 * a1));
			}
			else
			{
				fprintf(out, "%lg +%lgi\n", -a2 / (2 * a1), (pow(fabs(D), 0.5) / (2 * a1)));
				fprintf(out, "%lg -%lgi\n", -a2 / (2 * a1), (pow(fabs(D), 0.5) / (2 * a1)));
			}
			i++;
		}
		else
		{
			fprintf(out, "%lg\n", a->data[i][i]);
		}
	}
}
Matrix *input(FILE *file)
{
	int n;
	fscanf(file, "%d", &n);
	Matrix *a = create_matrix(n, n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fscanf(file, "%lg", &a->data[i][j]);
		}
	}
	fclose(file);
	return a;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Error: cnt args != 2 \n");
		exit(1);
	}
	FILE *file = fopen(argv[1], "r");
	if (file == NULL)
	{
		printf("Error: cannot open file '%s'\n", argv[1]);
		return 1;
	}
	Matrix *a = input(file);
	size_t n = a->cols;
	int shod = 0;
	int it = 0;
	while (!shod && it < n * n * 10000)
	{
		Matrix *Q = i_matrix(n);
		it++;
		Matrix *r = qr(a, Q);
		Matrix *sub_A = create_matrix(n, n);
		multiply_matrices(r, Q, sub_A);
		shod = convergence(a);
		destroy_matrix(Q);
		destroy_matrix(r);
		if (convert_to_Hausholder_type(sub_A))
		{
			destroy_matrix(sub_A);
			destroy_matrix(a);
			return 1;
		}

		if (isnan(sub_A->data[0][0]))
		{
			destroy_matrix(sub_A);
			break;
		}
		copy_(sub_A, a);
		destroy_matrix(sub_A);
	}
	FILE *out = fopen(argv[2], "w");
	if (out == NULL)
	{
		destroy_matrix(a);
		printf("Error: cannot open file '%s'\n", argv[2]);
		return 1;
	}
	output(out, a, n);
	fclose(out);
	destroy_matrix(a);
	return 0;
}