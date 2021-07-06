
#include "test_pas-co2-mock.hpp"

/**
 * Class C++ PASCO2 Class Test Suite
 */
class PASCO2SBus_Test: public ::testing::Test
{
    public:

        NiceMock<MockSBus> sbus;
        NiceMock<MockPWM>  pwm;

        void SetUp()
        {
            ON_CALL(sbus, init())
            .WillByDefault(Return(OK));

            ON_CALL(sbus, deinit())
            .WillByDefault(Return(OK));

            ON_CALL(sbus, read(_, _, _))
            .WillByDefault(Return(OK));

            ON_CALL(sbus, write(_, _, _))
            .WillByDefault(Return(OK));

            ON_CALL(pwm, init())
            .WillByDefault(Return(OK));

            ON_CALL(pwm, deinit())
            .WillByDefault(Return(OK));
        }

        void TearDown()
        {

        }
};


/**
 * softReset() tests
 */

TEST_F(PASCO2SBus_Test, softReset_SBusInstanceNullSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);
   
    ASSERT_EQ(OK, cotwo.softReset());
}

TEST_F(PASCO2SBus_Test, softReset_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.softReset());
}

TEST_F(PASCO2SBus_Test, softReset_preResetSaveConfigError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.softReset());
}

TEST_F(PASCO2SBus_Test, softReset_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.softReset());
}

TEST_F(PASCO2SBus_Test, softReset_TimerDelayError)
{
    MockTimer timer;

    MockPASCO2 cotwo(&sbus, nullptr, &timer);

    EXPECT_CALL(timer, delay(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.softReset());
}

TEST_F(PASCO2SBus_Test, softReset_TimerDelaySuccess)
{
    MockTimer timer;

    MockPASCO2 cotwo(&sbus, nullptr, &timer);

    EXPECT_CALL(timer, delay(_))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, postResetRestoreConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.softReset());
}

TEST_F(PASCO2SBus_Test, softReset_postResetRestoreConfigError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, postResetRestoreConfig())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.softReset());
}

TEST_F(PASCO2SBus_Test, softReset_Success)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, postResetRestoreConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.softReset());
}

/**
 * hardReset() tests
 */

TEST_F(PASCO2SBus_Test, hardReset_NullPower3V3Instance)
{
    PASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_setStatusError)
{
    MockGPIO power3V3;

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_preResetSaveConfigError)
{
    MockGPIO power3V3;

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_power3V3DisableError)
{
    MockGPIO power3V3;

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_FirstTimerDelayError)
{
    MockGPIO  power3V3;
    MockTimer timer;

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, delay(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, &timer, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_power3V3EnableError)
{
    MockGPIO power3V3;

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_SecondTimerDelayError)
{
    MockGPIO  power3V3;
    MockTimer timer;

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    {
        InSequence s;

        EXPECT_CALL(timer, delay(_))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(timer, delay(_))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));

    }

    MockPASCO2 cotwo(&sbus, nullptr, &timer, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.hardReset());
}


TEST_F(PASCO2SBus_Test, hardReset_postResetRestoreConfigError)
{
    MockGPIO power3V3;
    MockTimer timer;

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, delay(_))
    .Times(2)
    .WillRepeatedly(Return(OK));

    MockPASCO2 cotwo(&sbus, nullptr, &timer, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(cotwo, postResetRestoreConfig())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_WithSBusSuccess)
{
    MockGPIO power3V3;

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(OK));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, preResetSaveConfig())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(cotwo, postResetRestoreConfig())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.hardReset());
}

TEST_F(PASCO2SBus_Test, hardReset_NoSBusSuccess)
{
    MockGPIO power3V3;

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(OK));

    MockPASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.hardReset());
}

/**
 * setOperationMode() tests
 */

TEST_F(PASCO2SBus_Test, setOperationMode_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setOperationMode(MODE_IDLE));
}

TEST_F(PASCO2SBus_Test, setOperationMode_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.setOperationMode(MODE_IDLE));
}

TEST_F(PASCO2SBus_Test, setOperationMode_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.setOperationMode(MODE_SINGLE_SHOT));
}

TEST_F(PASCO2SBus_Test, setOperationMode_Success)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillOnce(Return(OK));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.setOperationMode(MODE_SINGLE_SHOT));
}

/**
 * enablePWM() tests
 */

TEST_F(PASCO2SBus_Test, enablePWM_NullPWMInterfaceSuccess)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.enablePWM());
}

TEST_F(PASCO2SBus_Test, enablePWM_setStatusError)
{
    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.enablePWM());
}

TEST_F(PASCO2SBus_Test, enablePWM_PWMSelectEnableError)
{
    MockGPIO pwmSelect;

    EXPECT_CALL(pwmSelect, enable())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, &pwm, nullptr, nullptr, nullptr, nullptr, nullptr, &pwmSelect);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enablePWM());
}

TEST_F(PASCO2SBus_Test, enablePWM_PWMSelectEnableSuccess)
{
    MockGPIO pwmSelect;

    EXPECT_CALL(pwmSelect, enable())
    .Times(1)
    .WillRepeatedly(Return(OK));

    MockPASCO2 cotwo(&sbus, &pwm, nullptr, nullptr, nullptr, nullptr, nullptr, &pwmSelect);
    
    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.enablePWM());
}

TEST_F(PASCO2SBus_Test, enablePWM_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enablePWM());
}

TEST_F(PASCO2SBus_Test, enablePWM_SBusSuccess)
{
    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.enablePWM());
}

TEST_F(PASCO2SBus_Test, enablePWM_Success)
{
    MockPASCO2 cotwo(nullptr, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.enablePWM());
}

/**
 * disablePWM() tests
 */

TEST_F(PASCO2SBus_Test, disablePWM_NullPWMInterfaceSuccess)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.disablePWM());
}

TEST_F(PASCO2SBus_Test, disablePWM_setStatusError)
{
    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.disablePWM());
}

TEST_F(PASCO2SBus_Test, disablePWM_PWMSelectEnableError)
{
    MockGPIO pwmSelect;

    EXPECT_CALL(pwmSelect, disable())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, &pwm, nullptr, nullptr, nullptr, nullptr, nullptr, &pwmSelect);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.disablePWM());
}

TEST_F(PASCO2SBus_Test, disablePWM_PWMSelectEnableSuccess)
{
    MockGPIO pwmSelect;

    EXPECT_CALL(pwmSelect, disable())
    .Times(1)
    .WillRepeatedly(Return(OK));

    MockPASCO2 cotwo(&sbus, &pwm, nullptr, nullptr, nullptr, nullptr, nullptr, &pwmSelect);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.disablePWM());
}

TEST_F(PASCO2SBus_Test, disablePWM_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.disablePWM());
}

TEST_F(PASCO2SBus_Test, disablePWM_SBusSuccess)
{
    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.disablePWM());
}

/**
 * setPWMMode() tests
 */

TEST_F(PASCO2SBus_Test, setPWMMode_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setPWMMode(PWM_SINGLE_PULSE));

    PASCO2 cotwo2(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo2.setPWMMode(PWM_SINGLE_PULSE));
}

TEST_F(PASCO2SBus_Test, setPWMMode_setStatusError)
{
    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.setPWMMode(PWM_SINGLE_PULSE));
}

TEST_F(PASCO2SBus_Test, setPWMMode_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.setPWMMode(PWM_SINGLE_PULSE));
}

TEST_F(PASCO2SBus_Test, setPWMMode_SBusSuccess)
{
    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.setPWMMode(PWM_SINGLE_PULSE));
}

/**
 * setMeasPeriod() tests
 */

TEST_F(PASCO2SBus_Test, setMeasPeriod_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setMeasPeriod(10));
}

TEST_F(PASCO2SBus_Test, setMeasPeriod_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.setMeasPeriod(10));
}

TEST_F(PASCO2SBus_Test, setMeasPeriod_PeriodOutOfRangeError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    ASSERT_EQ(CONFIG_ERROR, cotwo.setMeasPeriod(4));
    ASSERT_EQ(CONFIG_ERROR, cotwo.setMeasPeriod(0x1000));
}

TEST_F(PASCO2SBus_Test, setMeasPeriod_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.setMeasPeriod(0xFFF));
}

TEST_F(PASCO2SBus_Test, setMeasPeriod_SBusReadError)
{
    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.setMeasPeriod(0xAAA));
}

TEST_F(PASCO2SBus_Test, setMeasPeriod_SBusSuccess)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.setMeasPeriod(5));
}

/**
 * getCO2() tests
 */

TEST_F(PASCO2SBus_Test, getCO2_setStatusError)
{
    int16_t  ppm;

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.getCO2(ppm));
}

TEST_F(PASCO2SBus_Test, getCO2_SBusReadError)
{
    int16_t  ppm;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.getCO2(ppm));
}

TEST_F(PASCO2SBus_Test, getCO2_SBusSuccess)
{
    int16_t  ppm;

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    cotwo.writeReg(Reg::REG_ADDR_CO2PPM_H, 0xAB);
    cotwo.writeReg(Reg::REG_ADDR_CO2PPM_L, 0xCD);
    int16_t expectedPpm = 0xABCD;

    ASSERT_EQ(OK, cotwo.getCO2(ppm));
    ASSERT_EQ(expectedPpm, ppm);
}

TEST_F(PASCO2SBus_Test, getCO2_PWMGetPercentageError)
{
    int16_t  ppm;

    EXPECT_CALL(pwm, getDuty(_))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(nullptr, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.getCO2(ppm));
}

TEST_F(PASCO2SBus_Test, getCO2_PWMgetDutySuccess)
{
    int16_t  ppm;

    EXPECT_CALL(pwm, getDuty(_))
    .Times(1)
    .WillOnce(DoAll(SetArgReferee<0>(33), Return(OK)));

    MockPASCO2 cotwo(nullptr, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    int16_t expectedPpm = 3300;

    ASSERT_EQ(OK, cotwo.getCO2(ppm));
    ASSERT_EQ(expectedPpm, ppm);
}

/**
 * getDiagnosis() tests
 */

TEST_F(PASCO2SBus_Test, getDiagnosis_NullSBusInterfaceSuccess)
{
    Diag_t diag;

    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.getDiagnosis(diag));
}

TEST_F(PASCO2SBus_Test, getDiagnosis_setStatusError)
{
    Diag_t diag;

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.getDiagnosis(diag));
}

TEST_F(PASCO2SBus_Test, getDiagnosis_SBusReadError)
{
    Diag_t diag;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.getDiagnosis(diag));
}

TEST_F(PASCO2SBus_Test, getDiagnosis_SBusWriteError)
{
    Diag_t diag;

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.getDiagnosis(diag));
}

TEST_F(PASCO2SBus_Test, getDiagnosis_SBusSuccess)
{
    Diag_t diag;

    MockPASCO2 cotwo(&sbus, nullptr);

    cotwo.writeReg(Reg::REG_ADDR_SENS_STS, 0xAA);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.getDiagnosis(diag));
    ASSERT_TRUE(diag.sensorRdy);
    ASSERT_TRUE(diag.pwmPinEnabled);
    ASSERT_TRUE(diag.outOfRangeTempError);
    ASSERT_FALSE(diag.outOfRange12VError);
    ASSERT_TRUE(diag.commError);
}

/**
 * getMeasStatus() tests
 */

TEST_F(PASCO2SBus_Test, getMeasStatus_NullSBusInterfaceSuccess)
{
    MeasStatus_t measSt;

    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.getMeasStatus(measSt));
}

TEST_F(PASCO2SBus_Test, getMeasStatus_setStatusError)
{
    MeasStatus_t measSt;

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR,cotwo.getMeasStatus(measSt));
}

TEST_F(PASCO2SBus_Test, getMeasStatus_SBusReadError)
{
    MeasStatus_t measSt;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.getMeasStatus(measSt));
}

TEST_F(PASCO2SBus_Test, getMeasStatus_SBusWriteError)
{
    MeasStatus_t measSt;

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.getMeasStatus(measSt));
}

TEST_F(PASCO2SBus_Test, getMeasStatus_SBusSuccess)
{
    MeasStatus_t measSt;

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    cotwo.writeReg(Reg::REG_ADDR_MEAS_STS, 0x1C);

    ASSERT_EQ(OK, cotwo.getMeasStatus(measSt));
    ASSERT_TRUE(measSt.dataRdy);
    ASSERT_TRUE(measSt.intActive);
    ASSERT_TRUE(measSt.alarmActive);
}

/**
 * enableInterrupt() tests
 */

void cback(void *){};

TEST_F(PASCO2SBus_Test, enableInterrupt_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.enableInterrupt(cback, INT_ALARM_ACTIVE, INT_IO_PUSH_PULL_ACTIVE_HIGH));
}

TEST_F(PASCO2SBus_Test, enableInterrupt_setStatusError)
{
    MockGPIO   interrupt;

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.enableInterrupt(cback, INT_ALARM_ACTIVE, INT_IO_PUSH_PULL_ACTIVE_HIGH));
}

TEST_F(PASCO2SBus_Test, enableInterrupt_SBusWriteError)
{
    MockGPIO   interrupt;

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enableInterrupt(cback, INT_ALARM_ACTIVE, INT_IO_PUSH_PULL_ACTIVE_HIGH));
}

TEST_F(PASCO2SBus_Test, enableInterrupt_InterrutEnableError)
{
    MockGPIO   interrupt;

    EXPECT_CALL(interrupt, enableInt(_))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enableInterrupt(cback, INT_ALARM_ACTIVE, INT_IO_PUSH_PULL_ACTIVE_HIGH));
}

TEST_F(PASCO2SBus_Test, enableInterrupt_Success)
{
    MockGPIO   interrupt;

    EXPECT_CALL(interrupt, enableInt(_))
    .Times(1)
    .WillRepeatedly(Return(OK));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.enableInterrupt(cback, INT_ALARM_ACTIVE, INT_IO_PUSH_PULL_ACTIVE_HIGH));
}

TEST_F(PASCO2SBus_Test, enableInterrupt_NoSBusIntDisabledSuccess)
{
    MockGPIO   interrupt;

    MockPASCO2 cotwo(nullptr, &pwm, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.enableInterrupt(cback, INT_DISABLED, INT_IO_PUSH_PULL_ACTIVE_HIGH));
}

/**
 * disableInterrupt() tests
 */

TEST_F(PASCO2SBus_Test, disableInterrupt_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.disableInterrupt());
}

TEST_F(PASCO2SBus_Test, disableInterrupt_setStatusError)
{
    MockGPIO   interrupt;

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.disableInterrupt());
}

TEST_F(PASCO2SBus_Test, disableInterrupt_SBusWriteError)
{
    MockGPIO   interrupt;

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.disableInterrupt());
}

TEST_F(PASCO2SBus_Test, disableInterrupt_GPIOInterrutDisableError)
{
    MockGPIO   interrupt;

    EXPECT_CALL(interrupt, disableInt())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.disableInterrupt());
}

TEST_F(PASCO2SBus_Test, disableInterrupt_Success)
{
    MockGPIO   interrupt;

    EXPECT_CALL(interrupt, disableInt())
    .Times(1)
    .WillRepeatedly(Return(OK));

    MockPASCO2 cotwo(nullptr, &pwm, nullptr, &interrupt);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.disableInterrupt());
}

/**
 * enableABOCompensation() tests
 */

TEST_F(PASCO2SBus_Test, enableABOCompensation_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.enableABOCompensation(ABOC_ENABLED_PERIODIC, 13));
}

TEST_F(PASCO2SBus_Test, enableABOCompensation_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.enableABOCompensation(ABOC_ENABLED_PERIODIC, 13));
}

TEST_F(PASCO2SBus_Test, enableABOCompensation_SBusFirstWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enableABOCompensation(ABOC_ENABLED_PERIODIC, 13));
}

TEST_F(PASCO2SBus_Test, enableABOCompensation_SBusSecondWriteError)
{
    {
        InSequence s;
        EXPECT_CALL(sbus, write(_, _, _))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, write(_, _, _))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enableABOCompensation(ABOC_ENABLED_PERIODIC, 13));
}

TEST_F(PASCO2SBus_Test, enableABOCompensation_Success)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.enableABOCompensation(ABOC_ENABLED_PERIODIC, 13));
}

/**
 * disableABOCompensation() tests
 */

TEST_F(PASCO2SBus_Test, disableABOCompensation_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.disableABOCompensation());
}

TEST_F(PASCO2SBus_Test, disableABOCompensation_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.disableABOCompensation());
}

TEST_F(PASCO2SBus_Test, disableABOCompensation_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.disableABOCompensation());
}

TEST_F(PASCO2SBus_Test,disableABOCompensation_Success)
{

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.disableABOCompensation());
}

/**
 * setPressureCompensation() tests
 */

TEST_F(PASCO2SBus_Test, setPressureCompensation_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setPressureCompensation(0x11));
}

TEST_F(PASCO2SBus_Test, setPressureCompensation_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.setPressureCompensation(0x11));
}

TEST_F(PASCO2SBus_Test, setPressureCompensation_PressCompInvalidRangeError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    ASSERT_EQ(CONFIG_ERROR, cotwo.setPressureCompensation(599));
    ASSERT_EQ(CONFIG_ERROR, cotwo.setPressureCompensation(1601));
}

TEST_F(PASCO2SBus_Test, setPressureCompensation_SBusWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.setPressureCompensation(1600));
}

TEST_F(PASCO2SBus_Test, setPressureCompensation_Success)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.setPressureCompensation(1600));
}

/**
 * enableAlarm() tests
 */

TEST_F(PASCO2SBus_Test, enableAlarm_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.enableAlarm(1, ALARM_TH_CROSS_UP));
}

TEST_F(PASCO2SBus_Test, enableAlarm_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.enableAlarm(1, ALARM_TH_CROSS_UP));
}

TEST_F(PASCO2SBus_Test, enableAlarm_SBusFirstWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enableAlarm(500, ALARM_TH_CROSS_UP));
}

TEST_F(PASCO2SBus_Test, enableAlarm_SBusSecondWriteError)
{
    {
        InSequence s;

        EXPECT_CALL(sbus, write(_, _, _))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, write(_, _, _))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enableAlarm(300, ALARM_TH_CROSS_UP));
}

TEST_F(PASCO2SBus_Test, enableAlarm_Success)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.enableAlarm(300, ALARM_TH_CROSS_UP));
}

/**
 * disableAlarm() tests
 */

TEST_F(PASCO2SBus_Test, disableAlarm_NullSBusInterfaceSuccess)
{
    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.disableAlarm());
}

TEST_F(PASCO2SBus_Test, disableAlarm_setStatusError)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.disableAlarm());
}

TEST_F(PASCO2SBus_Test, disableAlarm_SBusFirstWriteError)
{
    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.disableAlarm());
}

TEST_F(PASCO2SBus_Test, disableAlarm_SBusSecondWriteError)
{
    {
        InSequence s;

        EXPECT_CALL(sbus, write(_, _, _))
        .Times(1)
        .WillRepeatedly(Return(OK));

        EXPECT_CALL(sbus, write(_, _, _))
        .Times(1)
        .WillRepeatedly(Return(INTF_ERROR));
    }
    
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.disableAlarm());
}

TEST_F(PASCO2SBus_Test, disableAlarm_Success)
{
    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.disableAlarm());
}

/**
 * getDeviceID() tests
 */


TEST_F(PASCO2SBus_Test, getDeviceID_NullSBusInterfaceSuccess)
{    
    uint8_t    prodID, revID;

    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.getDeviceID(prodID, revID));
}

TEST_F(PASCO2SBus_Test, getDeviceID_setStatusError)
{
    uint8_t    prodID, revID;

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.getDeviceID(prodID, revID));
}

TEST_F(PASCO2SBus_Test, getDeviceID_SBusWriteError)
{
    uint8_t    prodID, revID;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.getDeviceID(prodID, revID));
}

TEST_F(PASCO2SBus_Test, getDeviceID_Success)
{
    uint8_t     prodID, revID;

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.getDeviceID(prodID, revID));
}
