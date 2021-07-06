#include "test_pas-co2-mock.hpp"
#include "pas-co2-pulse.hpp"

/**
 * Class C++ PASCO2 Pulse Class Test Suite
 */
class PASCO2Pulse_Test: public ::testing::Test
{
    public:
        
        NiceMock<MockPWM> pwm;
        NiceMock<MockGPIO> power3V3;
        NiceMock<MockGPIO> power12V;
            
        // uint8_t regVal[2] = {0x00, 0x80};

        void SetUp()
        {
            ON_CALL(pwm, init())
            .WillByDefault(Return(OK));

            ON_CALL(pwm, deinit())
            .WillByDefault(Return(OK));

            ON_CALL(power3V3, init())
            .WillByDefault(Return(OK));

            ON_CALL(power3V3, deinit())
            .WillByDefault(Return(OK));

            ON_CALL(power3V3, enable())
            .WillByDefault(Return(OK));

            ON_CALL(power3V3, disable())
            .WillByDefault(Return(OK));

            ON_CALL(power12V, init())
            .WillByDefault(Return(OK));

            ON_CALL(power12V, deinit())
            .WillByDefault(Return(OK));

            ON_CALL(power12V, enable())
            .WillByDefault(Return(OK));

            ON_CALL(power12V, disable())
            .WillByDefault(Return(OK));
        }

        void TearDown()
        {

        }
};

/**
 * enable() tests
 */

TEST_F(PASCO2Pulse_Test, enable_initError)
{
    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Pulse cotwo(&pwm);

    ASSERT_EQ(INTF_ERROR, cotwo.enable());
}

TEST_F(PASCO2Pulse_Test, enable_enableIREmitterError)
{
    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(INTF_ERROR, cotwo.enable());
}

TEST_F(PASCO2Pulse_Test, enable_power3V3EnableError)
{
    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, &power3V3);

    ASSERT_EQ(INTF_ERROR, cotwo.enable());
}

TEST_F(PASCO2Pulse_Test, enable_Success)
{
    PASCO2Pulse cotwo(&pwm, nullptr);

    ASSERT_EQ(OK, cotwo.enable());
}

/**
 * disable() tests
 */

TEST_F(PASCO2Pulse_Test, disable_power12VDisableError)
{
    EXPECT_CALL(power12V, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(INTF_ERROR, cotwo.disable());
}

TEST_F(PASCO2Pulse_Test, disable_power3V3DisableError)
{
    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr,  &power3V3);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(INTF_ERROR, cotwo.disable());
}

TEST_F(PASCO2Pulse_Test, disable_DeinitError)
{
    EXPECT_CALL(pwm, deinit())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, &power3V3, &power12V);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(INTF_ERROR, cotwo.disable());
}

TEST_F(PASCO2Pulse_Test, disable_Success)
{
    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, &power3V3, &power12V);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(OK, cotwo.disable());
}

/**
 * startMeasure() tests
 */

TEST_F(PASCO2Pulse_Test, startMeasure_enablePWMError)
{
    MockGPIO pwmSelect;

    EXPECT_CALL(pwmSelect, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwmSelect, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(pwmSelect, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, nullptr, nullptr, &pwmSelect);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure());
}

TEST_F(PASCO2Pulse_Test, startMeasure_disableInterruptError)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, disableInt())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr, &interrupt);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure());
}

TEST_F(PASCO2Pulse_Test, startMeasure_disableInterruptSuccess)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, disableInt())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr, &interrupt);

    ASSERT_EQ(OK, cotwo.startMeasure());
}

static void cBack (void *){ };

TEST_F(PASCO2Pulse_Test, startMeasure_enableInterruptError)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, enableInt(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr, &interrupt);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure(cBack));
}

TEST_F(PASCO2Pulse_Test, startMeasure_EnableIntSuccess)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, enableInt(_))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr, &interrupt);

    ASSERT_EQ(OK, cotwo.startMeasure(cBack));
}

TEST_F(PASCO2Pulse_Test, startMeasure_NoInstancesSuccess)
{
    PASCO2Pulse cotwo(&pwm);

    ASSERT_EQ(OK, cotwo.startMeasure());
}

/**
 * stopMeasure() tests
 */

TEST_F(PASCO2Pulse_Test, stopMeasure_disablePWMError)
{
    MockGPIO pwmSelect;

    EXPECT_CALL(pwmSelect, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwmSelect, disable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(pwmSelect, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, nullptr, nullptr, &pwmSelect);

    ASSERT_EQ(INTF_ERROR, cotwo.stopMeasure());
}

TEST_F(PASCO2Pulse_Test, stopMeasure_Success)
{
    MockGPIO pwmSelect;

    EXPECT_CALL(pwmSelect, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwmSelect, disable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwmSelect, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, nullptr, nullptr, &pwmSelect);

    ASSERT_EQ(OK, cotwo.stopMeasure());
}

/**
 * getCO2() tests
 */

TEST_F(PASCO2Pulse_Test, getCO2_getCO2Error)
{  
    int16_t  co2ppm = 0;

    EXPECT_CALL(pwm, getDuty(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Pulse cotwo(&pwm, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.getCO2(co2ppm));
    ASSERT_EQ(-1, co2ppm);
}

TEST_F(PASCO2Pulse_Test, getCO2_Success)
{  
    int16_t  co2ppm = 0;

    EXPECT_CALL(pwm, getDuty(_))
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pulse cotwo(&pwm, nullptr);

    ASSERT_EQ(OK, cotwo.getCO2(co2ppm));
    ASSERT_EQ(0, co2ppm);
}


/**
 * reset() tests
 */

TEST_F(PASCO2Pulse_Test, reset_softResetError)
{
    {
        InSequence s;

        EXPECT_CALL(power3V3, disable())
        .Times(1)
        .WillOnce(Return(INTF_ERROR));

        EXPECT_CALL(power3V3, disable())
        .Times(1)
        .WillOnce(Return(OK));
    }
    PASCO2Pulse cotwo(&pwm, nullptr, nullptr, &power3V3);

    ASSERT_EQ(INTF_ERROR, cotwo.reset());
}

TEST_F(PASCO2Pulse_Test, reset_Success)
{
    PASCO2Pulse cotwo(&pwm, nullptr);

    ASSERT_EQ(OK, cotwo.reset());
}
