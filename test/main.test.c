#include <stdio.h>
#include "decode.test.c"
#include "encode.test.c"

int main()
{
    int16_t failedAssertions = 0;

    printf("\n");
    printf("decodeMqttChunk\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldDecodeAMessageWithA2ByteHeaderAndAZeroLengthRemainingSize();
    failedAssertions += itShouldDecodeAMessageWithA2ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA3ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA4ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA5ByteHeader();
    failedAssertions += itShouldDecodeAMessageWithA5ByteHeaderGivenThrough1ByteChunks();
    failedAssertions += itShouldMoveTheBytesOfANewMessageToTheBeginning();

    printf("\n");
    printf("Total failed assertions: %i\n", failedAssertions);
    printf("\n");
}