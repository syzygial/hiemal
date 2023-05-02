#include <dirent.h>
#include <string.h>
#include <stdio.h>

#include "intern/alsa.h"
#include "test_common.h"

TEST(alsa_n_cards) {
  int n_cards = 0;
  //query procfs for ground truth
  DIR *alsa_proc = opendir("/proc/asound");
  struct dirent *d = readdir(alsa_proc);

  while(d != NULL) {
    if (d->d_type == DT_DIR && strncmp(d->d_name, "card", 4) == 0) {
      n_cards++;
    }
    d = readdir(alsa_proc);
  }
  closedir(alsa_proc);
  int rc = hm_alsa_n_cards();
  ASSERT_TRUE(rc == n_cards)
  return TEST_SUCCESS;
}

TEST(alsa_n_pcm) {
  FILE *pcm_file = fopen("/proc/asound/pcm", "r");

  char *line_ptr = NULL;
  ssize_t line_ptr_bytes = 0;
  const int n_cards = hm_alsa_n_cards();
  int n_pcm = 0; // ground truth
  int n_pcm_test = 0;

  int n_bytes_read = getline(&line_ptr, &line_ptr_bytes, pcm_file);
  while (n_bytes_read != -1) {
    if (strstr(line_ptr, "playback") != NULL) {
      n_pcm++;
    }
    if (strstr(line_ptr, "capture") != NULL) {
      n_pcm++;
    }
    n_bytes_read = getline(&line_ptr, &line_ptr_bytes, pcm_file);
  }
  
  int ii = 0;
  for (ii = 0; ii < n_cards; ii++) {
    n_pcm_test += hm_alsa_n_pcm(ii);
  }
  free(line_ptr);
  fclose(pcm_file);
  ASSERT_TRUE(n_pcm == n_pcm_test);
  return TEST_SUCCESS;
}

TEST(alsa_default_io) {
  hm_device_io_t *io = NULL;
  int rc = hm_alsa_default_io_init(&io, PLAYBACK);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(io != NULL);
  rc = hm_alsa_device_io_delete(&io);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(io == NULL);
  return TEST_SUCCESS;
}