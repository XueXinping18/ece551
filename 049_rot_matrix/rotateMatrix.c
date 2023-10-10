#include <stdio.h>
#include <stdlib.h>

int load_matrix(int matrix[][10], FILE * fp);
void rotate(int matrix[][10]);
int main(int argc, char ** argv) {
  if (argc != 2) {
    fprintf(stderr, "wrong number of CLAs");
    return EXIT_FAILURE;
  }
  FILE * fp = fopen(argv[1], "r");
  if (fp == NULL) {
    fprintf(stderr, "fail to open the file");
    return EXIT_FAILURE;
  }
  int matrix[10][10];
  if (!load_matrix(matrix, fp)) {
    fprintf(stderr, "The content of file is bad");
    return EXIT_FAILURE;
  };
  rotate(matrix);
  if (fclose(fp) != 0) {
    perror("Fail to close the file!");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
int load_matrix(int matrix[][10], FILE * fp) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      int c = fgetc(fp);
      // is '\0' acceptable?
      if (c == EOF || c == 0x0a || c == 0)
        return 0;
      matrix[i][j] = c;
    }
    int new_line = fgetc(fp);
    // is last line required for new line?
    if (new_line != 0x0a)
      return 0;
  }
  if (fgetc(fp) != EOF)
    return 0;
  return 1;
}
void rotate(int matrix[][10]) {
  for (int j = 0; j < 10; j++) {
    for (int i = 9; i >= 0; i--) {
      printf("%c", matrix[i][j]);
    }
    printf("\n");
  }
}
