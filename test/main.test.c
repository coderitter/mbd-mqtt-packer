#include <stdio.h>
#include "unpack.test.c"
#include "pack.test.c"
#include "get_size.test.c"

int main()
{
    int16_t failed_assertions = 0;

    printf("\n");
    printf("get_mqtt_remaining_length_size\n");
    printf("--------------------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_remaining_length_size_for_1_byte_length();
    failed_assertions += it_should_get_the_correct_remaining_length_size_for_2_byte_length();
    failed_assertions += it_should_get_the_correct_remaining_length_size_for_3_byte_length();
    failed_assertions += it_should_get_the_correct_remaining_length_size_for_4_byte_length();

    printf("\n");
    printf("get_mqtt_connect_size\n");
    printf("------------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_connect_size();
    failed_assertions += it_should_get_the_correct_connect_size_with_client_identifier_size_of_0();
    
    printf("\n");
    printf("get_mqtt_publish_Size\n");
    printf("------------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_publish_size();

    printf("\n");
    printf("get_mqtt_puback_size\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_puback_size();

    printf("\n");
    printf("get_mqtt_pubrec_size\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_pubrec_size();

    printf("\n");
    printf("get_mqtt_pubrel_size\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_pubrel_size();

    printf("\n");
    printf("get_mqtt_pubcomp_size\n");
    printf("------------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_pubcomp_size();

    printf("\n");
    printf("get_mqtt_subscribe_size\n");
    printf("--------------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_subscribe_size();

    printf("\n");
    printf("get_mqtt_unsubscribe_size\n");
    printf("----------------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_unsubscribe_size();

    printf("\n");
    printf("get_pingreq_size\n");
    printf("--------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_pingreq_size();

    printf("\n");
    printf("get_disconnect_size\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_get_the_correct_disconnect_size();

    printf("\n");
    printf("pack_mqtt_remaining_length\n");
    printf("-------------------------\n");
    printf("\n");

    failed_assertions += it_should_pack_the_remaining_length_with_1_byte();
    failed_assertions += it_should_pack_the_remaining_length_with_2_bytes();
    failed_assertions += it_should_pack_the_remaining_length_with_3_bytes();
    failed_assertions += it_should_pack_the_remaining_length_with_4_bytes();

    printf("\n");
    printf("pack_mqtt_connect\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_pack_connect();
    failed_assertions += it_should_pack_connect_with_client_identifier_size_of_0();

    printf("\n");
    printf("pack_mqtt_publish\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_pack_publish();
    failed_assertions += it_should_pack_publish_with_qos_0();

    printf("\n");
    printf("pack_mqtt_puback\n");
    printf("----------------\n");
    printf("\n");

    failed_assertions += it_should_pack_puback();

    printf("\n");
    printf("pack_mqtt_pubrec\n");
    printf("----------------\n");
    printf("\n");

    failed_assertions += it_should_pack_pubrec();

    printf("\n");
    printf("pack_mqtt_pubrel\n");
    printf("----------------\n");
    printf("\n");

    failed_assertions += it_should_pack_pubrel();

    printf("\n");
    printf("pack_mqtt_pubcomp\n");
    printf("---------------\n");
    printf("\n");

    failed_assertions += it_should_pack_pubcomp();

    printf("\n");
    printf("pack_mqtt_subscribe\n");
    printf("-------------------\n");
    printf("\n");

    failed_assertions += it_should_pack_subscribe();

    printf("\n");
    printf("pack_mqtt_unsubscribe\n");
    printf("---------------------\n");
    printf("\n");

    failed_assertions += it_should_pack_unsubscribe();

    printf("\n");
    printf("pack_pingreq\n");
    printf("-------------\n");
    printf("\n");

    failed_assertions += it_should_pack_pingreq();

    printf("\n");
    printf("pack_disconnect\n");
    printf("----------------\n");
    printf("\n");

    failed_assertions += it_should_pack_disconnect();

    printf("\n");
    printf("unpack_mqtt_packet\n");
    printf("---------------\n");
    printf("\n");

    failed_assertions += it_should_unpack_with_a_2_byte_header_and_a_zero_length_remaining_size();
    failed_assertions += it_should_unpack_with_a_2_byte_header();
    failed_assertions += it_should_unpack_with_a_3_byte_header();
    failed_assertions += it_should_unpack_with_a_4_byte_header();
    failed_assertions += it_should_unpack_with_a_5_byte_header();
    failed_assertions += it_should_unpack_with_a_5_byte_header_given_through_1_byte_chunks();
    failed_assertions += it_should_unpack_only_one_packet();

    printf("\n");
    printf("unpack_mqtt_packet_identifier\n");
    printf("--------------------------\n");
    printf("\n");

    failed_assertions += it_should_unpack_the_packet_identifier_which_is_after_the_fixed_header();

    printf("\n");
    printf("unpack_mqtt_connack\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_unpack_a_connack_packet();

    printf("\n");
    printf("unpack_mqtt_publish\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_unpack_a_publish_packet();
    failed_assertions += it_should_unpack_a_publish_packet_without_packet_identifier();

    printf("\n");
    printf("unpack_mqtt_suback\n");
    printf("-----------------\n");
    printf("\n");

    failed_assertions += it_should_unpack_a_suback_packet();

    printf("\n");
    printf("Total failed assertions: %i\n", failed_assertions);
    printf("\n");
}