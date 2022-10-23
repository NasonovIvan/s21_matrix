#include "s21_matrix.h"

// Функция проверки корректности матрицы
int check_matrix(matrix_t *A) {
  int error = OK;
  if (!A || A->rows <= 0 || A->columns <= 0) error = ERR_MATRIX;
  return error;
}

// Создание матриц (create_matrix)
int s21_create_matrix(int rows, int columns, matrix_t *result) {
  if (!result || rows <= 0 || columns <= 0) return ERR_MATRIX;
  result->rows = rows;
  result->columns = columns;
  result->matrix = calloc(rows, sizeof(double *));
  for (int i = 0; i < rows; i++) {
    result->matrix[i] = calloc(columns, sizeof(double));
  }
  return OK;
}

// Очистка матриц (remove_matrix)
void s21_remove_matrix(matrix_t *A) {
  if (!check_matrix(A)) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

// Сравнение матриц (eq_matrix)
int s21_eq_matrix(matrix_t *A, matrix_t *B) {
  if (!A || !B || A->columns != B->columns || A->rows != B->rows)
    return FAILURE;
  int result = SUCCESS;
  double precision = 1e-7;
  double help_var = 0;
  double frac_part_number_A = 0;
  double frac_part_number_B = 0;
  int int_part_number_A = 0;
  int int_part_number_B = 0;
  if (result) {
    for (int i = 0; i < A->rows; i++) {
      for (int j = 0; j < A->columns; j++) {
        if (fabs(A->matrix[i][j] - B->matrix[i][j]) >= precision) {
          result = FAILURE;
          break;
        } else {
          frac_part_number_A = modf(A->matrix[i][j], &help_var);
          frac_part_number_B = modf(B->matrix[i][j], &help_var);
          frac_part_number_A = frac_part_number_A * 1e7;
          frac_part_number_B = frac_part_number_B * 1e7;
          int_part_number_A = (int)frac_part_number_A;
          int_part_number_B = (int)frac_part_number_B;
          if (abs(int_part_number_A - int_part_number_B) != 0) {
            result = FAILURE;
            break;
          }
        }
      }
      if (!result) break;
    }
  }
  return result;
}

// Сложение (sum_matrix)
int s21_sum_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (check_matrix(A) || check_matrix(B) || !result) return ERR_MATRIX;
  if (A->columns != B->columns || A->rows != B->rows) return ERR_COUNT;
  // int result_func = OK;
  s21_create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
    }
  }
  return OK;
}

// Вычитание матриц (sub_matrix)
int s21_sub_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (check_matrix(A) || check_matrix(B) || !result) return ERR_MATRIX;
  if (A->columns != B->columns || A->rows != B->rows) return ERR_COUNT;
  s21_create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
    }
  }
  return OK;
}

// Умножение матрицы на число (mult_number)
int s21_mult_number(matrix_t *A, double number, matrix_t *result) {
  if (check_matrix(A) || !result) return ERR_MATRIX;
  s21_create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[i][j] = A->matrix[i][j] * number;
    }
  }
  return OK;
}

// Умножение двух матриц (mult_matrix)
int s21_mult_matrix(matrix_t *A, matrix_t *B, matrix_t *result) {
  if (check_matrix(A) || check_matrix(B) || !result) return ERR_MATRIX;
  if (A->columns != B->rows) return ERR_COUNT;
  s21_create_matrix(A->rows, B->columns, result);
  double sum = 0;
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < B->columns; j++) {
      sum = 0;
      for (int k = 0; k < A->columns; k++) {
        sum += A->matrix[i][k] * B->matrix[k][j];
      }
      result->matrix[i][j] = sum;
    }
  }
  return OK;
}

// Транспонирование матрицы (transpose)
int s21_transpose(matrix_t *A, matrix_t *result) {
  if (check_matrix(A) || !result) return ERR_MATRIX;
  s21_create_matrix(A->columns, A->rows, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      result->matrix[j][i] = A->matrix[i][j];
    }
  }
  return OK;
}

// Определитель матрицы (determinant)
int s21_determinant(matrix_t *A, double *result) {
  if (check_matrix(A) || !result) return ERR_MATRIX;
  if (A->columns != A->rows) return ERR_COUNT;
  matrix_t help_mat;
  double help_det = 0;
  int help_colm_A = 0;
  int help_colm_res = 0;
  *result = 0;
  if (A->rows == 1) {
    *result = A->matrix[0][0];
  } else if (A->rows == 2) {
    *result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[1][0] * A->matrix[0][1];
  } else {
    for (int i = 0; i < A->columns; i++) {
      s21_create_matrix(A->rows - 1, A->columns - 1, &help_mat);
      for (int k = 1, l = 0; k < A->rows; k++, l++) {
        help_colm_A = 0;
        help_colm_res = 0;
        while (help_colm_A < A->columns) {
          if (help_colm_A == i) {
            help_colm_A++;
            continue;
          }
          help_mat.matrix[l][help_colm_res] = A->matrix[k][help_colm_A];
          help_colm_res++;
          help_colm_A++;
        }
      }
      s21_determinant(&help_mat, &help_det);
      *result += pow(-1.0, i) * A->matrix[0][i] * help_det;
      s21_remove_matrix(&help_mat);
    }
  }
  return OK;
}

// Минор матрицы и матрица алгебраических дополнений (calc_complements)
int s21_calc_complements(matrix_t *A, matrix_t *result) {
  if (check_matrix(A) || !result) return ERR_MATRIX;
  if (A->columns != A->rows) return ERR_COUNT;
  matrix_t minor_matrix;
  double minor_det = 0;
  int i_A = 0;
  int j_A = 0;
  int rows_minor = 0;
  int colm_minor = 0;
  s21_create_matrix(A->rows, A->columns, result);
  for (int i = 0; i < A->rows; i++) {
    for (int j = 0; j < A->columns; j++) {
      s21_create_matrix(A->rows - 1, A->columns - 1, &minor_matrix);
      i_A = 0;
      rows_minor = 0;
      while (i_A < A->rows) {
        if (i_A == i) {
          i_A++;
          continue;
        }
        j_A = 0;
        colm_minor = 0;
        while (j_A < A->columns) {
          if (j_A == j) {
            j_A++;
            continue;
          }
          minor_matrix.matrix[rows_minor][colm_minor] = A->matrix[i_A][j_A];
          colm_minor++;
          j_A++;
        }
        rows_minor++;
        i_A++;
      }
      minor_det = 0;
      s21_determinant(&minor_matrix, &minor_det);
      result->matrix[i][j] = pow(-1, (i + j)) * minor_det;
      s21_remove_matrix(&minor_matrix);
    }
  }
  return OK;
}

// Обратная матрица (inverse_matrix)
int s21_inverse_matrix(matrix_t *A, matrix_t *result) {
  if (!A || !A->matrix || !result) return ERR_MATRIX;
  double det = 0;
  s21_determinant(A, &det);
  if (A->columns <= 0 || A->rows <= 0 || det == 0) return ERR_COUNT;

  matrix_t algebra_matrix = {NULL, 0, 0};
  matrix_t transponse_matrix = {NULL, 0, 0};
  s21_calc_complements(A, &algebra_matrix);
  s21_transpose(&algebra_matrix, &transponse_matrix);
  s21_mult_number(&transponse_matrix, 1 / det, result);
  s21_remove_matrix(&algebra_matrix);
  s21_remove_matrix(&transponse_matrix);

  return OK;
}
