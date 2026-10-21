/* 16-byte table-backed transform used by decrypt.py. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static uint8_t const_data[0x36750];

static uint32_t load_u32(const void *address) {
  const uint8_t *bytes = address;
  return (uint32_t)bytes[0] | (uint32_t)bytes[1] << 8 | (uint32_t)bytes[2] << 16 | (uint32_t)bytes[3] << 24;
}

static uint32_t table_word(const uint8_t *table, uint32_t index) {
  return load_u32(table + index * 4);
}

static uint32_t decode_first_stage_byte(uint8_t value) {
  return (value + 0x123) ^ 0x19b3092a;
}

/* These forms hold for every entry of the fixed tables used below. */
static uint32_t mix_table_word_a(uint32_t value) {
  return (value + 0xce853eba) ^ 0x4eb7d448;
}

static uint32_t mix_table_word_b(uint32_t value) {
  return (value + 0x4e853eba) ^ 0x8e4b91e3;
}

static uint32_t mix_table_word_c(uint32_t value) {
  return (value + 0xf8570001) ^ 0x59ca32d7;
}

static uint8_t decode_output_byte0(uint8_t value, uint8_t table_mask, uint8_t value_mask, uint8_t output_mask) {
  return (uint8_t)((const_data[196656 + (value ^ table_mask)] - (value ^ value_mask) - 3) ^ output_mask);
}

static uint8_t decode_output_byte1(uint8_t value, uint8_t table_mask, uint8_t value_mask, uint8_t output_mask) {
  return (uint8_t)(((value ^ value_mask) + 0x78) ^ const_data[144848 + (value ^ table_mask)] ^ output_mask);
}

static uint8_t decode_output_byte2(uint8_t value, uint8_t table_mask, uint8_t output_mask) {
  return const_data[144592 + (value ^ table_mask)] ^ output_mask;
}

static uint8_t decode_output_byte3(uint8_t value, uint8_t table_mask, uint8_t value_mask, uint8_t output_mask) {
  return (uint8_t)(((value ^ value_mask) + const_data[118912 + (value ^ table_mask)] + 0xa5) ^ output_mask);
}

static void transform_block(const uint8_t source[16], const uint8_t work[16], const uint8_t constant[16], uint8_t output[16]) {
  uint32_t uVar3;
  uint32_t uVar4;
  uint32_t uVar5;
  uint32_t uVar6;
  uint32_t uVar7;
  uint32_t uVar8;
  uint32_t uVar9;
  uint32_t uVar10;
  uint32_t uVar11;
  uint32_t uVar12;
  uint32_t uVar13;
  uint32_t uVar14;
  uint32_t uVar15;
  uint32_t uVar16;
  uint32_t uVar17;
  uint32_t uVar18;
  uint32_t uVar19;
  uint32_t uVar20;
  uint32_t uVar21;
  uint32_t uVar22;
  uint32_t uVar23;
  uint32_t uVar24;
  uint32_t uVar25;
  uint32_t uVar26;
  uint32_t uVar27;
  uint32_t uVar28;
  uint32_t uVar29;
  uint32_t uVar30;
  uint32_t uVar31;
  uint32_t uVar32;
  uint32_t uVar33;
  uint32_t uVar34;
  uint32_t uVar35;
  uint32_t uVar36;
  uint32_t uVar37;
  uint32_t uVar38;
  uint32_t uVar39;
  uint32_t uVar40;
  uint32_t uVar41;
  uint32_t uVar42;
  uint32_t uVar43;
  uint32_t uVar44;
  uint32_t uVar45;
  uint32_t uVar46;
  uint32_t uVar47;
  uint32_t uVar48;
  uint32_t uVar49;
  uint32_t uVar50;
  uint32_t uVar52;
  uint32_t uVar53;
  uint32_t uVar54;
  /* Build the work-dependent state. */
  const uint8_t *const input_byte1_table = const_data + 123824;
  const uint8_t *const input_byte0_table = const_data + 177360;
  uVar10 = input_byte0_table[(work[0xc] ^ 0x76)] + 0x4b ^ 0x7b;
  uVar5 = uVar10 & 0xff;
  const uint8_t *const input_byte3_table = const_data + 209440;
  const uint8_t *const input_byte2_table = const_data + 180784;
  uVar4 = ((uint32_t)input_byte0_table[(work[0] ^ 0x9c)] << 0x18 | input_byte3_table[(work[3] ^ 0x47)] ^ 0xd6 | (input_byte2_table[(work[2] ^ 0xac)] ^ 0xec) << 8 | (input_byte1_table[(work[1] ^ 0x4a)] ^ 0xa6) << 0x10) + 0x4b000000;
  uVar3 = (input_byte1_table[(work[9] ^ 0xe6)] ^ 0x72) << 0x10 | (input_byte2_table[(work[10] ^ 0xa7)] ^ 6) << 8 | input_byte3_table[(work[0xb] ^ 0xfb)] ^ 0x75 | ((uint32_t)input_byte0_table[(work[8] ^ 0xa8)] + 0x4b ^ 0x28) << 0x18;
  uVar52 = (uVar3 ^ 0x786633df);
  uVar6 = uVar10 << 0x18 | (input_byte1_table[(work[0xd] ^ 0x84)] ^ 0xbd) << 0x10 | input_byte3_table[(work[0xf] ^ 8)] ^ 0x40 | (input_byte2_table[(work[0xe] ^ 0xe3)] ^ 0xe8) << 8;
  uVar3 = (input_byte1_table[(work[5] ^ 0xd1)] ^ 0x65) << 0x10 | (input_byte2_table[(work[6] ^ 0x40)] ^ 0x79) << 8 | ((uint32_t)input_byte0_table[(work[4] ^ 0xe0)] + 0x4b ^ 0xe8) << 0x18 | input_byte3_table[(work[7] ^ 0x3f)] ^ 0x28;
  uVar10 = (uVar3 ^ 0x697720c);
  uVar3 = (((uVar6 ^ 0x5b80b964) & 0xff) ^ 0xa7) + 0x1f264300;
  uVar11 = uVar3 ^ 0xdbbaed5b;
  const uint8_t *const first_low_table = const_data + 152848;
  uVar12 = (uVar11 ^ 0x576) + 0xd4e8000 ^ table_word(first_low_table, uVar3 ^ 0x1f2643b6);
  uVar11 = uVar4 ^ 0x3cfbe2e1;
  const uint8_t *const first_high_table = const_data + 191936;
  uVar3 = uVar5 ^ 0x78;
  uVar3 = uVar4 ^ decode_first_stage_byte(uVar3) ^ table_word(first_high_table, uVar5 ^ 0xc4) ^ uVar12;
  const uint8_t *const first_byte2_table = const_data + 181040;
  const uint8_t *const first_byte1_table = const_data + 153872;
  uVar13 = table_word(first_byte1_table, (input_byte2_table[(work[0xe] ^ 0xe3)] ^ 0xe4)) + 0x82923b6fU ^ table_word(first_byte2_table, (input_byte1_table[(work[0xd] ^ 0x84)] ^ 0xa4)) ^ (uVar3 ^ 0x7eab6905);
  uVar14 = uVar10 ^ uVar13 ^ 0x654f64ea ^ 0x33a1f418;
  uVar15 = uVar52 ^ uVar14 ^ 0x72c8bd2a;
  uVar16 = uVar6 ^ uVar15;
  uVar17 = uVar16 ^ 0x134f22c8;
  uVar3 = uVar17 & 0xff ^ 0xc49cae69;
  uVar3 = (uVar3 ^ 0x76) + 0xd4e7d00 ^ table_word(first_low_table, uVar17 & 0xff ^ 0x84);
  uVar3 = (uVar3 ^ 0x7eab6905) ^ table_word(first_byte2_table, uVar17 >> 0x10 & 0xff ^ 0xe5);
  uVar5 = uVar17 >> 0x18 ^ 0x85;
  uVar18 = table_word(first_byte1_table, uVar17 >> 8 & 0xff ^ 0x21) + 0x82923b6fU ^ table_word(first_high_table, uVar17 >> 0x18 ^ 0x39) ^ uVar13 ^ decode_first_stage_byte(uVar5) ^ uVar3;
  uVar19 = uVar18 ^ 0x3f98d223;
  uVar20 = uVar52 ^ uVar19;
  uVar21 = uVar20 ^ uVar17 ^ 0x94592e63;
  uVar3 = uVar21 >> 0x18 ^ 0x77;
  uVar3 = ((table_word(first_byte1_table, uVar21 >> 8 & 0xff ^ 0xdc) + 0x3e921d40U) ^ 0xf398f012) ^ table_word(first_high_table, uVar21 >> 0x18 ^ 0xcb) ^ uVar19 ^ decode_first_stage_byte(uVar3);
  uVar5 = uVar21 & 0xff ^ 0xc49cae02;
  uVar22 = table_word(first_low_table, uVar21 & 0xff ^ 0xef) ^ table_word(first_byte2_table, uVar21 >> 0x10 & 0xff ^ 0xfe) ^ (uVar5 ^ 0x76) + 0xd4e7d00 ^ (uVar3 ^ 0x7eab6905);
  uVar23 = uVar14 ^ uVar19 ^ uVar22 ^ 0x258d9763;
  uVar24 = uVar23 ^ uVar20 ^ 0x4601c465;
  uVar21 = uVar24 ^ uVar21;
  uVar3 = uVar21 >> 0x18 ^ 0x19;
  uVar5 = uVar21 & 0xff ^ 0xc49cae48;
  uVar25 = table_word(first_byte2_table, uVar21 >> 0x10 & 0xff ^ 0x8f) ^ table_word(first_byte1_table, uVar21 >> 8 & 0xff ^ 0x93) + 0x82923b6fU ^ table_word(first_high_table, uVar21 >> 0x18 ^ 0xa5) ^ table_word(first_low_table, uVar21 & 0xff ^ 0xa5) ^ uVar22 ^ (uVar5 ^ 0x76) + 0xd4e7d00 ^ decode_first_stage_byte(uVar3);
  uVar26 = uVar25 ^ 0x7eab6905;
  uVar27 = uVar26 ^ uVar20 ^ 0x4601c465;
  uVar28 = uVar27 ^ uVar21 ^ 0xcbb1f184;
  uVar3 = uVar28 & 0xff;
  uVar4 = (uVar3 ^ 0xc49caefb) ^ (table_word(first_low_table, uVar3 ^ 0x16) ^ 0x15778576);
  uVar3 = table_word(first_byte1_table, uVar28 >> 8 & 0xff ^ 0xc2) + 0x82923b6fU ^ uVar26 ^ uVar4;
  uVar5 = uVar28 >> 0x18 ^ 0xe9;
  uVar29 = table_word(first_high_table, uVar28 >> 0x18 ^ 0x55) ^ table_word(first_byte2_table, uVar28 >> 0x10 & 0xff ^ 0xb2) ^ decode_first_stage_byte(uVar5) ^ (uVar3 ^ 0x7eab6905);
  uVar30 = uVar29 ^ uVar26 ^ uVar23;
  uVar31 = uVar27 ^ uVar30 ^ 0x35055031;
  uVar32 = uVar31 ^ uVar28;
  uVar3 = uVar32 & 0xff;
  uVar3 = (uVar3 ^ 0x68) + 0x15778500 ^ table_word(first_low_table, uVar3 ^ 0x32);
  uVar3 = table_word(first_byte1_table, uVar32 >> 8 & 0xff ^ 0x73) + 0x82923b6fU ^ uVar29 ^ (uVar3 ^ 0x4fd1f582);
  uVar5 = uVar3 ^ 0xde575bdd;
  uVar12 = table_word(first_byte2_table, uVar32 >> 0x10 & 0xff ^ 0x40) ^ 0x7eab6905;
  uVar4 = uVar12 - (table_word(first_byte2_table, uVar32 >> 0x10 & 0xff ^ 0x40) ^ uVar5 ^ 0xba37c7c4);
  uVar33 = uVar32 >> 0x18 ^ 0x36;
  uVar34 = decode_first_stage_byte(uVar33) ^ table_word(first_high_table, uVar32 >> 0x18 ^ 0x8a) ^ (uVar5 ^ -(uVar3 ^ 0x1acbf51c) ^ (uVar4 ^ 0x47dd682a) ^ 0x8341c6eb) + uVar12;
  uVar3 = uVar34 ^ 0x5e2bc04c;
  uVar35 = uVar30 ^ uVar3;
  uVar36 = uVar35 ^ 0x8d84a556;
  uVar28 = uVar36 ^ uVar28;
  uVar5 = uVar28 & 0xff ^ 0xc49caec6;
  uVar5 = (uVar5 ^ 0x76) + 0xd4e7d00 ^ table_word(first_low_table, uVar28 & 0xff ^ 0x2b);
  uVar12 = uVar28 >> 0x18 ^ 0x4c;
  uVar37 = table_word(first_high_table, uVar28 >> 0x18 ^ 0xf0) ^ table_word(first_byte2_table, uVar28 >> 0x10 & 0xff ^ 0x2f) ^ (table_word(first_byte1_table, ((uVar28 >> 8 ^ 0xffffffff) & 0xff)) + 0x82923b6fU) ^ uVar3 ^ decode_first_stage_byte(uVar12) ^ (uVar5 ^ 0x5e2bc04c);
  uVar31 = uVar36 ^ uVar31;
  uVar5 = uVar37 ^ 0xa8938483;
  uVar36 = uVar36 ^ uVar5;
  uVar38 = uVar36 ^ 0x5c685d36;
  uVar39 = uVar38 ^ uVar31;
  uVar40 = uVar39 ^ uVar28;
  uVar3 = uVar40 >> 0x18 ^ 0xf4;
  uVar3 = decode_first_stage_byte(uVar3) ^ table_word(first_high_table, uVar40 >> 0x18 ^ 0x48);
  uVar5 = (uVar3 ^ 0x3b38ed86) ^ uVar5;
  uVar3 = (((uVar40 ^ 0xef68065b) & 0xff) ^ 0x36) + 0x3a312300;
  uVar33 = table_word(first_low_table, uVar40 & 0xff ^ 0xbf);
  uVar12 = uVar3 ^ 0xfead8d3f;
  uVar3 = uVar12 - (uVar33 ^ uVar3 ^ 0xebda0849) ^ -(uVar33 ^ 0x15778576);
  uVar41 = table_word(first_byte1_table, uVar40 >> 8 & 0xff ^ 0xfe) + 0x82923b6fU ^ table_word(first_byte2_table, uVar40 >> 0x10 & 0xff ^ 0x8d) ^ ((uVar3 ^ 0x15778576) ^ uVar33) + uVar12 ^ (uVar5 ^ 0x7eab6905);
  uVar42 = uVar41 ^ uVar31;
  uVar43 = uVar42 ^ uVar40 ^ 0x6e5ac8d9;
  uVar3 = uVar43 & 0xff ^ 0xc49cae97;
  uVar3 = table_word(first_low_table, uVar43 & 0xff ^ 0x7a) ^ table_word(first_byte1_table, uVar43 >> 8 & 0xff ^ 0xa3) + 0x82923b6fU ^ uVar41 ^ (uVar3 ^ 0x76) + 0xd4e7d00 ^ 0xde782d98;
  uVar5 = uVar43 >> 0x18 ^ 0xb0;
  uVar44 = table_word(first_high_table, uVar43 >> 0x18 ^ 0xc) ^ table_word(first_byte2_table, uVar43 >> 0x10 & 0xff ^ 0xf1) ^ decode_first_stage_byte(uVar5) ^ (uVar3 ^ 0x7eab6905);
  uVar38 = uVar41 ^ uVar38;
  uVar45 = uVar38 ^ uVar44 ^ 0xfacd8d8d;
  uVar46 = uVar45 ^ uVar40 ^ 0x6e5ac8d9;
  uVar3 = uVar46 >> 0x18 ^ 0xac;
  uVar3 = decode_first_stage_byte(uVar3) ^ table_word(first_high_table, uVar46 >> 0x18 ^ 0x10);
  uVar5 = table_word(first_byte1_table, uVar46 >> 8 & 0xff ^ 2) + 0x82923b6fU ^ uVar44 ^ uVar3;
  uVar3 = (((uVar46 ^ 0xc1bfc8d8) & 0xff) ^ 0xf3) + 0x44c83100;
  uVar5 = (uVar5 ^ 0x7eab6905) ^ table_word(first_byte2_table, uVar46 >> 0x10 & 0xff ^ 0xe6);
  uVar47 = uVar3 ^ table_word(first_low_table, uVar3 ^ 0x44c831d0) ^ (uVar5 ^ 0x15778576);
  uVar48 = uVar46 ^ uVar47 ^ uVar42 ^ 0x3cccedab;
  uVar49 = uVar47 ^ uVar42 ^ 0xd59b4d21;
  uVar4 = (uVar24 ^ 0x2a09be64) - (uVar13 ^ 0x4f54299);
  uVar3 = (uVar4 ^ 0x58fa2e52);
  uVar4 = (uVar46 ^ 0x2849af33) + (uVar16 ^ 0x487fb4cc);
  uVar54 = (uVar4 + 0x33b086f2U) ^ 0x3a846044;
  uVar5 = (input_byte1_table[(constant[13] ^ 0xc)] ^ 7) << 0x10 | (input_byte2_table[(constant[14] ^ 0x9c)] ^ 0x95) << 8 | input_byte3_table[(constant[15] ^ 0x20)] ^ 0x71 | ((uint32_t)input_byte0_table[(constant[12] ^ 0x54)] + 0x4b ^ 0xca) << 0x18;
  uVar12 = (input_byte2_table[(constant[2] ^ 0x4c)] ^ 0xa1) << 8 | (input_byte1_table[(constant[1] ^ 0x1f)] ^ 0x25) << 0x10 | input_byte3_table[(constant[3] ^ 0x9f)] ^ 0x73 | ((uint32_t)input_byte0_table[(constant[0] ^ 0x52)] + 0x4b ^ 0xe5) << 0x18;
  uVar33 = ((uint32_t)input_byte0_table[(constant[4] ^ 0x71)] + 0x4b ^ 0x51) << 0x18 | (input_byte2_table[(constant[6] ^ 0xc7)] ^ 0xd0) << 8 | input_byte3_table[(constant[7] ^ 0x89)] ^ 0x79 | (input_byte1_table[(constant[5] ^ 0xf)] ^ 0xfb) << 0x10;
  uVar50 = uVar3 ^ 0xeab3aab1;
  /* Mix source and constant into the work-dependent state. */
  const uint8_t *const source_byte0_table = const_data + 202112;
  const uint8_t *const source_byte2_table = const_data + 126160;
  const uint8_t *const source_byte1_table = const_data + 156256;
  const uint8_t *const source_byte3_table = const_data + 172224;
  uVar7 = ((uint32_t)source_byte0_table[(source[8] ^ 0x53)] ^ 0xc6) << 0x18 | (source_byte1_table[(source[9] ^ 0x5d)] ^ 0xc6) << 0x10 | source_byte3_table[(source[11] ^ 0x41)] ^ 0x5f | (source_byte2_table[(source[10] ^ 0x37)] ^ 0xf2) << 8;
  uVar8 = ((uint32_t)source_byte0_table[(source[12] ^ 0x32)] ^ 0x78) << 0x18 | (source_byte2_table[(source[14] ^ 0x95)] ^ 0xd5) << 8 | source_byte3_table[(source[15] ^ 0xcd)] ^ 0x30 | (source_byte1_table[(source[13] ^ 0x71)] ^ 0x1f) << 0x10;
  uVar9 = (source_byte1_table[(source[5] ^ 0xe4)] ^ 0xbf) << 0x10 | ((uint32_t)source_byte0_table[(source[4] ^ 0x5c)] ^ 0xb3) << 0x18 | source_byte3_table[(source[7] ^ 0xe6)] ^ 0xaa | (source_byte2_table[(source[6] ^ 0xc5)] ^ 0x4c) << 8;
  uVar53 = ((uint32_t)source_byte0_table[(source[0] ^ 0x22)] ^ 0x2b) << 0x18 | (source_byte2_table[(source[2] ^ 0xae)] ^ 0x6a) << 8 | (source_byte1_table[(source[1] ^ 0x97)] ^ 0xf6) << 0x10 | source_byte3_table[(source[3] ^ 0x9d)] ^ 0xd5;
  uVar52 = uVar52 ^ uVar7 ^ 0xba60a927 ^ (((uint32_t)input_byte0_table[(constant[8] ^ 0x76)] + 0x4b ^ 0xa7) << 0x18 | input_byte3_table[(constant[11] ^ 0x7b)] ^ 0xd2 | (input_byte2_table[(constant[10] ^ 0x61)] ^ 0x9e) << 8 | (input_byte1_table[(constant[9] ^ 0xf9)] ^ 0xad) << 0x10);
  uVar5 = uVar6 ^ uVar5 ^ uVar8 ^ 0x64effffb;
  uVar11 = uVar3 ^ uVar11 ^ uVar12 ^ uVar53;
  uVar10 = uVar10 ^ uVar3 ^ uVar33 ^ uVar9 ^ 0x67ee5dfb;
  uVar7 = uVar11 ^ uVar3 ^ 0x59ba3917;
  uVar10 = (uVar10 ^ 0x56031cbe) ^ uVar50;
  const uint8_t *const middle_high_table = const_data + 178736;
  const uint8_t *const middle_low_table = const_data + 206048;
  uVar11 = table_word(middle_low_table, uVar5 & 0xff ^ 199) + 0xce853ebaU ^ table_word(middle_high_table, uVar7 >> 0x18 ^ 0x2c) ^ (uVar7 >> 0x18 ^ 0x33c08cbd) + 0xd0fd5400;
  const uint8_t *const middle_byte1_table = const_data + 200544;
  uVar12 = table_word(middle_low_table, uVar7 & 0xff ^ 0xd8) + 0xce853ebaU ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0x68) ^ (uVar10 >> 0x18 ^ 0x33c08cf9) + 0xd0fd5400;
  uVar12 = (uVar12 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar5 >> 8 & 0xff ^ 0xa5);
  const uint8_t *const middle_byte2_table = const_data + 199504;
  uVar11 = (uVar11 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0x55);
  uVar6 = mix_table_word_a(table_word(middle_low_table, uVar52 & 0xff ^ 0xa5)) ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0xb4);
  uVar33 = uVar52 >> 0x18 ^ 0x33c08c9c;
  uVar33 = table_word(middle_high_table, uVar52 >> 0x18 ^ 0x8c) ^ (uVar33 ^ 0x81) + 0xd0fd5400;
  uVar6 = (uVar5 >> 0x18 ^ 0x33c08ccb) + 0xd0fd5400 ^ table_word(middle_byte2_table, uVar7 >> 0x10 & 0xff ^ 0x26) ^ table_word(middle_high_table, uVar5 >> 0x18 ^ 0x5a) ^ (uVar6 ^ 0xe4b91e3);
  uVar10 = (uVar33 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar7 >> 8 & 0xff ^ 0x98) ^ table_word(middle_low_table, uVar10 & 0xff ^ 0x9a) + 0xce853ebaU;
  uVar11 = (uVar11 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0x3e);
  uVar10 = uVar15 ^ 0xb7acceb3 ^ table_word(middle_byte2_table, uVar5 >> 0x10 & 0xff ^ 0xea) ^ uVar10;
  uVar5 = uVar16 ^ 0x6bff3f42 ^ (uVar6 ^ 0x23808b8e);
  uVar52 = uVar14 ^ 0xcea7dbc5 ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0x59) ^ uVar12;
  uVar12 = mix_table_word_b(table_word(middle_low_table, uVar5 & 0xff ^ 0xa7)) ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0x34);
  uVar6 = uVar10 >> 0x18 ^ 0x33c08c17;
  uVar6 = (uVar6 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 7);
  uVar11 = uVar13 ^ 0x5116330 ^ (uVar11 ^ 0x1e421a9);
  uVar33 = table_word(middle_low_table, uVar11 & 0xff ^ 0x19) + 0xce853ebaU ^ table_word(middle_high_table, uVar52 >> 0x18 ^ 0x4a) ^ (uVar52 >> 0x18 ^ 0x33c08cdb) + 0xd0fd5400;
  uVar33 = (uVar33 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar5 >> 8 & 0xff ^ 0x68);
  uVar7 = uVar5 >> 0x18 ^ 0x33c08cf4;
  uVar7 = (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar5 >> 0x18 ^ 0xe4);
  uVar5 = (uVar6 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar5 >> 0x10 & 0xff ^ 0x6e);
  uVar6 = table_word(middle_low_table, uVar10 & 0xff ^ 0xb6) + 0xce853ebaU ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0x94) ^ (uVar7 ^ 0x4eb7d448);
  uVar7 = uVar11 >> 0x18 ^ 0x33c08c89;
  uVar12 = uVar18 ^ 0x964f96f7 ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0x8a) ^ (uVar12 ^ 0x4eb7d448) ^ (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar11 >> 0x18 ^ 0x99);
  uVar10 = uVar14 ^ uVar19 ^ 0xf30b7afb ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0xe2) ^ (uVar33 ^ 0xe4b91e3);
  uVar52 = uVar20 ^ 0x1adb87e7 ^ table_word(middle_byte1_table, uVar11 >> 8 & 0xff ^ 0x36) ^ table_word(middle_low_table, uVar52 & 0xff ^ 0x9b) + 0xce853ebaU ^ (uVar5 ^ 0x4eb7d448);
  uVar11 = uVar20 ^ uVar17 ^ 0x318b8018 ^ table_word(middle_byte2_table, uVar11 >> 0x10 & 0xff ^ 0xc5) ^ (uVar6 ^ 0xe4b91e3);
  uVar5 = mix_table_word_a(table_word(middle_low_table, uVar10 & 0xff ^ 0x3f)) ^ table_word(middle_byte1_table, uVar12 >> 8 & 0xff ^ 0xfb);
  uVar5 = table_word(middle_byte2_table, uVar11 >> 0x10 & 0xff ^ 0x97) ^ 0x377d6cee ^ table_word(middle_high_table, uVar52 >> 0x18 ^ 0x2c) ^ uVar52 >> 0x18 ^ (uVar5 ^ 0xe4b91e3);
  uVar6 = uVar10 >> 0x18 ^ 0x377d6c9b ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0x59) ;
  uVar6 = (uVar6 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0x2f);
  uVar33 = mix_table_word_b(table_word(middle_low_table, uVar11 & 0xff ^ 0xb3)) ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0x84);
  uVar7 = uVar11 >> 0x18 ^ 0x33c08cf8;
  uVar8 = uVar12 >> 0x18 ^ 0x33c08cc9;
  uVar10 = uVar21 ^ 0x90ae848f ^ table_word(middle_byte2_table, uVar12 >> 0x10 & 0xff ^ 0x36) ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0xc1) ^ (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar11 >> 0x18 ^ 0xe8) ^ table_word(middle_low_table, uVar52 & 0xff ^ 0x30) + 0xce853ebaU;
  uVar5 = uVar24 ^ 0x626932c5 ^ (uVar5 ^ 0x48608ca1);
  uVar11 = table_word(middle_byte1_table, uVar11 >> 8 & 0xff ^ 0xad) ^ uVar23 ^ table_word(middle_low_table, uVar12 & 0xff ^ 0x72) + 0xce853ebaU ^ (uVar6 ^ 0x4eb7d448);
  uVar52 = uVar22 ^ 0x45c73e3d ^ (uVar8 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0x8e) ^ table_word(middle_high_table, uVar12 >> 0x18 ^ 0xd9) ^ (uVar33 ^ 0x4eb7d448);
  uVar12 = mix_table_word_b(table_word(middle_low_table, uVar11 & 0xff ^ 0xab)) ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0x66);
  uVar6 = uVar11 >> 0x18 ^ 0x33c08c3d;
  uVar6 = table_word(middle_high_table, uVar11 >> 0x18 ^ 0x2d) ^ table_word(middle_byte2_table, uVar5 >> 0x10 & 0xff ^ 0xe) ^ mix_table_word_b(table_word(middle_low_table, uVar52 & 0xff ^ 0xbd)) ^ (uVar6 ^ 0x81) + 0xd0fd5400;
  uVar33 = uVar52 >> 0x18 ^ 0xe4b9124 ^ table_word(middle_byte2_table, uVar11 >> 0x10 & 0xff ^ 0x44);
  uVar33 = (uVar33 ^ 0x377d6c81) ^ table_word(middle_high_table, uVar52 >> 0x18 ^ 0x84);
  uVar7 = uVar10 >> 0x18 ^ 0x33c08ce3;
  uVar7 = (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0xf3) ^ table_word(middle_low_table, uVar5 & 0xff ^ 0xa2) + 0xce853ebaU;
  uVar11 = (uVar7 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar11 >> 8 & 0xff ^ 0xe);
  uVar7 = uVar5 >> 0x18 ^ 0x33c08c37;
  uVar12 = uVar27 ^ 0xdacebdbd ^ (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0x4a) ^ (uVar12 ^ 0x4eb7d448) ^ table_word(middle_high_table, uVar5 >> 0x18 ^ 0x27);
  uVar5 = uVar25 ^ 0x348b8bca ^ table_word(middle_byte1_table, uVar5 >> 8 & 0xff ^ 0xa7) ^ table_word(middle_low_table, uVar10 & 0xff ^ 0xa6) + 0xce853ebaU ^ (uVar33 ^ 0x4eb7d448);
  uVar52 = uVar27 ^ uVar21 ^ 0x249841 ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0xf7) ^ (uVar11 ^ 0xe4b91e3);
  uVar10 = uVar26 ^ uVar23 ^ 0x3d1e55f7 ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0x8d) ^ (uVar6 ^ 0x4eb7d448);
  uVar11 = mix_table_word_a(table_word(middle_low_table, uVar10 & 0xff ^ 0x55)) ^ table_word(middle_byte1_table, uVar5 >> 8 & 0xff ^ 3);
  uVar6 = (uVar10 >> 0x18 ^ 0x33c08ca9) + 0xd0fd5400 ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0x38) ^ table_word(middle_low_table, uVar5 & 0xff ^ 0x19) + 0xce853ebaU;
  uVar6 = (uVar6 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar12 >> 0x10 & 0xff ^ 0xf4);
  uVar33 = mix_table_word_a(table_word(middle_low_table, uVar12 & 0xff ^ 0x55)) ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0x60) ^ table_word(middle_high_table, uVar52 >> 0x18 ^ 0x18) ^ (uVar52 >> 0x18 ^ 0x33c08c89) + 0xd0fd5400;
  uVar10 = table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0x55) ^ (uVar5 >> 0x18 ^ 0x33c08ca3) + 0xd0fd5400 ^ mix_table_word_b(table_word(middle_low_table, uVar52 & 0xff ^ 0xf4)) ^ table_word(middle_high_table, uVar5 >> 0x18 ^ 0x32);
  uVar13 = (uVar10 ^ 0x4eb7d448);
  uVar10 = table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0x9c) ^ (uVar11 ^ 0xe4b91e3);
  uVar11 = uVar32 ^ 0x6717c8f5 ^ table_word(middle_byte2_table, uVar5 >> 0x10 & 0xff ^ 0x99) ^ (uVar33 ^ 0xe4b91e3);
  uVar10 = uVar27 ^ uVar30 ^ 0x20c9a3b4 ^ uVar12 >> 0x18 ^ table_word(middle_high_table, uVar12 >> 0x18 ^ 0xb1) ^ (uVar10 ^ 0x377d6c81);
  uVar52 = uVar30 ^ 0x3eea361e ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0x74) ^ (uVar6 ^ 0x4eb7d448);
  uVar5 = uVar29 ^ 0x87746b7f ^ table_word(middle_byte1_table, uVar12 >> 8 & 0xff ^ 199);
  uVar12 = uVar5 ^ uVar13;
  uVar6 = uVar10 >> 0x18 ^ 0x33c08cb9;
  uVar6 = (uVar6 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0xa9);
  uVar33 = uVar52 >> 0x18 ^ 0x33c08caf;
  uVar33 = (uVar33 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar52 >> 0x18 ^ 0xbf);
  uVar33 = (uVar33 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0x48);
  uVar7 = uVar12 >> 0x18 ^ 0x33c08c43;
  uVar6 = table_word(middle_low_table, uVar52 & 0xff ^ 0xe3) + 0xce853ebaU ^ table_word(middle_byte1_table, uVar12 >> 8 & 0xff ^ 0x1f) ^ (uVar6 ^ 0x4eb7d448);
  uVar6 = uVar31 ^ 0x50321738 ^ table_word(middle_byte2_table, uVar11 >> 0x10 & 0xff ^ 0xec) ^ (uVar6 ^ 0xe4b91e3);
  uVar8 = uVar28 ^ 0x13e1fc0c ^ mix_table_word_a(table_word(middle_low_table, uVar10 & 0xff ^ 0xdf)) ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0xad);
  uVar9 = uVar11 >> 0x18 ^ 0x33c08c7b;
  uVar52 = uVar34 ^ 0x8252f3da ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0x14) ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0x27) ^ table_word(middle_high_table, uVar12 >> 0x18 ^ 0x53) ^ (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_low_table, uVar11 & 0xff ^ 0xe0) + 0xce853ebaU;
  uVar10 = (uVar8 ^ 0xe4b91e3) ^ (uVar9 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar11 >> 0x18 ^ 0x6b) ^ table_word(middle_byte2_table, uVar12 >> 0x10 & 0xff ^ 0x44);
  uVar11 = uVar35 ^ 0xf300959b ^ table_word(middle_byte1_table, uVar11 >> 8 & 0xff ^ 100) ^ table_word(middle_low_table, ((uVar5 ^ uVar13 ^ 0xffffffff) & 0xff)) + 0xce853ebaU ^ (uVar33 ^ 0x4eb7d448);
  uVar5 = uVar11 >> 0x18 ^ 0x33c08c9b;
  uVar5 = (uVar5 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar11 >> 0x18 ^ 0x8b);
  uVar12 = mix_table_word_a(table_word(middle_low_table, uVar10 & 0xff)) ^ table_word(middle_byte1_table, uVar6 >> 8 & 0xff ^ 0x41);
  uVar33 = uVar6 >> 0x18 ^ 0x377d6cb8 ^ table_word(middle_high_table, uVar6 >> 0x18 ^ 0x7a);
  uVar33 = (uVar33 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0x75);
  uVar5 = (uVar5 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar6 >> 0x10 & 0xff ^ 0xeb);
  uVar7 = uVar52 >> 0x18 ^ 0x33c08c25;
  uVar8 = uVar10 >> 0x18 ^ 0x33c08c75;
  uVar5 = uVar36 ^ 0x215e2ccb ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0xdc) ^ (uVar5 ^ 0x4eb7d448) ^ table_word(middle_low_table, uVar52 & 0xff ^ 0xab) + 0xce853ebaU;
  uVar10 = uVar40 ^ 0x1f432ca8 ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0x51) ^ table_word(middle_byte1_table, uVar11 >> 8 & 0xff ^ 0xd7) ^ (uVar8 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0x65) ^ table_word(middle_low_table, uVar6 & 0xff ^ 0xfe) + 0xce853ebaU;
  uVar6 = uVar39 ^ 0x7a337c23 ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0x75) ^ (uVar33 ^ 0x4eb7d448) ^ table_word(middle_low_table, uVar11 & 0xff ^ 0x2f) + 0xce853ebaU;
  uVar52 = uVar37 ^ 0xd81f3004 ^ table_word(middle_byte2_table, uVar11 >> 0x10 & 0xff ^ 0xaf) ^ (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar52 >> 0x18 ^ 0x35) ^ (uVar12 ^ 0xe4b91e3);
  uVar11 = (uVar10 >> 0x18 ^ 0x33c08c00) + 0xd0fd5400 ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0x91);
  uVar11 = (uVar11 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar5 >> 8 & 0xff ^ 0x26);
  uVar12 = uVar52 >> 0x18 ^ 0x33c08c0b;
  uVar12 = table_word(middle_low_table, uVar10 & 0xff ^ 0x9b) + 0xce853ebaU ^ (uVar12 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_high_table, uVar52 >> 0x18 ^ 0x1b);
  uVar12 = (uVar12 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar5 >> 0x10 & 0xff ^ 0x4d);
  uVar33 = uVar6 >> 0x18 ^ 0x377d6c63 ^ mix_table_word_a(table_word(middle_low_table, uVar5 & 0xff ^ 0xaf)) ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0x83) ^ table_word(middle_high_table, uVar6 >> 0x18 ^ 0xa1);
  uVar7 = uVar5 >> 0x18 ^ 0x33c08cff;
  uVar5 = (uVar7 ^ 0x81) + 0xd0fd5400 ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0x48) ^ mix_table_word_a(table_word(middle_low_table, uVar52 & 0xff ^ 0xde)) ^ table_word(middle_high_table, uVar5 >> 0x18 ^ 0xef);
  uVar10 = uVar42 ^ 0x5aa762b4 ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0x89) ^ (uVar33 ^ 0xe4b91e3);
  uVar52 = uVar43 ^ 0x8780aca1 ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 2) ^ (uVar11 ^ 0xe4b91e3) ^ table_word(middle_low_table, uVar6 & 0xff ^ 0x3b) + 0xce853ebaU;
  uVar11 = uVar41 ^ 0x9a463139 ^ (uVar12 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar6 >> 8 & 0xff ^ 0x2b);
  uVar5 = uVar38 ^ 0x8fa0bf20 ^ table_word(middle_byte2_table, uVar6 >> 0x10 & 0xff ^ 0xe7) ^ (uVar5 ^ 0xe4b91e3);
  uVar12 = mix_table_word_b(table_word(middle_low_table, uVar5 & 0xff ^ 0x77)) ^ table_word(middle_byte2_table, uVar52 >> 0x10 & 0xff ^ 0xca);
  uVar6 = uVar11 >> 0x18 ^ 0x33c08c8a;
  uVar6 = table_word(middle_high_table, uVar11 >> 0x18 ^ 0x9a) ^ table_word(middle_byte2_table, uVar5 >> 0x10 & 0xff ^ 0x92) ^ mix_table_word_b(table_word(middle_low_table, uVar52 & 0xff ^ 0xfc)) ^ (uVar6 ^ 0x81) + 0xd0fd5400;
  uVar33 = uVar52 >> 0x18 ^ 0x33c08c1d;
  uVar33 = table_word(middle_high_table, uVar52 >> 0x18 ^ 0xd) ^ (uVar33 ^ 0x81) + 0xd0fd5400;
  uVar12 = uVar10 >> 0x18 ^ 0x33c08c03 ^ table_word(middle_high_table, uVar10 >> 0x18 ^ 0x13) ^ (uVar12 ^ 0x377d6c81);
  uVar7 = uVar5 >> 0x18 ^ 0x33c08c5d;
  uVar7 = table_word(middle_high_table, uVar5 >> 0x18 ^ 0x4d) ^ (uVar7 ^ 0x81) + 0xd0fd5400;
  uVar33 = (uVar33 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar11 >> 0x10 & 0xff ^ 0xb9) ^ table_word(middle_low_table, uVar10 & 0xff ^ 0x24) + 0xce853ebaU;
  uVar12 = uVar42 ^ uVar45 ^ uVar54 ^ table_word(middle_byte1_table, uVar11 >> 8 & 0xff ^ 0xf2) ^ (uVar12 ^ 0x4eb7d448) ^ 0x86e37f37;
  uVar5 = (uVar33 ^ 0x4eb7d448) ^ table_word(middle_byte1_table, uVar5 >> 8 & 0xff ^ 0xf1);
  uVar11 = (uVar7 ^ 0xe4b91e3) ^ table_word(middle_byte2_table, uVar10 >> 0x10 & 0xff ^ 0xf7) ^ table_word(middle_low_table, uVar11 & 0xff ^ 0xe3) + 0xce853ebaU;
  uVar52 = uVar3 ^ uVar45 ^ table_word(middle_byte1_table, uVar52 >> 8 & 0xff ^ 0xd0) ^ (uVar11 ^ 0x4eb7d448) ^ 0x4fd66f27;
  uVar52 = (uVar52 ^ 0x56031cbe) ^ uVar50;
  uVar11 = uVar46 ^ 0x525cd2bd ^ (uVar5 ^ 0x7a157d8e);
  uVar3 = uVar3 ^ uVar44 ^ 0x4f5c5bdc ^ table_word(middle_byte1_table, uVar10 >> 8 & 0xff ^ 0xa1) ^ (uVar6 ^ 0x4eb7d448);
  uVar50 = (uVar3 ^ 0x56031cbe) ^ uVar50;
  uVar54 = (uVar12 ^ 0x457b9fbb) ^ uVar54;
  const uint8_t *const final_byte1_table = const_data + 148192;
  const uint8_t *const final_high_table = const_data + 183344;
  const uint8_t *const final_low_table = const_data + 185392;
  uVar3 = table_word(final_high_table, uVar52 >> 0x18 ^ 0xf) ^ 0x6e4f6830 ^ table_word(final_low_table, uVar50 & 0xff ^ 0x7a);
  const uint8_t *const final_byte2_table = const_data + 136112;
  uVar10 = mix_table_word_c(table_word(final_byte2_table, uVar11 >> 0x10 & 0xff ^ 0xee)) ^ table_word(final_byte1_table, uVar50 >> 8 & 0xff ^ 0x4c);
  uVar10 = (uVar10 ^ 0x1f226eb6) ^ table_word(final_low_table, uVar52 & 0xff ^ 0x84);
  uVar5 = mix_table_word_c(table_word(final_byte2_table, uVar52 >> 0x10 & 0xff ^ 0x1b)) ^ table_word(final_byte1_table, uVar54 >> 8 & 0xff ^ 0xcd);
  uVar5 = (uVar5 ^ 0x1f226eb6) ^ table_word(final_low_table, uVar11 & 0xff ^ 1);
  uVar12 = table_word(final_low_table, uVar54 & 0xff ^ 0x3c) ^ 0x6e4f6830 ^ table_word(final_high_table, uVar11 >> 0x18 ^ 0x24) ^ table_word(final_byte2_table, uVar50 >> 0x10 & 0xff ^ 0xd8) + 0x2156722cU;
  uVar5 = uVar47 ^ 0x95821f3f ^ table_word(final_high_table, uVar50 >> 0x18 ^ 0x67) ^ (uVar5 ^ 0x716d0686);
  uVar10 = (uVar49 ^ 0x56031cbe) ^ 0xe8b64565 ^ table_word(final_high_table, uVar54 >> 0x18 ^ 0x15) ^ (uVar10 ^ 0x716d0686);
  uVar3 = uVar45 ^ uVar47 ^ 0xc0df89ef ^ (uVar3 ^ 0x70caa0b8) ^ table_word(final_byte1_table, uVar11 >> 8 & 0xff ^ 0x24) ^ table_word(final_byte2_table, uVar54 >> 0x10 & 0xff ^ 0xc3) + 0x2156722cU;
  uVar52 = (uVar48 ^ 0x56031cbe) ^ 0x4b867b29 ^ table_word(final_byte1_table, uVar52 >> 8 & 0xff ^ 0x61) ^ (uVar12 ^ 0x70caa0b8);
  /* Decode the four 32-bit words into output bytes. */
  output[0] = decode_output_byte3((uint8_t)(uVar5 >> 24), 0x77, 0x41, 0x7a);
  output[1] = decode_output_byte2((uint8_t)(uVar5 >> 16), 0xc2, 0xb5);
  output[2] = decode_output_byte1((uint8_t)(uVar5 >> 8), 0xf8, 0x6d, 0x80);
  output[3] = decode_output_byte0((uint8_t)uVar5, 0xef, 0x64, 0xf5);
  output[4] = decode_output_byte3((uint8_t)(uVar3 >> 24), 0x1e, 0x28, 0xa6);
  output[5] = decode_output_byte2((uint8_t)(uVar3 >> 16), 0x2c, 0xc8);
  output[6] = decode_output_byte1((uint8_t)(uVar3 >> 8), 0x80, 0x15, 0x03);
  output[7] = decode_output_byte0((uint8_t)uVar3, 0xee, 0x65, 0x3b);
  output[8] = decode_output_byte3((uint8_t)(uVar10 >> 24), 0xe8, 0xde, 0x7d);
  output[9] = decode_output_byte2((uint8_t)(uVar10 >> 16), 0x2e, 0xb1);
  output[10] = decode_output_byte1((uint8_t)(uVar10 >> 8), 0xcb, 0x5e, 0x2b);
  output[11] = decode_output_byte0((uint8_t)uVar10, 0x6a, 0xe1, 0xbe);
  output[12] = decode_output_byte3((uint8_t)(uVar52 >> 24), 0x6c, 0x5a, 0xb8);
  output[13] = decode_output_byte2((uint8_t)(uVar52 >> 16), 0x1d, 0x90);
  output[14] = decode_output_byte1((uint8_t)(uVar52 >> 8), 0x0a, 0x9f, 0xdd);
  output[15] = decode_output_byte0((uint8_t)uVar52, 0xf8, 0x73, 0x74);
}

static int from_hex(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    }

    if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }

    if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    }

    return -1;
}

static void parse16(const char *text, uint8_t out[16]) {
    if (strlen(text) != 32) {
        fprintf(stderr, "expected 32 hex chars\n");
        exit(2);
    }

    for (int i = 0; i < 16; i++) {
        int hi = from_hex(text[i * 2]), lo = from_hex(text[i * 2 + 1]);
        if (hi < 0 || lo < 0) {
            fprintf(stderr, "bad hex\n");
            exit(2);
        }

        out[i] = (uint8_t)((hi << 4) | lo);
    }
}

static void transform_one(const char *source_hex, const char *work_hex, const char *constant_hex) {
    uint8_t source[16], work[16], constant[16], output[16];
    parse16(source_hex, source);
    parse16(work_hex, work);
    parse16(constant_hex, constant);
    transform_block(source, work, constant, output);

    for (int i = 0; i < 16; i++) {
        printf("%02x", output[i]);
    }

    printf("\n");
    fflush(stdout);
}

int main(int argc, char **argv) {
    bool batch = argc == 3 && strcmp(argv[1], "--batch") == 0;
    if (!batch && argc != 5) {
        fprintf(stderr, "usage: %s const-data.bin source16 work16 constant16\n       %s --batch const-data.bin < requests.txt\n", argv[0], argv[0]);
        return 2;
    }

    FILE *fp = fopen(batch ? argv[2] : argv[1], "rb");
    if (!fp || fread(const_data, 1, sizeof const_data, fp) != sizeof const_data || fgetc(fp) != EOF) {
        fprintf(stderr, "constant data missing or wrong size\n");
        return 2;
    }

    fclose(fp);
    if (batch) {
        char line[128], source[33], work[33], constant[33], extra;
        while (fgets(line, sizeof line, stdin)) {
            if (sscanf(line, "%32s %32s %32s %c", source, work, constant, &extra) != 3) {
                fprintf(stderr, "invalid batch request\n");
                return 2;
            }

            transform_one(source, work, constant);
        }

        if (ferror(stdin)) {
            fprintf(stderr, "batch input failed\n");
            return 2;
        }
    } else {
        transform_one(argv[2], argv[3], argv[4]);
    }

    return 0;
}
