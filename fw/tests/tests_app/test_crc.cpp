
#include "CppUTest/TestHarness.h"
#include "crc.h"

TEST_GROUP(crcGeneration)
{
};

TEST( crcGeneration, checkOutputOne)
{
    byte abTest1[] = { 0, 0, 52, 18 };
    word crc1 = wCRCgenerateCRC( CRC_SEED, abTest1, sizeof( abTest1 ) );
    BYTES_EQUAL( 14411, crc1);
}

TEST( crcGeneration, checkOutputTwo)
{
    // add 14411 in big endian notation, should give a crc of zero
    byte abTest2[] = { 0, 0, 52, 18, 14411 >> 8, 14411 & 0xff };
    word crc2 = wCRCgenerateCRC( CRC_SEED, abTest2, sizeof( abTest2 ) );
    BYTES_EQUAL( 0, crc2);
}
