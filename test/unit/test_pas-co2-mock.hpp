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
        MOCK_METHOD(Error_t, init,          (), (override));
        MOCK_METHOD(Error_t, deinit,        (), (override));
        MOCK_METHOD(Error_t, start,         (), (override));
        MOCK_METHOD(Error_t, stop,          (), (override));
        MOCK_METHOD(Error_t, elapsed,       (uint32_t & elapsed), (override));
        MOCK_METHOD(Error_t, elapsedMicro,  (uint32_t & elapsed), (override));
        MOCK_METHOD(Error_t, delay,         (uint32_t timeout), (override));
};

/**
 * PWM Mock
 */
class MockPWM: public PWM
{
    public:
        MOCK_METHOD(Error_t, init,    (), (override));
        MOCK_METHOD(Error_t, deinit,  (), (override));
        MOCK_METHOD(Error_t, getDuty, (double & co2ppm), (override));
};

/**
 * PASCO2 Mock (Mock Private Class functions)
 */
class MockPASCO2: public PASCO2
{
    public:
        MockPASCO2( SBus  * const sbus,
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

        MOCK_METHOD(Error_t, setStatus,              (const Status_t newStatus, bool enoughIfImplicit),  (override));
        MOCK_METHOD(Error_t, preResetSaveConfig,     (),                                                 (override));
        MOCK_METHOD(Error_t, postResetRestoreConfig, (),                                                 (override));
};

/**
 * PASCO2 Public( Private function Public)
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

        Error_t preResetSaveConfigPublic    () { return preResetSaveConfig();     };
        Error_t postResetRestoreConfigPublic() { return postResetRestoreConfig(); };
};