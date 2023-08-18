#include "intern/alsa.h"
#include "intern/pulse.h"
#include "test_common.h"

TEST(pulse_connection) {
  hm_backend_connection_t *pulse_backend = NULL;
  int rc = hm_pulse_connection_init(&pulse_backend);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(pulse_backend != NULL);
  rc = hm_pulse_connection_close(&pulse_backend);
  ASSERT_TRUE(rc == 0);
  ASSERT_TRUE(pulse_backend == NULL);
  return TEST_SUCCESS;
}

// TODO: fix test failure/discrepancy when bluetooth audio devices
// are connected
TEST(pulse_n_cards) {
  hm_backend_connection_t *pulse_backend = NULL;
  hm_pulse_connection_init(&pulse_backend);
  int n_cards_test = hm_pulse_n_cards(pulse_backend);
  // use alsa for ground truth
  int n_cards = hm_alsa_n_cards();
  hm_pulse_connection_close(&pulse_backend);
  ASSERT_TRUE(n_cards_test == n_cards);
  return TEST_SUCCESS;
}