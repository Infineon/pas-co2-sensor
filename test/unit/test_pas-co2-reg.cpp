#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "pas-co2-reg.hpp"

using namespace pasco2;
/**
 * Class C++ PASCO2 Reg Class Test Suite
 */
class PASCO2_Reg_Test: public ::testing::Test
{
    public:

        Reg reg;

        void SetUp()
        {
            
        }

        void TearDown()
        {

        }
};

TEST_F(PASCO2_Reg_Test, getProductID)
{
    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0x1F;
    ASSERT_EQ(0x0, reg.getProductID());

    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0xFF;
    ASSERT_EQ(0x7, reg.getProductID());

    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0x50;
    ASSERT_EQ(0x2,reg.getProductID());

    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0x30;
    ASSERT_EQ(0x1,reg.getProductID()); 
}

TEST_F(PASCO2_Reg_Test, getRevisionID)
{
    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0xFF;
    ASSERT_EQ(0x1F, reg.getRevisionID());

    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0xE0;
    ASSERT_EQ(0x00, reg.getRevisionID());

    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0xF0;
    ASSERT_EQ(0x10,reg.getRevisionID());

    reg.regMap[Reg::REG_ADDR_PROD_ID]  = 0x0A;
    ASSERT_EQ(0x0A,reg.getRevisionID()); 
}

TEST_F(PASCO2_Reg_Test, isSensorReady)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFF;
    ASSERT_EQ(true, reg.isSensorReady());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x7F;
    ASSERT_EQ(false, reg.isSensorReady());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x80;
    ASSERT_EQ(true, reg.isSensorReady());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    ASSERT_EQ(false, reg.isSensorReady()); 
}

TEST_F(PASCO2_Reg_Test, isPWMPinEnabled)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xBF;
    ASSERT_EQ(true, reg.isPWMPinEnabled());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFF;
    ASSERT_EQ(false, reg.isPWMPinEnabled());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    ASSERT_EQ(true, reg.isPWMPinEnabled());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x40;
    ASSERT_EQ(false, reg.isPWMPinEnabled()); 
}

TEST_F(PASCO2_Reg_Test, isTempOutofRange)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFF;
    ASSERT_EQ(true, reg.isTempOutofRange());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xDF;
    ASSERT_EQ(false, reg.isTempOutofRange());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x20;
    ASSERT_EQ(true, reg.isTempOutofRange());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    ASSERT_EQ(false, reg.isTempOutofRange()); 
}

TEST_F(PASCO2_Reg_Test, clearTempOutofRangeFlag)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    reg.clearTempOutofRangeFlag();
    ASSERT_EQ(0x04, reg.regMap[Reg::REG_ADDR_SENS_STS]);

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFB;
    reg.clearTempOutofRangeFlag();
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_SENS_STS]);
}

TEST_F(PASCO2_Reg_Test, is12VOutofRange)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFF;
    ASSERT_EQ(true, reg.is12VOutofRange());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xEF;
    ASSERT_EQ(false, reg.is12VOutofRange());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x10;
    ASSERT_EQ(true, reg.is12VOutofRange());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    ASSERT_EQ(false, reg.is12VOutofRange()); 
}

TEST_F(PASCO2_Reg_Test, clear12VOutofRangeFlag)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    reg.clear12VOutofRangeFlag();
    ASSERT_EQ(0x02, reg.regMap[Reg::REG_ADDR_SENS_STS]);

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFD;
    reg.clear12VOutofRangeFlag();
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_SENS_STS]);
}

TEST_F(PASCO2_Reg_Test, hasCommErrorOccurred)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFF;
    ASSERT_EQ(true, reg.hasCommErrorOccurred());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xF7;
    ASSERT_EQ(false, reg.hasCommErrorOccurred());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x08;
    ASSERT_EQ(true, reg.hasCommErrorOccurred());

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    ASSERT_EQ(false, reg.hasCommErrorOccurred()); 
}

TEST_F(PASCO2_Reg_Test, clearCommErrorFlag)
{
    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0x00;
    reg.clearCommErrorFlag();
    ASSERT_EQ(0x01, reg.regMap[Reg::REG_ADDR_SENS_STS]);

    reg.regMap[Reg::REG_ADDR_SENS_STS]  = 0xFE;
    reg.clearCommErrorFlag();
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_SENS_STS]);
}

TEST_F(PASCO2_Reg_Test, getMeasRate)
{
    reg.regMap[Reg::REG_ADDR_MEAS_RATE_H]  = 0xFF;
    reg.regMap[Reg::REG_ADDR_MEAS_RATE_L]  = 0xFF;
    ASSERT_EQ(0x0FFF, reg.getMeasRate());

    reg.regMap[Reg::REG_ADDR_MEAS_RATE_H]  = 0xF5;
    reg.regMap[Reg::REG_ADDR_MEAS_RATE_L]  = 0xAA;
    ASSERT_EQ(0x05AA, reg.getMeasRate());
}

TEST_F(PASCO2_Reg_Test, setMeasRate)
{
    reg.regMap[Reg::REG_ADDR_MEAS_RATE_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_MEAS_RATE_L]  = 0x00;
    reg.setMeasRate(0xFFFF);
    ASSERT_EQ(0x0F, reg.regMap[Reg::REG_ADDR_MEAS_RATE_H]);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_MEAS_RATE_L]);

    reg.regMap[Reg::REG_ADDR_MEAS_RATE_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_MEAS_RATE_L]  = 0x00;
    reg.setMeasRate(0xF5BB);
    ASSERT_EQ(0x05, reg.regMap[Reg::REG_ADDR_MEAS_RATE_H]);
    ASSERT_EQ(0xBB, reg.regMap[Reg::REG_ADDR_MEAS_RATE_L]);
}

TEST_F(PASCO2_Reg_Test, enablePWMOutput)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    reg.enablePWMOutput();
    ASSERT_EQ(0x20, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xDF;
    reg.enablePWMOutput();
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);
}

TEST_F(PASCO2_Reg_Test, disablePWMOutput)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFF;
    reg.disablePWMOutput();
    ASSERT_EQ(0xDF, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x20;
    reg.disablePWMOutput();
    ASSERT_EQ(0x00, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);
}

TEST_F(PASCO2_Reg_Test, isPWMOutputEnabled)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFF;
    ASSERT_EQ(true, reg.isPWMOutputEnabled());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xDF;
    ASSERT_EQ(false, reg.isPWMOutputEnabled());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x20;
    ASSERT_EQ(true, reg.isPWMOutputEnabled());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    ASSERT_EQ(false, reg.isPWMOutputEnabled());
}

TEST_F(PASCO2_Reg_Test, getPWMMode)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    ASSERT_EQ(PWM_SINGLE_PULSE, reg.getPWMMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xEF;
    ASSERT_EQ(PWM_SINGLE_PULSE, reg.getPWMMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x10;
    ASSERT_EQ(PWM_TRAIN_PULSE, reg.getPWMMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFF;
    ASSERT_EQ(PWM_TRAIN_PULSE, reg.getPWMMode());
}

TEST_F(PASCO2_Reg_Test, setPWMMode)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFF;
    reg.setPWMMode(PWM_SINGLE_PULSE);
    ASSERT_EQ(0xEF, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x10;
    reg.setPWMMode(PWM_SINGLE_PULSE);
    ASSERT_EQ(0x00, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    reg.setPWMMode(PWM_TRAIN_PULSE);
    ASSERT_EQ(0x10, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xEF;
    reg.setPWMMode(PWM_TRAIN_PULSE);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);
}

TEST_F(PASCO2_Reg_Test, getABOCMode)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    ASSERT_EQ(ABOC_DISABLED, reg.getABOCMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xF3;
    ASSERT_EQ(ABOC_DISABLED, reg.getABOCMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x04;
    ASSERT_EQ(ABOC_ENABLED_PERIODIC, reg.getABOCMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xF7;
    ASSERT_EQ(ABOC_ENABLED_PERIODIC, reg.getABOCMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x08;
    ASSERT_EQ(ABOC_ENABLED_FORCED, reg.getABOCMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFB;
    ASSERT_EQ(ABOC_ENABLED_FORCED, reg.getABOCMode());
}

TEST_F(PASCO2_Reg_Test, setABOCMode)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFF;
    reg.setABOCMode(ABOC_DISABLED);
    ASSERT_EQ(0xF3, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    reg.setABOCMode(ABOC_DISABLED);
    ASSERT_EQ(0x00, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFB;
    reg.setABOCMode(ABOC_ENABLED_PERIODIC);
    ASSERT_EQ(0xF7, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    reg.setABOCMode(ABOC_ENABLED_PERIODIC);
    ASSERT_EQ(0x04, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xF7;
    reg.setABOCMode(ABOC_ENABLED_FORCED);
    ASSERT_EQ(0xFB, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    reg.setABOCMode(ABOC_ENABLED_FORCED);
    ASSERT_EQ(0x08, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);
}

TEST_F(PASCO2_Reg_Test, getOperationMode)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    ASSERT_EQ(MODE_IDLE, reg.getOperationMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFC;
    ASSERT_EQ(MODE_IDLE, reg.getOperationMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x01;
    ASSERT_EQ(MODE_SINGLE_SHOT, reg.getOperationMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFD;
    ASSERT_EQ(MODE_SINGLE_SHOT, reg.getOperationMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x02;
    ASSERT_EQ(MODE_CONTINUOUS_MEAS, reg.getOperationMode());

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFE;
    ASSERT_EQ(MODE_CONTINUOUS_MEAS, reg.getOperationMode());
}

TEST_F(PASCO2_Reg_Test, setOperationMode)
{
    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x03;
    reg.setOperationMode(MODE_IDLE);
    ASSERT_EQ(0x00, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFF;
    reg.setOperationMode(MODE_IDLE);
    ASSERT_EQ(0xFC, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    reg.setOperationMode(MODE_SINGLE_SHOT);
    ASSERT_EQ(0x01, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFE;
    reg.setOperationMode(MODE_SINGLE_SHOT);
    ASSERT_EQ(0xFD, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0x00;
    reg.setOperationMode(MODE_CONTINUOUS_MEAS);
    ASSERT_EQ(0x02, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);

    reg.regMap[Reg::REG_ADDR_MEAS_CFG]  = 0xFD;
    reg.setOperationMode(MODE_CONTINUOUS_MEAS);
    ASSERT_EQ(0xFE, reg.regMap[Reg::REG_ADDR_MEAS_CFG]);
}

TEST_F(PASCO2_Reg_Test, getCO2PPM)
{
    reg.regMap[Reg::REG_ADDR_CO2PPM_H]  = 0xAB;
    reg.regMap[Reg::REG_ADDR_CO2PPM_L]  = 0xCD;
    ASSERT_EQ(0xABCD, reg.getCO2PPM());

    reg.regMap[Reg::REG_ADDR_CO2PPM_H]  = 0xFF;
    reg.regMap[Reg::REG_ADDR_CO2PPM_L]  = 0xFF;
    ASSERT_EQ(0xFFFF, reg.getCO2PPM());
}

TEST_F(PASCO2_Reg_Test, isDataReady)
{
    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xFF;
    ASSERT_EQ(true, reg.isDataReady());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xEF;
    ASSERT_EQ(false, reg.isDataReady());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x10;
    ASSERT_EQ(true, reg.isDataReady());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x00;
    ASSERT_EQ(false, reg.isDataReady());
}

TEST_F(PASCO2_Reg_Test, hasInterruptOcurred)
{
    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xFF;
    ASSERT_EQ(true, reg.hasInterruptOcurred());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xF7;
    ASSERT_EQ(false, reg.hasInterruptOcurred());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x08;
    ASSERT_EQ(true, reg.hasInterruptOcurred());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x00;
    ASSERT_EQ(false, reg.hasInterruptOcurred());
}

TEST_F(PASCO2_Reg_Test, clearInterruptFlag)
{
    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x00;
    reg.clearInterruptFlag();
    ASSERT_EQ(0x02, reg.regMap[Reg::REG_ADDR_MEAS_STS]);

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xFD;
    reg.clearInterruptFlag();
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_MEAS_STS]);
}

TEST_F(PASCO2_Reg_Test, isAlarmSet)
{
    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xFF;
    ASSERT_EQ(true, reg.isAlarmSet());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xFB;
    ASSERT_EQ(false, reg.isAlarmSet());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x04;
    ASSERT_EQ(true, reg.isAlarmSet());

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x00;
    ASSERT_EQ(false, reg.isAlarmSet());
}

TEST_F(PASCO2_Reg_Test, clearAlarmFlag)
{
    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0x00;
    reg.clearAlarmFlag();
    ASSERT_EQ(0x01, reg.regMap[Reg::REG_ADDR_MEAS_STS]);

    reg.regMap[Reg::REG_ADDR_MEAS_STS]  = 0xFE;
    reg.clearAlarmFlag();
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_MEAS_STS]);
}

TEST_F(PASCO2_Reg_Test, getInterruptIOConf)
{
    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFF;
    ASSERT_EQ(INT_IO_PUSH_PULL_ACTIVE_HIGH, reg.getInterruptIOConf());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xEF;
    ASSERT_EQ(INT_IO_PUSH_PULL_ACTIVE_LOW, reg.getInterruptIOConf());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x10;
    ASSERT_EQ(INT_IO_PUSH_PULL_ACTIVE_HIGH, reg.getInterruptIOConf());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    ASSERT_EQ(INT_IO_PUSH_PULL_ACTIVE_LOW, reg.getInterruptIOConf());
}

TEST_F(PASCO2_Reg_Test, setInterruptIOConf)
{
    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setInterruptIOConf(INT_IO_PUSH_PULL_ACTIVE_HIGH);
    ASSERT_EQ(0x10, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xEF;
    reg.setInterruptIOConf(INT_IO_PUSH_PULL_ACTIVE_HIGH);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x10;
    reg.setInterruptIOConf(INT_IO_PUSH_PULL_ACTIVE_LOW);
    ASSERT_EQ(0x00, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFF;
    reg.setInterruptIOConf(INT_IO_PUSH_PULL_ACTIVE_LOW);
    ASSERT_EQ(0xEF, reg.regMap[Reg::REG_ADDR_INT_CFG]);
}

TEST_F(PASCO2_Reg_Test, getInterruptFunc)
{
    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xF1;
    ASSERT_EQ(INT_DISABLED, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    ASSERT_EQ(INT_DISABLED, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xF3;
    ASSERT_EQ(INT_ALARM_ACTIVE, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x02;
    ASSERT_EQ(INT_ALARM_ACTIVE, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xF5;
    ASSERT_EQ(INT_DATA_READY, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x04;
    ASSERT_EQ(INT_DATA_READY, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xF7;
    ASSERT_EQ(INT_SERIAL_BUS_BUSY, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x06;
    ASSERT_EQ(INT_SERIAL_BUS_BUSY, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xF9;
    ASSERT_EQ(INT_EARLY_MEAS, reg.getInterruptFunc());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x08;
    ASSERT_EQ(INT_EARLY_MEAS, reg.getInterruptFunc());
}

TEST_F(PASCO2_Reg_Test, setInterrutFunc)
{
    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFF;
    reg.setInterrutFunc(INT_DISABLED);
    ASSERT_EQ(0xF1, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setInterrutFunc(INT_DISABLED);
    ASSERT_EQ(0x00, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFD;
    reg.setInterrutFunc(INT_ALARM_ACTIVE);
    ASSERT_EQ(0xF3, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setInterrutFunc(INT_ALARM_ACTIVE);
    ASSERT_EQ(0x02, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFB;
    reg.setInterrutFunc(INT_DATA_READY);
    ASSERT_EQ(0xF5, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setInterrutFunc(INT_DATA_READY);
    ASSERT_EQ(0x04, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xF9;
    reg.setInterrutFunc(INT_SERIAL_BUS_BUSY);
    ASSERT_EQ(0xF7, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setInterrutFunc(INT_SERIAL_BUS_BUSY);
    ASSERT_EQ(0x06, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xF7;
    reg.setInterrutFunc(INT_EARLY_MEAS);
    ASSERT_EQ(0xF9, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setInterrutFunc(INT_EARLY_MEAS);
    ASSERT_EQ(0x08, reg.regMap[Reg::REG_ADDR_INT_CFG]);
}

TEST_F(PASCO2_Reg_Test, getAlarmType)
{
    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFE;
    ASSERT_EQ(ALARM_TH_CROSS_DOWN, reg.getAlarmType());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    ASSERT_EQ(ALARM_TH_CROSS_DOWN, reg.getAlarmType());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFF;
    ASSERT_EQ(ALARM_TH_CROSS_UP, reg.getAlarmType());

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x01;
    ASSERT_EQ(ALARM_TH_CROSS_UP, reg.getAlarmType());
}

TEST_F(PASCO2_Reg_Test, setAlarmType)
{
    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFF;
    reg.setAlarmType(ALARM_TH_CROSS_DOWN);
    ASSERT_EQ(0xFE, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setAlarmType(ALARM_TH_CROSS_DOWN);
    ASSERT_EQ(0x00, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0xFE;
    reg.setAlarmType(ALARM_TH_CROSS_UP);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_INT_CFG]);

    reg.regMap[Reg::REG_ADDR_INT_CFG]  = 0x00;
    reg.setAlarmType(ALARM_TH_CROSS_UP);
    ASSERT_EQ(0x01, reg.regMap[Reg::REG_ADDR_INT_CFG]);
}

TEST_F(PASCO2_Reg_Test, getAlarmThreshold)
{
    reg.regMap[Reg::REG_ADDR_ALARM_TH_H]  = 0xAB;
    reg.regMap[Reg::REG_ADDR_ALARM_TH_L]  = 0xCD;
    int16_t result = 0xABCD;
    ASSERT_EQ(result, reg.getAlarmThreshold());

    reg.regMap[Reg::REG_ADDR_ALARM_TH_H]  = 0xFF;
    reg.regMap[Reg::REG_ADDR_ALARM_TH_L]  = 0xFF;
    result = 0xFFFF;
    ASSERT_EQ(result, reg.getAlarmThreshold());
}

TEST_F(PASCO2_Reg_Test, setAlarmThreshold)
{
    reg.regMap[Reg::REG_ADDR_ALARM_TH_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_ALARM_TH_L]  = 0x00;
    reg.setAlarmThreshold(0xFFFF);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_ALARM_TH_H]);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_ALARM_TH_L]);

    reg.regMap[Reg::REG_ADDR_ALARM_TH_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_ALARM_TH_L]  = 0x00;
    reg.setAlarmThreshold(0xDCBA);
    ASSERT_EQ(0xDC, reg.regMap[Reg::REG_ADDR_ALARM_TH_H]);
    ASSERT_EQ(0xBA, reg.regMap[Reg::REG_ADDR_ALARM_TH_L]);
}

TEST_F(PASCO2_Reg_Test, getPressCompReference)
{
    reg.regMap[Reg::REG_ADDR_PRESS_REF_H]  = 0xAB;
    reg.regMap[Reg::REG_ADDR_PRESS_REF_L]  = 0xCD;
    uint16_t result = 0xABCD;
    ASSERT_EQ(result, reg.getPressCompReference());

    reg.regMap[Reg::REG_ADDR_PRESS_REF_H]  = 0xFF;
    reg.regMap[Reg::REG_ADDR_PRESS_REF_L]  = 0xFF;
    result = 0xFFFF;
    ASSERT_EQ(result, reg.getPressCompReference());
}

TEST_F(PASCO2_Reg_Test, setPressCompReference)
{
    reg.regMap[Reg::REG_ADDR_PRESS_REF_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_PRESS_REF_L]  = 0x00;
    reg.setPressCompReference(0xFFFF);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_PRESS_REF_H]);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_PRESS_REF_L]);

    reg.regMap[Reg::REG_ADDR_PRESS_REF_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_PRESS_REF_L]  = 0x00;
    reg.setPressCompReference(0xDCBA);
    ASSERT_EQ(0xDC, reg.regMap[Reg::REG_ADDR_PRESS_REF_H]);
    ASSERT_EQ(0xBA, reg.regMap[Reg::REG_ADDR_PRESS_REF_L]);
}

TEST_F(PASCO2_Reg_Test, getABOCReference)
{
    reg.regMap[Reg::REG_ADDR_CALIB_REF_H]  = 0xAB;
    reg.regMap[Reg::REG_ADDR_CALIB_REF_L]  = 0xCD;
    int16_t result = 0xABCD;
    ASSERT_EQ(result, reg.getABOCReference());

    reg.regMap[Reg::REG_ADDR_CALIB_REF_H]  = 0xFF;
    reg.regMap[Reg::REG_ADDR_CALIB_REF_L]  = 0xFF;
    result = 0xFFFF;
    ASSERT_EQ(result, reg.getABOCReference());
}

TEST_F(PASCO2_Reg_Test, setABOCReference)
{
    reg.regMap[Reg::REG_ADDR_CALIB_REF_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_CALIB_REF_L]  = 0x00;
    reg.setABOCReference(0xFFFF);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_CALIB_REF_H]);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_CALIB_REF_L]);

    reg.regMap[Reg::REG_ADDR_CALIB_REF_H]  = 0x00;
    reg.regMap[Reg::REG_ADDR_CALIB_REF_L]  = 0x00;
    reg.setABOCReference(0xDCBA);
    ASSERT_EQ(0xDC, reg.regMap[Reg::REG_ADDR_CALIB_REF_H]);
    ASSERT_EQ(0xBA, reg.regMap[Reg::REG_ADDR_CALIB_REF_L]);
}

TEST_F(PASCO2_Reg_Test, getScratchPad)
{
    reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]  = 0xAA;
    ASSERT_EQ(0xAA, reg.getScratchPad());

    reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]  = 0xFF;
    ASSERT_EQ(0xFF, reg.getScratchPad());
}

TEST_F(PASCO2_Reg_Test, setStratchPad)
{
    reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]  = 0x00;
    reg.setStratchPad(0xAA);
    ASSERT_EQ(0xAA, reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]);

    reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]  = 0x00;
    reg.setStratchPad(0xFF);
    ASSERT_EQ(0xFF, reg.regMap[Reg::REG_ADDR_SCRATCH_PAD]);
}

TEST_F(PASCO2_Reg_Test, setSoftReset)
{
    reg.regMap[Reg::REG_ADDR_SENS_RST]  = 0x00;
    reg.setSoftReset();
    ASSERT_EQ(0xA3, reg.regMap[Reg::REG_ADDR_SENS_RST]);

}