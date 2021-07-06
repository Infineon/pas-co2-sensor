#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "pas-co2-i2c.hpp"
#include "pas-co2-pal-i2c.hpp"

using namespace pasco2;

using ::testing::Return;
using ::testing::_;

class MockI2CPAL : public I2CPAL
{
    public:
        MOCK_METHOD(Error_t, init,          (), (override));
        MOCK_METHOD(Error_t, deinit,        (), (override));
        MOCK_METHOD(Error_t, setClockFreq,  (const uint32_t clockHz), (override));
        MOCK_METHOD(Error_t, write,         (const uint8_t slaveAddr, const uint8_t memAddr, const uint8_t * data, uint16_t & length), (override));
        MOCK_METHOD(Error_t, read,          (const uint8_t slaveAddr, const uint8_t memAddr,       uint8_t * data, uint16_t & length), (override));
};

/**
 * Class C++ PASCO2 I2C Class Test Suite
 */
class PASCO2I2C_Test: public ::testing::Test
{
    public:

        void SetUp()
        {
            
        }

        void TearDown()
        {

        }
};

/**
 * getProtocol() tests
 */

TEST_F(PASCO2I2C_Test, getProtocol)
{
    MockI2CPAL i2cpal;

    I2C i2c(&i2cpal);

    ASSERT_EQ(SBus::BUS_PROTO_I2C, i2c.getProtocol());
}

/**
 * init() tests
 */

TEST_F(PASCO2I2C_Test, init_i2cpalInitError)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    I2C i2c(&i2cpal);

    ASSERT_EQ(INTF_ERROR, i2c.init());
}

TEST_F(PASCO2I2C_Test, init_setClockFreqError)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(i2cpal, setClockFreq(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    I2C i2c(&i2cpal);

    ASSERT_EQ(INTF_ERROR, i2c.init());
}

TEST_F(PASCO2I2C_Test, init_Success)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(i2cpal, setClockFreq(_))
    .Times(1)
    .WillOnce(Return(OK));

    I2C i2c(&i2cpal);

    ASSERT_EQ(OK, i2c.init());
}

/**
 * deinit() tests
 */

TEST_F(PASCO2I2C_Test, deinit_i2cpalDeinitError)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    I2C i2c(&i2cpal);

    ASSERT_EQ(INTF_ERROR, i2c.deinit());
}

TEST_F(PASCO2I2C_Test, deinit_Success)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    I2C i2c(&i2cpal);

    ASSERT_EQ(OK, i2c.deinit());
}

/**
 * write() tests
 */

uint8_t data[3] = {0,2,3};

TEST_F(PASCO2I2C_Test, write_i2cpalWriteError)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, write(_, _, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    I2C i2c(&i2cpal);

    ASSERT_EQ(INTF_ERROR, i2c.write(0x01, data, 3));
}

TEST_F(PASCO2I2C_Test, write_Success)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, write(_, _, _, _))
    .Times(1)
    .WillOnce(Return(OK));

    I2C i2c(&i2cpal);

    ASSERT_EQ(OK, i2c.write(0x01, data, 3));
}

/**
 * read() tests
 */

TEST_F(PASCO2I2C_Test, read_i2cpalWriteError)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, read(_, _, _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    I2C i2c(&i2cpal);

    ASSERT_EQ(INTF_ERROR, i2c.read(0x01, data, 3));
}

TEST_F(PASCO2I2C_Test, read_Success)
{
    MockI2CPAL i2cpal;

    EXPECT_CALL(i2cpal, read(_, _, _, _))
    .Times(1)
    .WillOnce(Return(OK));

    I2C i2c(&i2cpal);

    ASSERT_EQ(OK, i2c.read(0x01, data, 3));
}

