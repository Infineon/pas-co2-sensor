#include "test_pas-co2-mock.hpp"
#include "pas-co2-serial.hpp"

/**
 * Class C++ PASCO2 Serial Class Test Suite
 */
class PASCO2Serial_Test: public ::testing::Test
{
    public:
        
        NiceMock<MockSBus> sbus;
        NiceMock<MockGPIO> power3V3;
        NiceMock<MockGPIO> power12V;
            
        uint8_t regVal[2] = {0x00, 0x80};

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
        
            ON_CALL(sbus, read(Reg::REG_ADDR_SCRATCH_PAD, _, 1))
            .WillByDefault(DoAll(SetArgPointee<1>(0x00), Return(OK)));

            ON_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _ , _))
            .WillByDefault(DoAll(SetArrayArgument<1>(regVal, regVal + 2), Return(OK)));

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

TEST_F(PASCO2Serial_Test, enable_initError)
{
    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus);

    ASSERT_EQ(INTF_ERROR, cotwo.enable());
}

TEST_F(PASCO2Serial_Test, enable_enableIREmitterError)
{
    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(INTF_ERROR, cotwo.enable());
}

TEST_F(PASCO2Serial_Test, enable_power3V3EnableError)
{
    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(INTF_ERROR, cotwo.enable());
}

TEST_F(PASCO2Serial_Test, enable_Success)
{
    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.enable());
}

/**
 * disable() tests
 */

TEST_F(PASCO2Serial_Test, disable_power12VDisableError)
{
    EXPECT_CALL(power12V, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(INTF_ERROR, cotwo.disable());
}

TEST_F(PASCO2Serial_Test, disable_power3V3DisableError)
{
    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(INTF_ERROR, cotwo.disable());
}

TEST_F(PASCO2Serial_Test, disable_DeinitError)
{
    EXPECT_CALL(sbus, deinit())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr, nullptr, nullptr, &power3V3, &power12V);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(INTF_ERROR, cotwo.disable());
}

TEST_F(PASCO2Serial_Test, disable_Success)
{
    PASCO2Serial cotwo(&sbus, nullptr, nullptr, nullptr, &power3V3, &power12V);

    ASSERT_EQ(OK, cotwo.enable());
    ASSERT_EQ(OK, cotwo.disable());
}

/**
 * startMeasure() tests
 */

TEST_F(PASCO2Serial_Test, startMeasure_disableAlarmError)
{
    EXPECT_CALL(sbus, write(Reg::REG_ADDR_ALARM_TH_H, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure());
}

TEST_F(PASCO2Serial_Test, startMeasure_disableInterruptError)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    {
        InSequence s;

        EXPECT_CALL(sbus, write(Reg::REG_ADDR_ALARM_TH_H, _, _))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, write(Reg::REG_ADDR_INT_CFG, _, _))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, write(Reg::REG_ADDR_INT_CFG, _, _))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    PASCO2Serial cotwo(&sbus, nullptr, &interrupt);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure());
}

TEST_F(PASCO2Serial_Test, startMeasure_setOperationModeError)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, disableInt())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));


    {
        InSequence s;

        EXPECT_CALL(sbus, write(Reg::REG_ADDR_ALARM_TH_H, _, _))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, write(Reg::REG_ADDR_INT_CFG, _, _))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, write(Reg::REG_ADDR_INT_CFG, _, _))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, write(Reg::REG_ADDR_MEAS_CFG, _, _))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    PASCO2Serial cotwo(&sbus, nullptr, &interrupt);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure());
}

TEST_F(PASCO2Serial_Test, startMeasure_WithoutArgumentsSuccess)
{
    PASCO2Serial cotwo(&sbus);

    ASSERT_EQ(OK, cotwo.startMeasure());
}

TEST_F(PASCO2Serial_Test, startMeasure_setMeasPeriodError)
{
    PASCO2Serial cotwo(&sbus);

    ASSERT_EQ(CONFIG_ERROR, cotwo.startMeasure(4));
}

TEST_F(PASCO2Serial_Test, startMeasure_setMeasPeriodSuccess)
{
    PASCO2Serial cotwo(&sbus);

    ASSERT_EQ(OK, cotwo.startMeasure(10));
}

TEST_F(PASCO2Serial_Test, startMeasure_enableAlarmError)
{
    EXPECT_CALL(sbus, write(Reg::REG_ADDR_INT_CFG, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure(0, 200));
}

TEST_F(PASCO2Serial_Test, startMeasure_enableAlarmSuccess)
{
    PASCO2Serial cotwo(&sbus);

    ASSERT_EQ(OK, cotwo.startMeasure(0, 200));
}

static void cBack (void *){ };

TEST_F(PASCO2Serial_Test, startMeasure_enableIntError)
{  
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, enableInt(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr, &interrupt);

    ASSERT_EQ(INTF_ERROR, cotwo.startMeasure(0, 0, cBack));
}

TEST_F(PASCO2Serial_Test, startMeasure_enableIntSuccess)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, enableInt(_))
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Serial cotwo(&sbus, nullptr, &interrupt);

    ASSERT_EQ(OK, cotwo.startMeasure(0, 0, cBack));
}

TEST_F(PASCO2Serial_Test, startMeasure_AllInstancesSuccess)
{
    MockGPIO interrupt;

    EXPECT_CALL(interrupt, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(interrupt, enableInt(_))
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Serial cotwo(&sbus, nullptr, &interrupt);

    ASSERT_EQ(OK, cotwo.startMeasure(10, 300, cBack));
}

/**
 * stopMeasure() tests
 */

TEST_F(PASCO2Serial_Test, stopMeasure_setOperationModeError)
{
    EXPECT_CALL(sbus, write(Reg::REG_ADDR_MEAS_CFG, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.stopMeasure());
}

TEST_F(PASCO2Serial_Test, stopMeasure_Success)
{
    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.stopMeasure());
}

/**
 * getCO2() tests
 */

TEST_F(PASCO2Serial_Test, getCO2_getMeasStatusError)
{
    int16_t  co2ppm = 0;

    {
        InSequence s;


        EXPECT_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _ , _))
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<1>(regVal, regVal + 2), Return(OK)));

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_MEAS_STS, _, _))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.getCO2(co2ppm));
}

TEST_F(PASCO2Serial_Test, getCO2_DataNotReady)
{
    int16_t  co2ppm = 0;

    {
        InSequence s;

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _ , _))
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<1>(regVal, regVal + 2), Return(OK)));

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_MEAS_STS, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(0xEF), Return(OK)));
    }

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.getCO2(co2ppm));
    ASSERT_EQ(0, co2ppm);
}

TEST_F(PASCO2Serial_Test, getCO2_getCO2Error)
{  
    int16_t  co2ppm = 0;

    {
        InSequence s;

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _ , _))
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<1>(regVal, regVal + 2), Return(OK)));

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_MEAS_STS, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(0x10), Return(OK)));

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_CO2PPM_H, _, 2))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.getCO2(co2ppm));
    ASSERT_EQ(-1, co2ppm);
}

TEST_F(PASCO2Serial_Test, getCO2_Success)
{
    int16_t  co2ppm = 0;
    
    {
        InSequence s;
        uint8_t rawco2regs[2] = {0x1B, 0xCD}; 

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _ , _))
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<1>(regVal, regVal + 2), Return(OK)));

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_MEAS_STS, _, _))
        .Times(1)
        .WillOnce(DoAll(SetArgPointee<1>(0x10), Return(OK)));

        EXPECT_CALL(sbus, read(Reg::REG_ADDR_CO2PPM_H, _, 2))
        .Times(1)
        .WillOnce(DoAll(SetArrayArgument<1>(rawco2regs, rawco2regs + 2), Return(OK)));
    }

    PASCO2Serial cotwo(&sbus, nullptr);


    ASSERT_EQ(OK, cotwo.getCO2(co2ppm));
    ASSERT_EQ(0x1BCD, co2ppm);
}

/**
 * getDiagnosis() tests
 */

TEST_F(PASCO2Serial_Test, getDiagnosis_Error)
{
    Diag_t diag;

    EXPECT_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.getDiagnosis(diag));
}

TEST_F(PASCO2Serial_Test, getDiagnosis_Success)
{
    Diag_t diag;

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.getDiagnosis(diag));
}

/**
 * calibrate() tests
 */

TEST_F(PASCO2Serial_Test, calibrate_enableABOCompensationError)
{
    ABOC_t  aboc        = ABOC_ENABLED_PERIODIC;
    int16_t abocRef     = 0;
    uint16_t pressRef   = 0x300;

    EXPECT_CALL(sbus, write(Reg::REG_ADDR_MEAS_CFG, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.calibrate(aboc, abocRef, pressRef));
}

TEST_F(PASCO2Serial_Test, calibrate_setPressureCompensationError)
{
    
    ABOC_t  aboc        = ABOC_ENABLED_PERIODIC;
    int16_t abocRef     = 0;
    uint16_t pressRef   = 500;

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(CONFIG_ERROR, cotwo.calibrate(aboc, abocRef, pressRef));
}

TEST_F(PASCO2Serial_Test, calibrate_Success)
{
    ABOC_t  aboc        = ABOC_ENABLED_PERIODIC;
    int16_t abocRef     = 0;
    uint16_t pressRef   = 0x300;

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.calibrate(aboc, abocRef, pressRef));
}

/**
 * reset() tests
 */

TEST_F(PASCO2Serial_Test, reset_softResetError)
{
    EXPECT_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _ , _))
    .Times(1)
    .WillOnce(DoAll(SetArrayArgument<1>(regVal, regVal + 2), Return(INTF_ERROR)));

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.reset());
}

TEST_F(PASCO2Serial_Test, reset_Success)
{
    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.reset());
}

/**
 * info() tests
 */

TEST_F(PASCO2Serial_Test, getDeviceID_Error)
{
    uint8_t prodID, revID;

    EXPECT_CALL(sbus, read(Reg::REG_ADDR_PROD_ID, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.getDeviceID(prodID, revID));
}

TEST_F(PASCO2Serial_Test, getDeviceID_Success)
{
    uint8_t prodID, revID;

    PASCO2Serial cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.getDeviceID(prodID, revID));
}