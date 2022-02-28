#include <stdio.h>
#include "unpack.test.c"
#include "pack.test.c"
#include "getSize.test.c"

int main()
{
    int16_t failedAssertions = 0;

    printf("\n");
    printf("getMqttRemainingLengthSize\n");
    printf("--------------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA1ByteLength();
    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA2ByteLength();
    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA3ByteLength();
    failedAssertions += itShouldGetTheCorrectRemainingLengthSizeForA4ByteLength();

    printf("\n");
    printf("getMqttConnectSize\n");
    printf("------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectConnectSize();
    failedAssertions += itShouldGetTheCorrectConnectSizeWithClientIdentifierSizeOf0();
    
    printf("\n");
    printf("getMqttPublishSize\n");
    printf("------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPublishSize();

    printf("\n");
    printf("getMqttPubAckSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubAckSize();

    printf("\n");
    printf("getMqttPubRecSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubRecSize();

    printf("\n");
    printf("getMqttPubRelSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubRelSize();

    printf("\n");
    printf("getMqttPubCompSize\n");
    printf("------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPubCompSize();

    printf("\n");
    printf("getMqttSubscribeSize\n");
    printf("--------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectSubscribeSize();

    printf("\n");
    printf("getMqttUnsubscribeSize\n");
    printf("----------------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectUnsubscribeSize();

    printf("\n");
    printf("getPingReqSize\n");
    printf("--------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectPingReqSize();

    printf("\n");
    printf("getDisconnectSize\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldGetTheCorrectDisconnectSize();

    printf("\n");
    printf("packMqttRemainingLength\n");
    printf("-------------------------\n");
    printf("\n");

    failedAssertions += itShouldPackTheRemainingLengthWith1Byte();
    failedAssertions += itShouldPackTheRemainingLengthWith2Bytes();
    failedAssertions += itShouldPackTheRemainingLengthWith3Bytes();
    failedAssertions += itShouldPackTheRemainingLengthWith4Bytes();

    printf("\n");
    printf("packMqttConnect\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldPackConnect();
    failedAssertions += itShouldPackConnectWithClientIdentifierSizeOf0();

    printf("\n");
    printf("packMqttPublish\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldPackPublish();

    printf("\n");
    printf("packMqttPubAck\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldPackPubAck();

    printf("\n");
    printf("packMqttPubRec\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldPackPubRec();

    printf("\n");
    printf("packMqttPubRel\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldPackPubRel();

    printf("\n");
    printf("packMqttPubComp\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldPackPubComp();

    printf("\n");
    printf("packMqttSubscribe\n");
    printf("-------------------\n");
    printf("\n");

    failedAssertions += itShouldPackSubscribe();

    printf("\n");
    printf("packMqttUnsubscribe\n");
    printf("---------------------\n");
    printf("\n");

    failedAssertions += itShouldPackUnsubscribe();

    printf("\n");
    printf("packPingReq\n");
    printf("-------------\n");
    printf("\n");

    failedAssertions += itShouldPackPingReq();

    printf("\n");
    printf("packDisconnect\n");
    printf("----------------\n");
    printf("\n");

    failedAssertions += itShouldPackDisconnect();

    printf("\n");
    printf("unpackMqttChunk\n");
    printf("---------------\n");
    printf("\n");

    failedAssertions += itShouldUnpackWithA2ByteHeaderAndAZeroLengthRemainingSize();
    failedAssertions += itShouldUnpackWithA2ByteHeader();
    failedAssertions += itShouldUnpackWithA3ByteHeader();
    failedAssertions += itShouldUnpackWithA4ByteHeader();
    failedAssertions += itShouldUnpackWithA5ByteHeader();
    failedAssertions += itShouldUnpackWithA5ByteHeaderGivenThrough1ByteChunks();
    failedAssertions += itShouldMoveTheBytesOfANewPacketToTheBeginning();

    printf("\n");
    printf("unpackMqttPacketIdentifier\n");
    printf("--------------------------\n");
    printf("\n");

    failedAssertions += itShouldUnpackThePacketIdentifierWhichIsAfterTheFixedHeader();

    printf("\n");
    printf("unpackMqttConnAck\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldUnpackAConnAckPacket();

    printf("\n");
    printf("unpackMqttPublish\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldUnpackAPublishPacket();

    printf("\n");
    printf("unpackMqttSubAck\n");
    printf("-----------------\n");
    printf("\n");

    failedAssertions += itShouldUnpackASubAckPacket();

    printf("\n");
    printf("Total failed assertions: %i\n", failedAssertions);
    printf("\n");
}