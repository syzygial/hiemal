#include "util.h"

int parse_test_list(testing_results_t *test_list, char *fmt) {
  int i = 0;
  int n_test_char;
  int cur_test = 0;
  int range_end = 0;
  if (fmt == NULL) {
    for (i = 0; i < test_list->n_tests; i++) test_list->tests[i].run_test = true;
    return 0;
  }
  for (i = 0; i < test_list->n_tests; i++) test_list->tests[i].run_test = false;
  char *fmt_copy = strdup(fmt);
  char *token = strtok(fmt_copy, ",");
  while (token != NULL) {
    if (strchr(token, '-') == NULL) { // single test
      cur_test = atoi(token);
      test_list->tests[cur_test-1].run_test = true; // subtract 1 to convert to 0-index
    }
    else { // range of tests
      n_test_char = strcspn(token, "-");
      token[n_test_char] = '\0';
      cur_test = atoi(token);
      range_end = atoi(token + n_test_char + 1);
      for (i = cur_test; i <= range_end; i++) test_list->tests[i-1].run_test = true;
      token[n_test_char] = '-';
    }
    token = strtok(NULL, ",");
  }
  free(fmt_copy);
  return 0;
}

int print_results_json(char *filename, const testing_results_t *results) {
  FILE *fp = fopen(filename, "w");
  fwrite("{\n", 1, 2, fp);
  fwrite("  \"results\":\n", 1, 13, fp);
  int i;
  // TODO: calculate the exact number of bytes to allocate
  char *line_buf = (char*)malloc(256);
  for (i = 0; i < results->n_tests; i++) {
    test_info_t *current_test = &(results->tests[i]);
    memset(line_buf, 0, 256);
    sprintf(line_buf, "    %s{\"id\": %d, \"name\": \"%s\", \"result\": \"%s\", \"elapsed_time\": %.2f}%s", \
      (i==0) ? "[" : "", i+1, current_test->test_name, (current_test->result == OK) ? "OK" : "FAILED", \
      current_test->elapsed_time, ((i+1)==results->n_tests) ? "]" : ",\n");
    fwrite(line_buf, 1, strlen(line_buf), fp);
  }
  free(line_buf);
  fwrite("\n}\n", 1, 3 ,fp);
  fclose(fp);
  return 0;
}