#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "pas-co2-uart.hpp"
#include "pas-co2-pal-uart.hpp"

using namespace pasco2;

using ::testing::Return;
using ::testing::_;
using ::testing::DoAll;
using ::testing::SetArrayArgument;

/**
 * UART PAL Mock
 */
class MockUARTPAL : public UARTPAL
{
    public:
        MOCK_METHOD(Error_t, init,          (), (override));
        MOCK_METHOD(Error_t, deinit,        (), (override));
        MOCK_METHOD(Error_t, config,        (const uint32_t  baudrate, const DataBits_t dataBits, const Parity_t parity, const StopBits_t stopBits), (override));
        MOCK_METHOD(Error_t, write,         (const uint8_t * data, uint16_t & length), (override));
        MOCK_METHOD(Error_t, read,          (      uint8_t * data, uint16_t & length), (override));
        MOCK_METHOD(Error_t, waitReceive,   (      uint16_t  bytesRcvd, uint32_t timeoutMs), (override));
};

/**
 * UART Mock (Mock Private Class functions)
 */
class MockUART: public UART
{
    public:
        MockUART    (UARTPAL  * const uartpal,
                     uint32_t         baudrateBps = dfltBaudrateBps) : 
                     UART(uartpal, baudrateBps){};

        MOCK_METHOD(void,    assembleWriteRequestFrame, (const uint8_t   regAddr, const uint8_t   regVal, char * frame),  (override));
        MOCK_METHOD(void,    assembleReadRequestFrame,  (const uint8_t   regAddr,                         char * frame),  (override));
        MOCK_METHOD(uint8_t, disassembleReadReplyFrame, (const char    * frame),                                          (override));
        MOCK_METHOD(bool,    isAckFrame,                (const char    * frame,   const uint8_t    len),                  (override));
        MOCK_METHOD(bool,    isNackFrame,               (const char    * frame,   const uint8_t    len),                  (override));
};

/**
 * PASCO2 Public( Private function Public)
 */
class UARTPub: public UART
{
    public:
         UARTPub                            (UARTPAL  * const uartpal,
                                             uint32_t         baudrateBps = dfltBaudrateBps) : 
                                             UART(uartpal, baudrateBps){};

    char uint4ToHexCharPub                  (const uint8_t   uint4)
                                            {
                                                return uint4ToHexChar(uint4);
                                            };

    void assembleWriteRequestFramePub       (const uint8_t   regAddr, 
                                             const uint8_t   data,
                                                   char    * frame)
                                            {
                                                assembleWriteRequestFrame(regAddr, data, frame);
                                            };

    void assembleReadRequestFramePub        (const uint8_t   regAddr, 
                                                   char    * frame)
                                            {
                                                assembleReadRequestFrame(regAddr, frame);
                                            };

    uint8_t disassembleReadReplyFramePub    (const char    * frame)
                                            {
                                                return  disassembleReadReplyFrame(frame);
                                            };

    bool    isAckFramePub                   (const char    * frame,
                                             const uint8_t   len)
                                            {
                                                return isAckFrame(frame, len);
                                            };

    bool    isNackFramePub                  (const char    * frame,
                                             const uint8_t   len)
                                            {
                                                return isNackFrame(frame,len);
                                            };

};

/**
 * Class C++ PASCO2 UART Test Suite
 */
class PASCO2UART_Test: public ::testing::Test
{
    public:
       
        MockUARTPAL uartpal;

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

TEST_F(PASCO2UART_Test, getProtocol)
{
    UART uart(&uartpal);

    ASSERT_EQ(SBus::BUS_PROTO_UART, uart.getProtocol());
}

/**
 * init() tests
 */

TEST_F(PASCO2UART_Test, init_initError)
{
    UART uart(&uartpal);

    EXPECT_CALL(uartpal, init())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.init());
}

TEST_F(PASCO2UART_Test, init_configError)
{
    UART uart(&uartpal);

    EXPECT_CALL(uartpal, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, config(_, _ , _, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.init());
}

TEST_F(PASCO2UART_Test, init_Success)
{
    UART uart(&uartpal);

    EXPECT_CALL(uartpal, init())
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, config(_, _ , _, _))
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, uart.init());
}

/**
 * deinit() tests
 */

TEST_F(PASCO2UART_Test, deinit_deinitError)
{
    UART uart(&uartpal);

    EXPECT_CALL(uartpal, deinit())
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.deinit());
}

TEST_F(PASCO2UART_Test, deinit_Success)
{
    UART uart(&uartpal);

    EXPECT_CALL(uartpal, deinit())
    .Times(1)
    .WillOnce(Return(OK));

    ASSERT_EQ(OK, uart.deinit());
}

/**
 * write() tests
 */

TEST_F(PASCO2UART_Test, write_writeError)
{
    uint8_t  memAddr = 0x01;
    uint8_t  data[2] = {0x33, 0x44};
    uint16_t length  = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleWriteRequestFrame(_, _, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.write(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, write_waitReceiveError)
{
    uint8_t  memAddr = 0x01;
    uint8_t  data[2] = {0x33, 0x44};
    uint16_t length  = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleWriteRequestFrame(_, _, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.write(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, write_readError)
{
    uint8_t  memAddr = 0x01;
    uint8_t  data[2] = {0x33, 0x44};
    uint16_t length  = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleWriteRequestFrame(_, _, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, read(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.write(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, write_isAckFrameError)
{
    uint8_t  memAddr    = 0x01;
    uint8_t  data[2]    = {0x33, 0x44};
    uint16_t length     = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleWriteRequestFrame(_, _, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, read(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uart, isAckFrame(_, _))
    .Times(1)
    .WillOnce(Return(false));

    ASSERT_EQ(INTF_ERROR, uart.write(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, write_Success)
{
    uint8_t  memAddr    = 0x01;
    uint8_t  data[2]    = {0x33, 0x44};
    uint16_t length     = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleWriteRequestFrame(_, _, _))
    .Times(2);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(uartpal, read(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(uart, isAckFrame(_, _))
    .Times(2)
    .WillRepeatedly(Return(true));

    ASSERT_EQ(OK, uart.write(memAddr, data, length));
}

/**
 * read() tests
 */

TEST_F(PASCO2UART_Test, read_writeError)
{
    uint8_t  memAddr = 0x01;
    uint8_t  data[2] = {0};
    uint16_t length  = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleReadRequestFrame(_, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.read(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, read_waitReceiveError)
{
    uint8_t  memAddr = 0x01;
    uint8_t  data[2] = {0};
    uint16_t length  = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleReadRequestFrame(_, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.read(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, read_readError)
{
    uint8_t  memAddr = 0x01;
    uint8_t  data[2] = {0};
    uint16_t length  = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleReadRequestFrame(_, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, read(_, _))
    .Times(1)
    .WillOnce(Return(INTF_ERROR));

    ASSERT_EQ(INTF_ERROR, uart.read(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, read_isNackFrameError)
{
    uint8_t  memAddr    = 0x01;
    uint8_t  data[2]    = {0};
    uint16_t length     = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleReadRequestFrame(_, _))
    .Times(1);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uartpal, read(_, _))
    .Times(1)
    .WillOnce(Return(OK));

    EXPECT_CALL(uart, isNackFrame(_, _))
    .Times(1)
    .WillOnce(Return(true));

    ASSERT_EQ(INTF_ERROR, uart.read(memAddr, data, length));
}

TEST_F(PASCO2UART_Test, read_Success)
{
    uint8_t  memAddr    = 0x01;
    uint8_t  data[2]    = {0};
    uint16_t length     = 2;

    MockUART uart(&uartpal);

    EXPECT_CALL(uart, assembleReadRequestFrame(_, _))
    .Times(2);

    EXPECT_CALL(uartpal, write(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(uartpal, waitReceive(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(uartpal, read(_, _))
    .Times(2)
    .WillRepeatedly(Return(OK));

    EXPECT_CALL(uart, isNackFrame(_, _))
    .Times(2)
    .WillRepeatedly(Return(false));

    EXPECT_CALL(uart, disassembleReadReplyFrame(_))
    .Times(2);

    ASSERT_EQ(OK, uart.read(memAddr, data, length));
}

/**
 * uint4ToHexChar() tests
 */

TEST_F(PASCO2UART_Test, uint4ToHexChar_Success)
{
    UARTPub uart(&uartpal);

    ASSERT_EQ('1', uart.uint4ToHexCharPub(0x1));
    ASSERT_EQ('2', uart.uint4ToHexCharPub(0x2));
    ASSERT_EQ('3', uart.uint4ToHexCharPub(0x3));
    ASSERT_EQ('4', uart.uint4ToHexCharPub(0x4));
    ASSERT_EQ('5', uart.uint4ToHexCharPub(0x5));
    ASSERT_EQ('6', uart.uint4ToHexCharPub(0x6));
    ASSERT_EQ('7', uart.uint4ToHexCharPub(0x7));
    ASSERT_EQ('8', uart.uint4ToHexCharPub(0x8));
    ASSERT_EQ('9', uart.uint4ToHexCharPub(0x9));
    ASSERT_EQ('a', uart.uint4ToHexCharPub(0xA));
    ASSERT_EQ('b', uart.uint4ToHexCharPub(0xB));
    ASSERT_EQ('c', uart.uint4ToHexCharPub(0xC));
    ASSERT_EQ('d', uart.uint4ToHexCharPub(0xD));
    ASSERT_EQ('e', uart.uint4ToHexCharPub(0xE));
    ASSERT_EQ('f', uart.uint4ToHexCharPub(0xF));
}

/**
 * assembleWriteRequestFrame() tests
 */

TEST_F(PASCO2UART_Test, assembleWriteRequestFrame_Success)
{  
    char    reqFrame[8] = {0};
    uint8_t regAddr     = 0xAB;
    uint8_t regVal      = 0xCD;

    UARTPub uart(&uartpal);

    uart.assembleWriteRequestFramePub(regAddr, regVal, (char*)reqFrame);

    ASSERT_STREQ("w,ab,cd\n", reqFrame);
}

/**
 * assembleReadRequestFrame() tests
 */

TEST_F(PASCO2UART_Test, assembleReadRequestFrame_Success)
{  
    char    reqFrame[8] = {0};
    uint8_t regAddr     = 0xED;

    UARTPub uart(&uartpal);

    uart.assembleReadRequestFramePub(regAddr, (char*)reqFrame);

    ASSERT_STREQ("r,ed\n", reqFrame);
}

/**
 * disassembleReadReplyFrame()
 */

TEST_F(PASCO2UART_Test, disassembleReadReplyFrame_Success)
{  
    UARTPub uart(&uartpal);

    ASSERT_EQ(0x3F, uart.disassembleReadReplyFramePub((char*)"3f\n"));
    ASSERT_EQ(0xFE, uart.disassembleReadReplyFramePub((char*)"fe\n"));
    ASSERT_EQ(0xB4, uart.disassembleReadReplyFramePub((char*)"B4\n"));
    ASSERT_EQ(0xAF, uart.disassembleReadReplyFramePub((char*)"AF\n"));
    ASSERT_EQ(0xC0, uart.disassembleReadReplyFramePub((char*)"c0\n"));
    ASSERT_EQ(0x1A, uart.disassembleReadReplyFramePub((char*)"1a\n"));
}

/**
 * isAckFrame() tests
 */

TEST_F(PASCO2UART_Test, isAckFrame_False)
{
    UARTPub uart(&uartpal);

    char  frame[2] = {0x06, 0x0A};

    ASSERT_FALSE(uart.isAckFramePub((char*)frame, 4));

    frame[0] = 0x01;
    frame[1] = 0x0D;

    ASSERT_FALSE(uart.isAckFramePub((char*)frame, 2));
}

TEST_F(PASCO2UART_Test, isAckFrame_True)
{
    UARTPub uart(&uartpal);

    char  frame[2] = {0x06, 0x0A};

    ASSERT_TRUE(uart.isAckFramePub((char*)frame, 2));
}

/**
 * isNackFrame() tests
 */

TEST_F(PASCO2UART_Test, isNackFrame_False)
{
    UARTPub uart(&uartpal);

    char  frame[2] = {0x15, 0x0A};

    ASSERT_FALSE(uart.isNackFramePub((char*)frame, 4));

    frame[0] = 0x01;
    frame[1] = 0x0D;

    ASSERT_FALSE(uart.isNackFramePub((char*)frame, 2));
}

TEST_F(PASCO2UART_Test, isNackFrame_True)
{
    UARTPub uart(&uartpal);

    char  frame[2] = {0x15, 0x0A};

    ASSERT_TRUE(uart.isNackFramePub((char*)frame, 2));
}