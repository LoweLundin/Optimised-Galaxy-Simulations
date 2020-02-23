#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static void update_maxdiff(double dx, double dy, double* maxabsdiff) {
  double absdiff = sqrt(dx*dx+dy*dy);
  if(absdiff > *maxabsdiff)
    *maxabsdiff = absdiff;
}

int read_doubles_from_file(int n, double* p, const char* fileName) {
  /* Open input file and determine its size. */
  FILE* input_file = fopen(fileName, "rb");
  if(!input_file) {
    printf("read_doubles_from_file error: failed to open input file '%s'.\n", fileName);
    return -1;
  }
  /* Get filesize using fseek() and ftell(). */
  fseek(input_file, 0L, SEEK_END);
  size_t fileSize = ftell(input_file);
  /* Now use fseek() again to set file position back to beginning of the file. */
  fseek(input_file, 0L, SEEK_SET);
  if(fileSize != n * sizeof(double)) {
    printf("read_doubles_from_file error: size of input file '%s' does not match the given n.\n", fileName);
    printf("For n = %d the file size is expected to be (n * sizeof(double)) = %lu but the actual file size is %lu.\n",
	   n, n * sizeof(double), fileSize);
    return -1;
  }
  /* Read contents of input_file into buffer. */
  size_t noOfItemsRead = fread(p, sizeof(char), fileSize, input_file);
  if(noOfItemsRead != fileSize) {
    printf("read_doubles_from_file error: failed to read file contents into buffer.\n");
    return -1;
  }
  /* OK, now we have the file contents in the buffer.
     Since we are finished with the input file, we can close it now. */
  if(fclose(input_file) != 0) {
    printf("read_doubles_from_file error: error closing input file.\n");
    return -1;
  }
  /* Return 0 to indicate success. */
  return 0;
}

/* The idea with the check_that_numbers_seem_OK() function is to check
   that there are no strange numbers like "nan" that may give problems
   when we try to compare the numbers later. */
int check_that_numbers_seem_OK(int n, double* buf) {
  const double minAllowedValue = -1e10;
  const double maxAllowedValue = 1e10;
  int OKcount = 0;
  int i;
  for(i = 0; i < n; i++) {
    double a = buf[i];
    if(a >= minAllowedValue && a <= maxAllowedValue)
      OKcount++;
  }
  if(OKcount == n)
    return 0;
  else
    return -1;
}

int main(int argc, const char* argv[]) {
  if(argc != 4) {
    printf("Give 3 input args: N gal1.gal gal2.gal\n");
    return -1;
  }
  int N = atoi(argv[1]);
  const char* fileName1 = argv[2];
  const char* fileName2 = argv[3];
  printf("N = %d\n", N);
  printf("fileName1 = '%s'\n", fileName1);
  printf("fileName2 = '%s'\n", fileName2);
  /* Read files. */
  double buf1[6*N];
  if(read_doubles_from_file(6*N, buf1, fileName1) != 0) {
    printf("Error reading file '%s'\n", fileName1);
    return -1;
  }
  if(check_that_numbers_seem_OK(6*N, buf1) != 0) {
    printf("Error: strange numbers found in file '%s'.\n", fileName1);
    return -1;
  }
  double buf2[6*N];
  if(read_doubles_from_file(6*N, buf2, fileName2) != 0) {
    printf("Error reading file '%s'\n", fileName2);
    return -1;
  }
  if(check_that_numbers_seem_OK(6*N, buf2) != 0) {
    printf("Error: strange numbers found in file '%s'.\n", fileName2);
    return -1;
  }
  /* Compare positions and velocities. */
  double pos_maxdiff = 0;
  double vel_maxdiff = 0;
  int i;
  for(i = 0; i < N; i++) {
    double pos_dx = buf1[i*6+0] - buf2[i*6+0];
    double pos_dy = buf1[i*6+1] - buf2[i*6+1];
    double m_diff = buf1[i*6+2] - buf2[i*6+2];
    double vel_dx = buf1[i*6+3] - buf2[i*6+3];
    double vel_dy = buf1[i*6+4] - buf2[i*6+4];
    double b_diff = buf1[i*6+5] - buf2[i*6+5];
    if(fabs(m_diff) > 1e-9) {
      printf("ERROR: mass values do not match.\n");
      return -1;
    }
    if(fabs(b_diff) > 1e-9) {
      printf("ERROR: 'brightness' values do not match.\n");
      return -1;
    }
    update_maxdiff(pos_dx, pos_dy, &pos_maxdiff);
    update_maxdiff(vel_dx, vel_dy, &vel_maxdiff);
  }
  printf("pos_maxdiff = %16.12f\n", pos_maxdiff);
  return 0;
}
