#include "sunspots.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

unsigned read_year(char * line, char ** end_pointer);
unsigned read_month(char * month_str, char ** end_pointer);
double read_num(char * num_str, char ** end_pointer);
/* Parse a line string to ss_monthly_t*/
ss_monthly_t parseLine(char * line) {
  // WRITE ME
  ss_monthly_t ans;
  // corner case: input is NULL pointer
  if (line == NULL) {
    fprintf(stderr, "Can't read a NULL line\n");
    exit(EXIT_FAILURE);
  }
  // Allocate some dummy pointers on the stack
  char * str_loc = NULL;
  char ** end_pointer = &str_loc;
  // read year
  ans.year = read_year(line, end_pointer);
  // check hyphen
  if (**end_pointer != '-') {
    fprintf(stderr, "The format is not YYYY-MM, which is wrong!\n");
    exit(EXIT_FAILURE);
  }
  //read month
  char * month_str = ++(*end_pointer);
  ans.month = read_month(month_str, end_pointer);
  // check comma
  if (**end_pointer != ',') {
    fprintf(stderr, "The separator between date and number should be comma!\n");
    exit(EXIT_FAILURE);
  }
  //read number of sunspots
  char * num_str = ++(*end_pointer);
  ans.num = read_num(num_str, end_pointer);
  // check end of the string
  while (**end_pointer == '\n')
    (*end_pointer)++;
  if (**end_pointer != '\0') {
    fprintf(stderr, "The string is not properly ended!\n");
    exit(EXIT_FAILURE);
  }
  return ans;
}
/* Read the year from the line and store the pointer after reading to end_pointer */
unsigned read_year(char * line, char ** end_pointer) {
  long year = strtol(line, end_pointer, 10);
  if (end_pointer == NULL) {
    printf("test : end_pointer is wrong, input is %s", line);
  }
  if (*end_pointer != line + 4 || !isdigit(*line)) {
    fprintf(stderr, "The format is not YYYY-MM, which is wrong\n");
    exit(EXIT_FAILURE);
  }
  else if (year < 0 || year > 9999) {
    fprintf(stderr, "The year is not within the acceptable range.\n");
    exit(EXIT_FAILURE);
  }
  return (unsigned)year;
}
/* Read the month starting from month_str and store the pointer after reading to end_pointer */
unsigned read_month(char * month_str, char ** end_pointer) {
  long month = strtol(month_str, end_pointer, 10);
  if (*end_pointer != month_str + 2 || !isdigit(*month_str)) {
    fprintf(stderr, "The format is not YYYY-MM, which is wrong!\n");
    exit(EXIT_FAILURE);
  }
  else if (month <= 0 || month > 12) {
    fprintf(stderr, "The month is not within the acceptable range.\n");
    exit(EXIT_FAILURE);
  }
  return (unsigned)month;
}
/* Read the number of sunspots starting from num_str and store the pointer after reading to end_pointer */
double read_num(char * num_str, char ** end_pointer) {
  double num = strtod(num_str, end_pointer);
  if (!isdigit(*num_str) && *num_str != '.') {
    fprintf(stderr, "The format for the number of sunspots is wrong!\n");
    exit(EXIT_FAILURE);
  }
  else if (*end_pointer - num_str <= 0) {
    fprintf(stderr, "The number of sunspots is not found!\n");
    exit(EXIT_FAILURE);
  }
  else if (num < 0) {
    fprintf(stderr, "The number of sunspots should be non-negative!\n");
    exit(EXIT_FAILURE);
  }
  return num;
}
double calculate_mean(ss_monthly_t * data, size_t n, size_t index, unsigned w);
/* Calculate the moving average of data with width w and store in mean*/
void meanFilter(ss_monthly_t * data, size_t n, ss_monthly_t * mean, unsigned w) {
  if (data == NULL || mean == NULL || w % 2 == 0 || w > n) {
    fprintf(stderr, "Some input of meanFilter is inappropriate\n");
    exit(EXIT_FAILURE);
  }
  // do we need to sizeof to make sure lengths are the same?
  for (size_t i = 0; i < n; i++) {
    mean[i].year = data[i].year;
    mean[i].month = data[i].month;
    mean[i].num = calculate_mean(data, n, i, w);
  }
}
/* Calculate the mean given the data, current index and odd w*/
double calculate_mean(ss_monthly_t * data, size_t n, size_t index, unsigned w) {
  unsigned half = w / 2;
  size_t lower_bound = index >= half ? index - half : 0;
  size_t upper_bound = index + half < n ? index + half : n - 1;
  size_t length = upper_bound - lower_bound + 1;
  double sum = 0;
  for (size_t i = lower_bound; i <= upper_bound; i++) {
    sum += data[i].num;
  }
  // the more accurate result should be using long double to sum and cast to double
  double mean = sum / length;
  return mean;
}
/* Calculate the maximum value of the number of sunspots*/
double findLocalMax(ss_monthly_t * data, size_t n) {
  if (data == NULL || n == 0) {
    fprintf(stderr, "Some input of findLocalMax is inappropriate\n");
    exit(EXIT_FAILURE);
  }
  size_t max_index = 0;
  double max = -1.0;
  for (size_t i = 0; i < n; i++) {
    if (data[i].num > max) {
      max = data[i].num;
      max_index = i;
    }
  }
  return data[max_index].year + data[max_index].month / 12.0;
}

/* Calculate the period of sunspots */
double calcSsPeriod(double * timeStamps, size_t n) {
  if (timeStamps == NULL || n < 2) {
    fprintf(stderr, "Some input of calcSsperiod is inappropriate");
    exit(EXIT_FAILURE);
  }
  double sum = timeStamps[n - 1] - timeStamps[0];
  return sum / (n - 1);
}
