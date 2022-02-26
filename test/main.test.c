#include <stdio.h>
#include "decode.test.c"
#include "encode.test.c"

int main()
{
    int16_t failedAssertions = 0;

    printf("\n");
    printf("encodeMqttConnect\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeConnect();
    failedAssertions += itShouldEncodeConnectWithClientIdentifierSizeOf0();

    printf("\n");
    printf("encodeMqttPublish\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePublish();

    printf("\n");
    printf("encodeMqttPubAck\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubAck();

    printf("\n");
    printf("encodeMqttPubRec\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubRec();

    printf("\n");
    printf("encodeMqttPubRel\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubRel();

    printf("\n");
    printf("encodeMqttPubComp\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodePubComp();

    printf("\n");
    printf("encodeMqttSubscribe\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeSubscribe();

    printf("\n");
    printf("decodeMqttUnsubscribe\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldEncodeUnsubscribe();

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