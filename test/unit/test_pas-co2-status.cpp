#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "pas-co2.hpp"
#include "pas-co2-reg.hpp"
#include "pas-co2-sbus.hpp"
#include "pas-co2-pal-gpio.hpp"
#include "pas-co2-pal-timer.hpp"
#include "pas-co2-pal-pwm.hpp"

using namespace pasco2;

using ::testing::Return;
using ::testing::_;
using ::testing::InSequence;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::SetArgPointee;
using ::testing::SetArrayArgument;
using ::testing::NiceMock;


/** 
 * SBus Mock
 */
class MockSBus : public SBus
{
    public:
        MOCK_METHOD(Error_t, init,   (), (override));
        MOCK_METHOD(Error_t, deinit, (), (override));
        MOCK_METHOD(Error_t, write,  (const uint8_t    addr, const uint8_t * data, uint16_t length), (override));
        MOCK_METHOD(Error_t, read,   (const uint8_t    addr,       uint8_t * data, uint16_t length), (override));
        MOCK_METHOD(SBus::Proto_t, getProtocol, (), (override));
};

/**
 * GPIO Mock
 */
class MockGPIO : public GPIO
{
    public:
        MOCK_METHOD(Error_t, init,        (), (override));
        MOCK_METHOD(Error_t, deinit,      (), (override));
        MOCK_METHOD(Error_t, enableInt,   (void (*cback) (void *)), (override));
        MOCK_METHOD(Error_t, disableInt,  (), (override));
        MOCK_METHOD(IntEvent_t,      intEvent,    (), (override));
        MOCK_METHOD(VLevel_t,        read,        (), (override));
        MOCK_METHOD(Error_t, write,       (VLevel_t level), (override));
        MOCK_METHOD(Error_t, enable,      (), (override));
        MOCK_METHOD(Error_t, disable,     (), (override));
};

/**
 * Timer Mock
 */
class MockTimer: public Timer
{
    public:
        MOCK_METHOD(Error_t, init,    (), (override));
        MOCK_METHOD(Error_t, deinit,  (), (override));
        MOCK_METHOD(Error_t, start,   (), (override));
        MOCK_METHOD(Error_t, stop,    (), (override));
        MOCK_METHOD(Error_t, elapsed, (uint32_t & elapsed), (override));
        MOCK_METHOD(Error_t, delay,   (uint32_t timeout), (override));
};

/**
 * PWM Mock
 */
class MockPWM: public PWM
{
    public:
        MOCK_METHOD(Error_t, init,   (), (override));
        MOCK_METHOD(Error_t, deinit, (), (override));
        MOCK_METHOD(Error_t, getDuty,(double & duty), (override));
};

/**
 * PASCO2 Mock2 (Make Private function Public)
 */
class PASCO2Pub: public PASCO2
{
    public:
        PASCO2Pub(  SBus  * const sbus,
                        PWM   * const pwm,
                        Timer * const timer       = nullptr,
                        GPIO  * const interrupt   = nullptr,
                        GPIO  * const protoSelect = nullptr,
                        GPIO  * const power3V3    = nullptr, 
                        GPIO  * const power12V    = nullptr,
                        GPIO  * const pwmDisable  = nullptr) : 
                        PASCO2(sbus, pwm, timer, interrupt, protoSelect, power3V3, power12V, pwmDisable)
                        {

                        };

        void writeReg(uint8_t addr, uint8_t value) { reg.regMap[addr] = value; };

        Error_t setStatusPub(const Status_t newStatus, bool enoughIfImplicit = false){ return setStatus(newStatus, enoughIfImplicit); };
        Error_t preResetSaveConfigPublic    () { return preResetSaveConfig();     };
        Error_t postResetRestoreConfigPublic() { return postResetRestoreConfig(); };
};


/**
 * Class C++ PASCO2 Class Test Suite
 */
class PASCO2Status_Test: public ::testing::Test
{
    public:
        
        NiceMock<MockPWM> pwm;
        NiceMock<MockGPIO> power3V3;
        NiceMock<MockGPIO> power12V;

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
 * setStatus() tests
 */

TEST_F(PASCO2Status_Test, setStatus_SameStatusSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToInitedError)
{
    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(INITED));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToInitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToLogicOnlyOnError)
{
    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToLogicOnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToIROnlyOnError)
{
    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToIROnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);
    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToOnError)
{
    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(ON));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_UnitedToOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToUnitedError)
{
    EXPECT_CALL(pwm, deinit())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToUnitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToLogicOnlyOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToLogicOnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToIROnlyOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToIROnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(ON));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_InitedToOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyOnToUnitedError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyOnToUnitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyOnToInitedError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(INITED));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyOnToInitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyOnToIROnlyOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyOnToIROnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyToOnError)
{
   PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());

}

TEST_F(PASCO2Status_Test, setStatus_LogicOnlyToOnSuccess)
{
   PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToUnitedError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    EXPECT_CALL(power12V, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToUnitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToInitedError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    EXPECT_CALL(power12V, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(INITED));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToInitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToLogicOnlyOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    EXPECT_CALL(power12V, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToLogicOnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_IROnlyOnToOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_OnToUnitedError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_OnToUnitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(UNINITED));
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_OnToInitedError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(INITED));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_OnToInitedSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(INITED));
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_OnToLogicOnlyOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);

    EXPECT_CALL(power12V, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(ON, cotwo.getStatus());
}


TEST_F(PASCO2Status_Test, setStatus_OnToLogicOnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON));
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_OnToIROnlyOnError)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_OnToIROnlyOnSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, &power3V3);

    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(IR_ONLY_ON));
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2Status_Test, setStatus_ImplicitSuccess)
{
    PASCO2Pub cotwo(nullptr, &pwm);
    
    ASSERT_EQ(OK, cotwo.setStatusPub(UNINITED, true));
    ASSERT_EQ(OK, cotwo.setStatusPub(ON));
    ASSERT_EQ(ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.setStatusPub(LOGIC_ONLY_ON, true));
    ASSERT_EQ(ON, cotwo.getStatus());

}