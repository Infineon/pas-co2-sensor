#include "test_pas-co2-mock.hpp"
#include "pas-co2-pwm-sw.hpp"

using namespace pasco2;

/**
 * PASCO2 PWMSW Public( Private function Public)
 */
class PWMSWPub: public PWMSW
{
    public:
        PWMSWPub            (GPIO  * const pwmIn, 
                             Timer * const timer,
                             Mode_t        mode) :
                             PWMSW(pwmIn, timer, mode) {};

        void                callbackPub()
                            {
                                callback();
                            };

        static void         int0HandlerPub ()
                            {
                                int0Handler();
                            };

        static void         int1HandlerPub ()
                            {
                                int1Handler();
                            };

        static void         int2HandlerPub ()
                            {
                                int2Handler();
                            };

        static void         int3HandlerPub ()
                            {
                                int3Handler();
                            };

        static void       * isrRegisterPub (PWMSW *objPtr)
                            {
                                return isrRegister(objPtr);
                            }
};

/**
 * Class C++ PASCO2 PWM Test Suite
 */
class PASCO2PWMSW_Test: public ::testing::Test
{
    public:
       
        NiceMock<MockGPIO> pwmIn;
        NiceMock<MockTimer> timer;

        void SetUp()
        {
            ON_CALL(pwmIn, init())
            .WillByDefault(Return(OK));

            ON_CALL(pwmIn, deinit())
            .WillByDefault(Return(OK));

            ON_CALL(pwmIn, enableInt(_))
            .WillByDefault(Return(OK));

            ON_CALL(timer, init())
            .WillByDefault(Return(OK));

            ON_CALL(timer, deinit())
            .WillByDefault(Return(OK));

            ON_CALL(timer, start())
            .WillByDefault(Return(OK));

            ON_CALL(timer, stop())
            .WillByDefault(Return(OK));
        }

        void TearDown()
        {

        }
};

/**
 * init() tests
 */

TEST_F(PASCO2PWMSW_Test, init_pwmInInitError)
{
    EXPECT_CALL(pwmIn, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.init());
}

TEST_F(PASCO2PWMSW_Test, init_timerInitError)
{
    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.init());
}

TEST_F(PASCO2PWMSW_Test, init_Success)
{
    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(OK, pwm.init());
}

/**
 * deinit() tests
 */

TEST_F(PASCO2PWMSW_Test, deinit_pwmInInitError)
{
    EXPECT_CALL(pwmIn, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.deinit());
}

TEST_F(PASCO2PWMSW_Test, deinit_timerInitError)
{
    EXPECT_CALL(timer, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.deinit());
}

TEST_F(PASCO2PWMSW_Test, deinit_Success)
{
    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(OK, pwm.deinit());
}

/**
 * intXHandler() tests
 */

TEST_F(PASCO2PWMSW_Test, intXHandler_Success)
{
    /* No assertion, intHandlers must not return fault exceptions */
    PWMSWPub pwm1(&pwmIn, &timer, PWMSW::Mode_t::MODE_INTERRUPT);
    PWMSWPub pwm2(&pwmIn, &timer, PWMSW::Mode_t::MODE_INTERRUPT);
    PWMSWPub pwm3(&pwmIn, &timer, PWMSW::Mode_t::MODE_INTERRUPT);
    PWMSWPub pwm4(&pwmIn, &timer, PWMSW::Mode_t::MODE_INTERRUPT);

    ASSERT_NE(nullptr, PWMSWPub::isrRegisterPub(&pwm1));
    ASSERT_NE(nullptr, PWMSWPub::isrRegisterPub(&pwm2));
    ASSERT_NE(nullptr, PWMSWPub::isrRegisterPub(&pwm3));
    ASSERT_NE(nullptr, PWMSWPub::isrRegisterPub(&pwm4));

    PWMSWPub::int0HandlerPub();
    PWMSWPub::int1HandlerPub();
    PWMSWPub::int2HandlerPub();
    PWMSWPub::int3HandlerPub();
}

/**
 * getDuty() tests
 */

TEST_F(PASCO2PWMSW_Test, getDuty_enableIntError)
{
    double duty = 0;
    
    EXPECT_CALL(pwmIn, enableInt(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_INTERRUPT);

    ASSERT_EQ(INTF_ERROR, pwm.getDuty(duty));
    ASSERT_EQ(-1 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_timerStartError)
{
    double duty = 0;
    
    /* Start timer */
    EXPECT_CALL(timer, start())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_INTERRUPT);

    ASSERT_EQ(INTF_ERROR, pwm.getDuty(duty));
    ASSERT_EQ(-1 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_timerElapsedTimeoutStartError)
{
    double duty = 0;
    
    /* Start timer */
    EXPECT_CALL(timer, elapsedMicro(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.getDuty(duty));
    ASSERT_EQ(-1 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_timerElapsedTimeoutUpdateError)
{
    double duty = 0;

    {
        InSequence s;
    
        /* Start timer */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(Return(OK));

        /* Timeout counter update */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_INTERRUPT);

    ASSERT_EQ(INTF_ERROR, pwm.getDuty(duty));
    ASSERT_EQ(-1 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_timeoutExpiredSuccess)
{
    double duty = 0;

    {
        InSequence s;

        /* Start timer */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(0), Return(OK)));

        /* Timeout counter update */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(3000001), Return(OK)));
    }

    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(OK, pwm.getDuty(duty));
    ASSERT_EQ(0 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_risingEdgeElapsedError)
{
    double duty = 0;

    EXPECT_CALL(pwmIn, read())
    .Times(1)
    .WillOnce(Return(GPIO::VLevel_t::GPIO_HIGH));

    {
        InSequence s;

        /* Start timer */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(Return(OK));

        /* Rise event timestamp (t1)*/
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }
     
    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.getDuty(duty));
    ASSERT_EQ(-1 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_fallingEdgeElapsedError)
{
    double duty = 0;

    {
        InSequence s1;

       /* Start timer */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(0), Return(OK)));

        /* @note: From previous test poll() level is already HIGH */

        /* Falling event timestamp (t2) */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    {
        InSequence s2;

        // EXPECT_CALL(pwmIn, read())
        // .Times(1)
        // .WillOnce(Return(GPIO::VLevel_t::GPIO_HIGH));

        EXPECT_CALL(pwmIn, read())
        .Times(1)
        .WillOnce(Return(GPIO::VLevel_t::GPIO_LOW));
    
    }
     
    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.getDuty(duty));
    ASSERT_EQ(-1 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_StopTimerError)
{
    double duty = 0;

    {
        InSequence s1;

       /* Start timer */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(0), Return(OK)));

        /* Rise event timestamp (t1)*/
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(50), Return(OK)));

        /* Timeout counter update */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(80), Return(OK)));

        /* Falling event timestamp (t2) */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(100), Return(OK)));

        /* Timeout counter update */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(3000001), Return(OK)));

        EXPECT_CALL(timer, stop())
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    {
        InSequence s2;

        EXPECT_CALL(pwmIn, read())
        .Times(1)
        .WillOnce(Return(GPIO::VLevel_t::GPIO_HIGH));

        EXPECT_CALL(pwmIn, read())
        .Times(1)
        .WillOnce(Return(GPIO::VLevel_t::GPIO_LOW));
    
    }
     
    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(INTF_ERROR, pwm.getDuty(duty));
    ASSERT_EQ(-1 , duty);
}

TEST_F(PASCO2PWMSW_Test, getDuty_OnPulseSuccess)
{
    double duty = 0;

    {
        InSequence s1;

        /* Start timer */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(0), Return(OK)));

        /* Rise event timestamp (t1)*/
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(1000), Return(OK)));
    
        /* Timeout counter update */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(1100), Return(OK)));

        /* Falling event timestamp (t2) */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(8500), Return(OK)));

        /* Timeout counter update */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(9000), Return(OK)));

        /* Rise event timestamp (t3) */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(13500), Return(OK)));

        /* Timeout counter update */
        EXPECT_CALL(timer, elapsedMicro(_))
        .Times(1)
        .WillOnce(DoAll(SetArgReferee<0>(3013501), Return(OK)));
    }

    {
        InSequence s2;

        EXPECT_CALL(pwmIn, read())
        .Times(1)
        .WillOnce(Return(GPIO::VLevel_t::GPIO_HIGH));

        EXPECT_CALL(pwmIn, read())
        .Times(1)
        .WillOnce(Return(GPIO::VLevel_t::GPIO_LOW));

        EXPECT_CALL(pwmIn, read())
        .Times(1)
        .WillOnce(Return(GPIO::VLevel_t::GPIO_HIGH));
    
    }
     
    PWMSW pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    ASSERT_EQ(OK, pwm.getDuty(duty));
    ASSERT_EQ(60 , duty);
}

/**
 * callback() tests
 */

TEST_F(PASCO2PWMSW_Test, callback_fallingEventSuccess)
{
    EXPECT_CALL(pwmIn, intEvent())
    .Times(1)
    .WillOnce(Return(GPIO::IntEvent_t::INT_FALLING_EDGE));

    PWMSWPub pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);
    
    pwm.callbackPub();
}

TEST_F(PASCO2PWMSW_Test, callback_risingEventSuccess)
{
    EXPECT_CALL(pwmIn, intEvent())
    .Times(1)
    .WillOnce(Return(GPIO::IntEvent_t::INT_RISING_EDGE));

    PWMSWPub pwm(&pwmIn, &timer, PWMSW::Mode_t::MODE_POLLING);

    pwm.callbackPub();
}