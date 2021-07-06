#include "test_pas-co2-mock.hpp"

/**
 * Class C++ PASCO2 Class Test Suite
 */
class PASCO2_Test: public ::testing::Test
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
 * init() tests
 */

TEST_F(PASCO2_Test, init_Success)
{
    MockPWM   pwm;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(nullptr, &pwm);
    
    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_AlreadyInited)
{
    MockPWM   pwm;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(nullptr, &pwm);
    
    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.init());
}

TEST_F(PASCO2_Test, init_SBusAndPWMInstancesNullError)
{
    PASCO2 cotwo(nullptr, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.init());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_SBusInitError)
{
    MockSBus sbus;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.init());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_SBusInitNoGPIOPortsSuccess)
{
    MockSBus sbus;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_SBusProtoSelectInitError)
{
    MockSBus sbus;
    MockGPIO protoSelect;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(protoSelect, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, &protoSelect);

    ASSERT_EQ(INTF_ERROR, cotwo.init());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_SBusProtoSelectInitSuccessI2CError)
{
    MockSBus sbus;
    MockGPIO protoSelect;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(protoSelect, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, getProtocol())
    .Times(1)
    .WillOnce(Return(SBus::Proto_t::BUS_PROTO_I2C));

    EXPECT_CALL(protoSelect, write(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, &protoSelect);

    ASSERT_EQ(INTF_ERROR, cotwo.init());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_SBusProtoSelectInitSuccessI2CSuccess)
{
    MockSBus sbus;
    MockGPIO protoSelect;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(protoSelect, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, getProtocol())
    .Times(1)
    .WillOnce(Return(SBus::Proto_t::BUS_PROTO_I2C));

    EXPECT_CALL(protoSelect, write(_))
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, &protoSelect);

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_SBusProtoSelectInitSuccessUARTError)
{
    MockSBus sbus;
    MockGPIO protoSelect;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(protoSelect, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, getProtocol())
    .Times(1)
    .WillOnce(Return(SBus::Proto_t::BUS_PROTO_UART));

    EXPECT_CALL(protoSelect, write(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(&sbus, nullptr, nullptr, nullptr, &protoSelect);

    ASSERT_EQ(INTF_ERROR, cotwo.init());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_PWMInitError)
{
    MockPWM pwm;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(INTF_ERROR, cotwo.init());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_PWMInitSuccess)
{
    MockPWM pwm;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm);

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_NoGPIOInstancesSBusPWMSuccess)
{
    MockSBus sbus;
    MockPWM pwm;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(&sbus, &pwm);

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_TimerInitError)
{
    MockPWM   pwm;
    MockTimer timer;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm, &timer);

    ASSERT_EQ(INTF_ERROR, cotwo.init());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_TimerInitSuccess)
{
    MockPWM   pwm;
    MockTimer timer;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(nullptr, &pwm, &timer);

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, init_GPIOPortsInitError)
{
    MockSBus  sbus;
    MockGPIO  generalGPIO;

    MockGPIO * GPIOSet[] = {&generalGPIO, nullptr, nullptr, nullptr, nullptr, nullptr};

    EXPECT_CALL(sbus, init())
    .Times(5)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(generalGPIO, init())
    .Times(5)
    .WillRepeatedly(Return(INTF_ERROR));

    for(int i = 0; i < 5; i++)
    {
        PASCO2 cotwo(&sbus, nullptr, nullptr, GPIOSet[0], GPIOSet[1], GPIOSet[2], GPIOSet[3], GPIOSet[4]);
      
        ASSERT_EQ(INTF_ERROR, cotwo.init());
        ASSERT_EQ(UNINITED, cotwo.getStatus());

        GPIOSet[i] = nullptr;
        GPIOSet[i + 1] = &generalGPIO;
    }
}

TEST_F(PASCO2_Test, init_AllInstancesInitSuccess)
{
    MockSBus  sbus;
    MockPWM   pwm;
    MockTimer timer;
    MockGPIO  generalGPIO;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(generalGPIO, init())
    .Times(5)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, getProtocol())
    .Times(1)
    .WillOnce(Return(SBus::Proto_t::BUS_PROTO_UART));

    EXPECT_CALL(generalGPIO, write(_))
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(&sbus, &pwm, &timer, &generalGPIO, &generalGPIO, &generalGPIO, &generalGPIO, &generalGPIO);
    
    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

/**
 * deinit() tests
 */

TEST_F(PASCO2_Test, deinit_AlreadyDeinited)
{
    MockPWM   pwm;

    MockPASCO2 cotwo(nullptr, &pwm);
    
    ASSERT_EQ(UNINITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.deinit());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, deinit_setStatusError)
{
    MockPWM   pwm;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(nullptr, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));
    
    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.deinit());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, deinit_SBusDeinitError)
{
    MockSBus   sbus;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.deinit());
    ASSERT_EQ(INITED, cotwo.getStatus());   /* As we are using setStatus mock */ 
}

TEST_F(PASCO2_Test, deinit_PWMDeinitError)
{
    MockPWM   pwm;
    MockSBus  sbus;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    {
        InSequence s;

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.deinit());
    ASSERT_EQ(INITED, cotwo.getStatus()); /* As we are using setStatus mock */ 
}

TEST_F(PASCO2_Test, deinit_NoGPIOInstancesSBusPWMSuccess)
{
    MockSBus sbus;
    MockPWM pwm;


    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    MockPASCO2 cotwo(&sbus, &pwm);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.deinit());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, deinit_TimerDeinitError)
{
    MockSBus  sbus;
    MockTimer timer;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    {
        InSequence s;

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillOnce(Return(OK));   
        
        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    MockPASCO2 cotwo(&sbus, nullptr, &timer);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.deinit());
    ASSERT_EQ(INITED, cotwo.getStatus());   /* As we are using setStatus mock */ 

}

TEST_F(PASCO2_Test, deinit_TimerDeinitSuccess)
{
    MockSBus  sbus;
    MockTimer timer;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(OK));   
        
    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    MockPASCO2 cotwo(&sbus, nullptr, &timer);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.deinit());
    ASSERT_EQ(UNINITED, cotwo.getStatus());   /* As we are using setStatus mock */ 

}

TEST_F(PASCO2_Test, deinit_GPIOPortsDeinitError)
{
    MockSBus  sbus;
    MockGPIO  generalGPIO;

    MockGPIO * GPIOSet[] = {&generalGPIO, nullptr, nullptr, nullptr, nullptr, nullptr};

    EXPECT_CALL(sbus, init())
    .Times(5)
    .WillRepeatedly(Return(OK));
    
    {
        InSequence s;

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(OK));   
        
        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(INTF_ERROR));

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(OK));   
        
        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(INTF_ERROR));

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(OK));   
        
        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(INTF_ERROR));

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(OK));   
        
        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(INTF_ERROR));

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(OK));   

        EXPECT_CALL(sbus, deinit())
        .Times(1)
        .WillRepeatedly(Return(INTF_ERROR));
        
    }

    EXPECT_CALL(generalGPIO, init())
    .Times(5)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(generalGPIO, deinit())
    .Times(5)
    .WillRepeatedly(Return(INTF_ERROR));

    EXPECT_CALL(sbus, getProtocol())
    .Times(1)
    .WillOnce(Return(SBus::Proto_t::BUS_PROTO_UART));

    EXPECT_CALL(generalGPIO, write(_))
    .Times(1)
    .WillOnce(Return(OK));

    for(int i = 0; i < 5; i++)
    {
        MockPASCO2 cotwo(&sbus, nullptr, nullptr, GPIOSet[0], GPIOSet[1], GPIOSet[2], GPIOSet[3], GPIOSet[4]);

        EXPECT_CALL(cotwo, setStatus(_, _))
        .Times(1)
        .WillOnce(Return(OK));

        ASSERT_EQ(OK, cotwo.init());
        ASSERT_EQ(INTF_ERROR, cotwo.deinit());
      
        GPIOSet[i] = nullptr;
        GPIOSet[i + 1] = &generalGPIO;
    }
}

TEST_F(PASCO2_Test, deinit_AllInstancesDeinitSuccess)
{
    MockSBus  sbus;
    MockPWM   pwm;
    MockTimer timer;
    MockGPIO  generalGPIO;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(generalGPIO, init())
    .Times(5)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(generalGPIO, deinit())
    .Times(5)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, getProtocol())
    .Times(1)
    .WillOnce(Return(SBus::Proto_t::BUS_PROTO_UART));

    EXPECT_CALL(generalGPIO, write(_))
    .Times(1)
    .WillOnce(Return(OK));

    MockPASCO2 cotwo(&sbus, &pwm, &timer, &generalGPIO, &generalGPIO, &generalGPIO, &generalGPIO, &generalGPIO);

    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));
    
    ASSERT_EQ(OK, cotwo.init());
    ASSERT_EQ(OK, cotwo.deinit());
    ASSERT_EQ(UNINITED,cotwo.getStatus());
}

/**
 * enableLogic() tests
 */

TEST_F(PASCO2_Test, enableLogic_setStatusError)
{
    MockGPIO  power3V3; 

    MockPASCO2 cotwo(nullptr, nullptr, nullptr, nullptr, nullptr,  &power3V3);
    
    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.enableLogic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, enableLogic_power3V3EnableError)
{
    MockGPIO  power3V3; 

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    MockPASCO2 cotwo(nullptr, nullptr, nullptr, nullptr, nullptr,  &power3V3);
    
    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(INTF_ERROR, cotwo.enableLogic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());     /* Using setStatus Mock */
}

TEST_F(PASCO2_Test, enableLogic_power3V3EnableSuccess)
{
    MockPWM   pwm;
    MockGPIO  power3V3; 
    MockGPIO  power12V;

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power12V, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    /* Called through ~PASCO2() deinit() */
    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr,  &power3V3, &power12V);

    /* Init implicit */
    ASSERT_EQ(OK, cotwo.enableLogic());
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus()); 
}

TEST_F(PASCO2_Test, enableLogic_TimerDelayError)
{
    MockSBus  sbus;
    MockTimer timer;
    MockGPIO  power3V3; 

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    /* Called through ~PASCO2() deinit() */
    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, delay(_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(power3V3, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(&sbus, nullptr, &timer, nullptr, nullptr,  &power3V3);

    /* Init implicit */
    ASSERT_EQ(INTF_ERROR, cotwo.enableLogic());
    ASSERT_EQ(INITED, cotwo.getStatus()); 
}

TEST_F(PASCO2_Test, enableLogic_SBusReadError)
{
    MockSBus  sbus;
    MockTimer timer;
    MockGPIO  power3V3; 

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    /* Called through ~PASCO2() deinit() */
    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(sbus, read(_,_,_))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, delay(_))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(&sbus, nullptr, &timer, nullptr, nullptr,  &power3V3);

    /* Init implicit */
    ASSERT_EQ(INTF_ERROR, cotwo.enableLogic());
    ASSERT_EQ(INITED, cotwo.getStatus()); 
}

TEST_F(PASCO2_Test, enableLogic_SBusPowerOnError)
{
    MockSBus  sbus;


    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    /* Called through ~PASCO2() deinit() */
    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(sbus, read(_,_,_))
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(&sbus, nullptr);

    /* Init implicit */
    ASSERT_EQ(IC_POWERON_ERROR, cotwo.enableLogic());
    ASSERT_EQ(INITED, cotwo.getStatus()); 
}

TEST_F(PASCO2_Test, enableLogic_WithSBusSuccesss)
{
    MockSBus  sbus;
    MockTimer timer;
    MockGPIO  power3V3; 

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillOnce(Return(OK));

    uint8_t regVal[2] = {0x00, 0x80};
    EXPECT_CALL(sbus, read(_,_,_))
    .Times(1)
    .WillRepeatedly(DoAll(SetArrayArgument<1>(regVal, regVal + 2), Return(OK)));

    EXPECT_CALL(timer, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(timer, delay(_))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    /* Called through ~PASCO2() deinit() */
    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(&sbus, nullptr, &timer, nullptr, nullptr,  &power3V3);

    /* Init implicit */
    ASSERT_EQ(OK, cotwo.enableLogic());
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus()); 
}

TEST_F(PASCO2_Test, enableLogic_AlreadyEnabledSuccess)
{
    MockPWM   pwm;
    MockGPIO  power3V3; 

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    /* Called through ~PASCO2() deinit() */
    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr,  &power3V3);

    /* Init implicit */
    ASSERT_EQ(OK, cotwo.enableLogic());
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus()); 
    ASSERT_EQ(OK, cotwo.enableLogic());
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus()); 
}

/**
 * disableLogic() tests
 */

TEST_F(PASCO2_Test, disableLogic_NullPower3V3InstanceSuccess)
{

    PASCO2 cotwo(nullptr, nullptr);

    ASSERT_EQ(OK, cotwo.disableLogic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, disableLogic_AlreadyDisabledSuccess)
{
    MockGPIO power3V3;

    PASCO2 cotwo(nullptr, nullptr, nullptr, nullptr, nullptr,  &power3V3);

    ASSERT_EQ(OK, cotwo.disableLogic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, disableLogic_power3V3DisableError)
{
    MockPWM   pwm;
    MockGPIO  power3V3; 

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr,  &power3V3);
    
    /* Init implicit */
    ASSERT_EQ(OK, cotwo.enableLogic());
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.disableLogic());
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2_Test, disableLogic_power3V3DisableSuccess)
{
    MockPWM   pwm;
    MockGPIO  power3V3; 

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(power3V3, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, enable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power3V3, disable())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr,  &power3V3);
    
    /* Init implicit */
    ASSERT_EQ(OK, cotwo.enableLogic());
    ASSERT_EQ(LOGIC_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.disableLogic());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

/**
 * enableIREmitter() tests
 */

TEST_F(PASCO2_Test, enableIREmitter_setStatusError)
{
    MockGPIO  power12V; 

    MockPASCO2 cotwo(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &power12V);
    
    EXPECT_CALL(cotwo, setStatus(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, cotwo.enableIREmitter());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}


TEST_F(PASCO2_Test, enableIREmitter_NullPower12VInstanceSuccess)
{
    MockSBus sbus;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_LE(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.enableIREmitter());
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2_Test, enableIREmitter_power12VEnableError)
{
    MockPWM   pwm;
    MockGPIO  power12V; 

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    EXPECT_CALL(power12V, init())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr,  &power12V);
    
    ASSERT_EQ(OK, cotwo.init());
    ASSERT_LE(INITED, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.enableIREmitter());
    ASSERT_LE(INITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, enableIREmitter_power12VEnableSuccess)
{
    MockPWM   pwm;
    MockGPIO  power12V; 

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(power12V, init())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(power12V, disable())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr,  &power12V);
    
    ASSERT_EQ(OK, cotwo.init());
    ASSERT_LE(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.enableIREmitter());
    ASSERT_LE(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2_Test, enableIREmitter_AlreadyEnabledSuccess)
{
    MockSBus sbus;

    EXPECT_CALL(sbus, init())
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, deinit())
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.init());
    ASSERT_LE(INITED, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.enableIREmitter());
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.enableIREmitter());
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

/**
 * disableIREmitter() tests
 */

TEST_F(PASCO2_Test,disableIREmitter_NullPower3V3InstanceSuccess)
{
    PASCO2 cotwo(nullptr, nullptr);

    ASSERT_EQ(OK, cotwo.disableIREmitter());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, disableIREmitter_AlreadyDisabledSuccess)
{
    MockSBus sbus;

    PASCO2 cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.disableIREmitter());
    ASSERT_EQ(UNINITED,cotwo.getStatus());
}

TEST_F(PASCO2_Test, disableIREmitter_power12VDisableError)
{
    MockPWM   pwm;
    MockGPIO  power12V; 

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power12V, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power12V, disable())
    .Times(2)
    .WillRepeatedly(Return(INTF_ERROR));

     PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);
    
    /* Init implicit */
    ASSERT_EQ(OK, cotwo.enableIREmitter());
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(INTF_ERROR, cotwo.disableIREmitter());
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
}

TEST_F(PASCO2_Test, disableIREmitter_power12VDisableSuccess)
{
    MockPWM   pwm;
    MockGPIO  power12V; 

    EXPECT_CALL(pwm, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(pwm, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    EXPECT_CALL(power12V, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power12V, enable())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(power12V, disable())
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2 cotwo(nullptr, &pwm, nullptr, nullptr, nullptr, nullptr, &power12V);
    
    /* Init implicit */
    ASSERT_EQ(OK, cotwo.enableIREmitter());
    ASSERT_EQ(IR_ONLY_ON, cotwo.getStatus());
    ASSERT_EQ(OK, cotwo.disableIREmitter());
    ASSERT_EQ(INITED, cotwo.getStatus());
}

/**
 * preResetSaveConfig() tests
 */

TEST_F(PASCO2_Test, preResetSaveConfig_SBusReadError)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.preResetSaveConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, preResetSaveConfig_SBusWriteError)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.preResetSaveConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, preResetSaveConfig_Success)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(OK));

    PASCO2Pub cotwo(&sbus, nullptr);

    ASSERT_EQ(OK, cotwo.preResetSaveConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

/**
 * postResetRestoreConfig() tests
 */

TEST_F(PASCO2_Test, postResetRestoreConfig_SBusFirstReadError)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(&sbus, nullptr);

    ASSERT_EQ(INTF_ERROR, cotwo.postResetRestoreConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, postResetRestoreConfig_PowerOnError)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(1)
    .WillOnce(Return(OK));

    PASCO2Pub cotwo(&sbus, nullptr);

    ASSERT_EQ(IC_POWERON_ERROR, cotwo.postResetRestoreConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, postResetRestoreConfig_SBusSecondReadError)
{
    MockSBus    sbus;

    {   
        InSequence s;

        EXPECT_CALL(sbus, read(_,_,_))
        .Times(1)
        .WillOnce(Return(OK));

        EXPECT_CALL(sbus, read(_, _, _))
        .Times(1)
        .WillOnce(Return(INTF_ERROR));
    }   

    PASCO2Pub cotwo(&sbus, nullptr);

    cotwo.writeReg(Reg::REG_ADDR_SENS_STS, 0x80);

    ASSERT_EQ(INTF_ERROR, cotwo.postResetRestoreConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, postResetRestoreConfig_ResetError)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    PASCO2Pub cotwo(&sbus, nullptr);

    cotwo.writeReg(Reg::REG_ADDR_SENS_STS, 0x80);
    cotwo.writeReg(Reg::REG_ADDR_SCRATCH_PAD, 0xED);

    ASSERT_EQ(RESET_ERROR, cotwo.postResetRestoreConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, postResetRestoreConfig_SBusWriteError)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(1)
    .WillRepeatedly(Return(INTF_ERROR));

    PASCO2Pub cotwo(&sbus, nullptr);

    cotwo.writeReg(Reg::REG_ADDR_SENS_STS, 0x80);
    cotwo.writeReg(Reg::REG_ADDR_SCRATCH_PAD, 0x00);

    ASSERT_EQ(INTF_ERROR, cotwo.postResetRestoreConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

TEST_F(PASCO2_Test, postResetRestoreConfig_Success)
{
    MockSBus    sbus;

    EXPECT_CALL(sbus, read(_, _, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(sbus, write(_, _, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    PASCO2Pub cotwo(&sbus, nullptr);

    cotwo.writeReg(Reg::REG_ADDR_SENS_STS, 0x80);
    cotwo.writeReg(Reg::REG_ADDR_SCRATCH_PAD, 0x00);

    ASSERT_EQ(OK, cotwo.postResetRestoreConfigPublic());
    ASSERT_EQ(UNINITED, cotwo.getStatus());
}

