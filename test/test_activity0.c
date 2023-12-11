#include "unity.h"
// #include "main.c"

void setUp(void)
{
    // This is run before EACH test
}

void tearDown(void)
{
    // This is run after EACH test
}

void test_tx_callback_function(void)
{
    // Define a test case where ret_val is not 0
    uint32_t ret_val = 1;
    char *info = "Test Info";

    // Redirect stdout to a buffer
    char buffer[50];
    freopen("/dev/null", "a", stdout);
    setbuf(stdout, buffer);

    // Call the function
    tx_callback_function(ret_val, info);

    // Check the output
    UNITY_TEST_ASSERT_EQUAL_STRING("TX Callback: From Test Info \t Error Code: 1\n", buffer, __LINE__, "TX Callback Function Test Failed");

    // Reset stdout
    freopen ("/dev/tty", "a", stdout);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_tx_callback_function);
    return UNITY_END();
}