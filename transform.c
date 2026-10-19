/* 16-byte block transform used by decrypt.py. */
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef uint8_t byte;
typedef uint8_t undefined;
typedef uint8_t undefined1;
typedef uint16_t undefined2;
typedef uint32_t undefined4;
typedef uint32_t uint;
typedef unsigned long ulong;
static uint8_t const_data[0x36750];
static uint8_t *tables[0x128];
static const int table_offsets[0x128] = {
  200538, 202127, 200546, 122547, 203686, 193051, -1, 145903,
  116063, 164007, 202378, -1, -1, 192979, -1, -1,
  165146, 128729, 179762, 165075, 182070, 119691, 207073, 189501,
  176066, 180791, 209702, -1, 140478, 137395, 195638, 149223,
  129562, 156527, 144562, 172739, -1, 199515, 116094, 122815,
  137138, 169611, 209978, -1, 205022, 128755, 199254, 193017,
  134503, 131663, 117634, 120963, 145382, 193307, 145118, 145647,
  164034, 176087, -1, 136123, 134814, 169571, 121478, 127703,
  126138, 119439, 129011, 154899, 156235, -1, 126174, 149503,
  -1, 161895, 172490, 180539, 120718, 178707, 117894, 180279,
  171967, 115999, -1, 178739, -1, 177691, -1, 147951,
  156258, 168515, -1, 170683, 177630, -1, 181046, -1,
  187450, 206047, 165110, 206051, 185398, 193563, 121502, 162927,
  201570, 191911, 211514, 198235, 192974, 207107, 157798, 169591,
  134554, 189543, -1, 189513, -1, 204763, 203438, 166479,
  193002, 155191, -1, 129307, 139454, 175043, 144854, -1,
  203392, 165439, -1, 163939, 126422, 172235, -1, 124095,
  182322, 191927, 183354, -1, 127454, 138419, -1, -1,
  208136, 157551, 174018, 121219, 132182, 121531, -1, 166463,
  156212, 212791, -1, 170939, 189570, 211763, -1, 131927,
  153882, -1, 129042, -1, 136086, -1, 204722, 149487,
  188466, 150775, 134250, 189611, 186430, -1, 194598, 203703,
  117130, 144607, 208162, 148195, 210998, -1, 149758, -1,
  -1, -1, 130578, -1, 135070, -1, 117110, 177111,
  184378, 152863, 131906, 172995, 189862, -1, 163983, 125119,
  -1, 118919, 165066, 209451, 151806, 197939, 177366, 191943,
  144586, 145363, 146914, 202083, 152817, 180027, 190894, -1,
  -1, -1, 196922, 154939, 117390, 201827, 168550, 133207,
  196666, 119183, 123826, 208419, 211254, 134528, 197966, 198223,
  167490, 133223, -1, -1, 165182, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, -1, -1,
  -1, -1, -1, -1, -1, -1, 212827, 218706,
  220763, -1, 222039, 213086, 213335, 219742, 213587, 217434,
  213843, 216154, 214879, 212806, 215903, 217174, 217691, 221010
};

void _fp_dh_b61f5b64e1495c6c24e155e10f02a1db(undefined4 *param_1)

{
  long *plVar1;
  byte *pbVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  uint uVar28;
  uint uVar29;
  uint uVar30;
  uint uVar31;
  uint uVar32;
  uint uVar33;
  uint uVar34;
  uint uVar35;
  uint uVar36;
  uint uVar37;
  uint uVar38;
  uint uVar39;
  uint uVar40;
  uint uVar41;
  uint uVar42;
  uint uVar43;
  uint uVar44;
  uint uVar45;
  uint uVar46;
  uint uVar47;
  uint uVar48;
  uint uVar49;
  uint uVar50;
  byte *pbVar51;
  uint uVar52;
  uint uVar53;
  uint uVar54;
  byte bVar55;
  bool bVar56;
  int iVar57;
  int iVar58;
  ulong uVar59;
  long lVar60;
  byte *pbVar61;
  undefined *puVar62;
  long lVar63;
  undefined *puVar64;
  undefined *puVar65;
  undefined *puVar66;
  undefined *puVar67;
  int iVar68;
  undefined *puVar69;
  undefined *puVar70;
  ulong uVar71;
  undefined *puVar72;
  long lVar73;
  
  uVar52 = ((uint)param_1 ^ 0xd0acfb92) * 0x66cb1a07;
  uVar10 = param_1[1] ^ uVar52;
  lVar73 = *(long *)(param_1 + 2);
  pbVar51 = *(byte **)(param_1 + 10);
  pbVar61 = *(byte **)(param_1 + 0xc);
  uVar52 = param_1[0xe] ^ uVar52;
  /* original helper only checks bounds; no side effects */
  plVar1 = (long *)(lVar73 + 0x90);
  lVar60 = ((ulong)((long)plVar1 * 0x4a988620f7f81bb5) % 9) * 0x10;
  *(long *)(lVar73 + 0x90) = lVar60;
  pbVar2 = (byte *)(lVar73 + 0x98);
  iVar68 = uVar52 + 0xf2314dbc;
  uVar3 = (iVar68 - (iVar68 * 2 + 0x413774e0U & 0xb5eae2be)) + 0xfb912bcf;
  puVar62 = tables[(int)(uVar10 + 4)];
  puVar65 = tables[(int)uVar10];
  puVar69 = tables[(int)(uVar10 - 0x5e)];
  iVar58 = (uVar3 ^ 0x804f4be0) + (uVar3 * 2 & 0xb574757e ^ 0xb560603e) + 0x3f6ded6a;
  iVar57 = uVar10 + (uint)(pbVar51 <= pbVar61 && pbVar61 != pbVar51) * 6 +
           (uint)(pbVar51 > pbVar61 || pbVar61 == pbVar51) * -0x16 +
           (uint)(iVar58 == 0x1a282829) * 0x16;
  do {
    uVar71 = (ulong)(iVar58 + 0xe5d7d7d6);
    iVar58 = iVar58 + -1;
    uVar59 = uVar71 & 0xf;
    pbVar51[uVar71] =
         pbVar61[uVar71] ^ puVar69[uVar59 - 6] ^ puVar62[uVar59 - 9] ^ puVar65[uVar59 - 2] ^
         (char)uVar59 * -0x5b ^ 0x23;
    iVar57 = iVar57 + (uint)(iVar58 == 0x1a282829) * 0x16;
  } while (iVar58 != 0x1a282829);
  lVar63 = 0;
  do {
    bVar56 = lVar63 == -0xf;
    *(byte *)(lVar73 + 0xa7 + lVar63) =
         *(byte *)(*(long *)(param_1 + 6) + lVar63 + 0xf) ^ (char)lVar63 * -0x47 - 0x29U ^
         tables[iVar57 + -1][lVar63] ^
         tables[iVar57 + -0x52][lVar63 + 0xb] ^
         tables[iVar57 + 0x36][lVar63 + 0xc] ^ 0x23;
    iVar57 = iVar57 + (uint)bVar56 * -0x17;
    lVar63 = lVar63 + -1;
  } while (!bVar56);
  *(long *)(lVar73 + 0x90) = lVar60 + 0x10;
  pbVar61 = *(byte **)(param_1 + 4);
  uVar10 = (uint)(pbVar51 == (byte *)0x0 || pbVar61 == (byte *)0x0);
  uVar3 = uVar10 ^ 1;
  uVar52 = uVar52 + 0x92cd082c;
  iVar58 = (uVar52 * 2 & 0xb6bd5f6a) + (uVar52 ^ 0xdb5eafb5);
  uVar52 = (uint)(iVar58 == -0x24a1504b || (iVar58 + 0xbU & 0xf) != 0);
  iVar57 = uVar52 * -0x1b + (uVar52 ^ 1) * -0x32 +
           (uVar3 | uVar3 << 1) +
           (uint)(pbVar2 == pbVar51) * 0x29 + (uint)(pbVar2 != pbVar51) * 0x1c + iVar57 +
           uVar10 * -0x18;
  puVar66 = tables[iVar57 + 0xaa];
  puVar67 = tables[iVar57 + 0x96];
  uVar10 = (byte)puVar67[((ulong)pbVar61[0xc] ^ 0x76) - 6] + 0x4b ^ 0x7b;
  uVar5 = uVar10 & 0xff;
  puVar62 = tables[iVar57 + 0x93];
  puVar72 = tables[iVar57 + -0x1f];
  iVar4 = ((uint)(byte)puVar67[((ulong)*pbVar61 ^ 0x9c) - 6] << 0x18 |
          (byte)puVar62[((ulong)pbVar61[3] ^ 0x47) - 0xb] ^ 0xd6 |
          ((byte)puVar72[((ulong)pbVar61[2] ^ 0xac) - 7] ^ 0xec) << 8 |
          ((byte)puVar66[((ulong)pbVar61[1] ^ 0x4a) - 2] ^ 0xa6) << 0x10) + 0x4b000000;
  uVar3 = ((byte)puVar66[((ulong)pbVar61[9] ^ 0xe6) - 2] ^ 0x72) << 0x10 |
          ((byte)puVar72[((ulong)pbVar61[10] ^ 0xa7) - 7] ^ 6) << 8 |
          (byte)puVar62[((ulong)pbVar61[0xb] ^ 0xfb) - 0xb] ^ 0x75 |
          ((byte)puVar67[((ulong)pbVar61[8] ^ 0xa8) - 6] + 0x4b ^ 0x28) << 0x18;
  uVar52 = uVar3 + (uVar3 & 0x786633df) * -2 + 0x786633df;
  uVar6 = uVar10 << 0x18 | ((byte)puVar66[((ulong)pbVar61[0xd] ^ 0x84) - 2] ^ 0xbd) << 0x10 |
          (byte)puVar62[((ulong)pbVar61[0xf] ^ 8) - 0xb] ^ 0x40 |
          ((byte)puVar72[((ulong)pbVar61[0xe] ^ 0xe3) - 7] ^ 0xe8) << 8;
  uVar3 = ((byte)puVar66[((ulong)pbVar61[5] ^ 0xd1) - 2] ^ 0x65) << 0x10 |
          ((byte)puVar72[((ulong)pbVar61[6] ^ 0x40) - 7] ^ 0x79) << 8 |
          ((byte)puVar67[((ulong)pbVar61[4] ^ 0xe0) - 6] + 0x4b ^ 0xe8) << 0x18 |
          (byte)puVar62[((ulong)pbVar61[7] ^ 0x3f) - 0xb] ^ 0x28;
  uVar10 = uVar3 + (uVar3 & 0x697720c) * -2 + 0x697720c;
  uVar3 = ((uVar6 ^ 0x5b80b964) & 0xff) + ((uVar6 ^ 0x5b80b964) & 0xa7) * -2 + 0x1f2643a7;
  uVar11 = uVar3 ^ 0xdbbaed5b;
  puVar65 = tables[iVar57 + 0x89];
  uVar12 = (uVar11 - ((uVar11 & 0x576) << 1 | 0x8290000)) + 0x15778576 ^
           *(uint *)(puVar65 + (ulong)(uVar3 ^ 0x1f2643b6) * 4 + -0xf);
  uVar11 = (iVar4 - (iVar4 * 2 & 0x79f7c5c2U)) + 0x3cfbe2e1;
  puVar70 = tables[iVar57 + 0x97];
  uVar3 = uVar5 ^ 0x78;
  uVar3 = uVar11 ^ (uVar3 - (uVar3 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
          *(uint *)(puVar70 + (ulong)(uVar5 ^ 0xc4) * 4 + -7) ^
          uVar12 + (uVar12 & 0x3cfbe2e1) * -2 + 0x3cfbe2e1;
  puVar64 = tables[iVar57 + 0x26];
  puVar69 = tables[iVar57 + 0x68];
  uVar13 = *(int *)(puVar69 +
                   ((ulong)(byte)puVar72[((ulong)pbVar61[0xe] ^ 0xe3) - 7] ^ 0xe4) * 4 + -10) +
           0x82923b6fU ^
           *(uint *)(puVar64 +
                    ((ulong)(byte)puVar66[((ulong)pbVar61[0xd] ^ 0x84) - 2] ^ 0xa4) * 4 + -6) ^
           uVar3 + (uVar3 & 0x7eab6905) * -2 + 0x7eab6905;
  uVar14 = uVar10 ^ uVar13 ^ 0x654f64ea ^ 0x33a1f418;
  uVar15 = uVar52 ^ uVar14 ^ 0x72c8bd2a;
  uVar16 = uVar6 ^ uVar15;
  uVar17 = uVar16 ^ 0x134f22c8;
  uVar3 = uVar17 & 0xff ^ 0xc49cae69;
  uVar3 = (uVar3 - ((uVar3 & 0x76) << 1 | 0x8290800)) + 0x15778576 ^
          *(uint *)(puVar65 + (ulong)(uVar17 & 0xff ^ 0x84) * 4 + -0xf);
  uVar3 = uVar3 + (uVar3 & 0x7eab6905) * -2 + 0x7eab6905 ^
          *(uint *)(puVar64 + (ulong)(uVar17 >> 0x10 & 0xff ^ 0xe5) * 4 + -6);
  uVar5 = uVar17 >> 0x18 ^ 0x85;
  uVar18 = *(int *)(puVar69 + (ulong)(uVar17 >> 8 & 0xff ^ 0x21) * 4 + -10) + 0x82923b6fU ^
           *(uint *)(puVar70 + (ulong)(uVar17 >> 0x18 ^ 0x39) * 4 + -7) ^ uVar13 ^ 0x654f64ea ^
           (uVar5 - (uVar5 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
           uVar3 + (uVar3 & 0x654f64ea) * -2 + 0x654f64ea;
  uVar19 = uVar18 ^ 0x3f98d223;
  uVar20 = uVar52 ^ uVar19;
  uVar21 = uVar20 ^ uVar17 ^ 0x94592e63;
  uVar3 = uVar21 >> 0x18 ^ 0x77;
  uVar3 = (*(int *)(puVar69 + (ulong)(uVar21 >> 8 & 0xff ^ 0xdc) * 4 + -10) -
          (*(int *)(puVar69 + (ulong)(uVar21 >> 8 & 0xff ^ 0xdc) * 4 + -10) * 2 + 0x52476deU &
          0x7f31a446)) + 0xc22b0d92 ^ *(uint *)(puVar70 + (ulong)(uVar21 >> 0x18 ^ 0xcb) * 4 + -7) ^
          uVar19 ^ (uVar3 - (uVar3 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d;
  uVar5 = uVar21 & 0xff ^ 0xc49cae02;
  uVar22 = *(uint *)(puVar65 + (ulong)(uVar21 & 0xff ^ 0xef) * 4 + -0xf) ^
           *(uint *)(puVar64 + (ulong)(uVar21 >> 0x10 & 0xff ^ 0xfe) * 4 + -6) ^
           (uVar5 - ((uVar5 & 0x76) << 1 | 0x8290800)) + 0x15778576 ^
           uVar3 + (uVar3 & 0x7eab6905) * -2 + 0x7eab6905;
  uVar23 = uVar14 ^ uVar19 ^ uVar22 ^ 0x258d9763;
  uVar24 = uVar23 ^ uVar20 ^ 0x4601c465;
  uVar21 = uVar24 ^ uVar21;
  uVar3 = uVar21 >> 0x18 ^ 0x19;
  uVar5 = uVar21 & 0xff ^ 0xc49cae48;
  uVar25 = *(uint *)(puVar64 + (ulong)(uVar21 >> 0x10 & 0xff ^ 0x8f) * 4 + -6) ^
           *(int *)(puVar69 + (ulong)(uVar21 >> 8 & 0xff ^ 0x93) * 4 + -10) + 0x82923b6fU ^
           *(uint *)(puVar70 + (ulong)(uVar21 >> 0x18 ^ 0xa5) * 4 + -7) ^
           *(uint *)(puVar65 + (ulong)(uVar21 & 0xff ^ 0xa5) * 4 + -0xf) ^ uVar22 ^
           (uVar5 - ((uVar5 & 0x76) << 1 | 0x8290800)) + 0x15778576 ^
           (uVar3 - (uVar3 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d;
  uVar26 = uVar25 ^ 0x7eab6905;
  uVar27 = uVar26 ^ uVar20 ^ 0x4601c465;
  uVar28 = uVar27 ^ uVar21 ^ 0xcbb1f184;
  uVar3 = uVar28 & 0xff;
  iVar4 = (uVar3 ^ 0xc49caefb) +
          (*(uint *)(puVar65 + (ulong)(uVar3 ^ 0x16) * 4 + -0xf) ^ 0x15778576) +
          ((uVar3 ^ 0x449caefb) &
          (*(uint *)(puVar65 + (ulong)(uVar3 ^ 0x16) * 4 + -0xf) ^ 0x4148476)) * -2;
  uVar3 = *(int *)(puVar69 + (ulong)(uVar28 >> 8 & 0xff ^ 0xc2) * 4 + -10) + 0x82923b6fU ^ uVar26 ^
          (iVar4 - (iVar4 * 2 & 0xf413ed5aU)) + 0x7a09f6ad ^ 0x7a09f6ad;
  uVar5 = uVar28 >> 0x18 ^ 0xe9;
  uVar29 = *(uint *)(puVar70 + (ulong)(uVar28 >> 0x18 ^ 0x55) * 4 + -7) ^
           *(uint *)(puVar64 + (ulong)(uVar28 >> 0x10 & 0xff ^ 0xb2) * 4 + -6) ^
           (uVar5 - (uVar5 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
           uVar3 + (uVar3 & 0x7eab6905) * -2 + 0x7eab6905;
  uVar30 = uVar29 ^ uVar26 ^ uVar23;
  uVar31 = uVar27 ^ uVar30 ^ 0x35055031;
  uVar32 = uVar31 ^ uVar28;
  uVar3 = uVar32 & 0xff;
  uVar3 = (uVar3 ^ 0x1e) + (uVar3 ^ (uVar3 ^ 0x1e | 0x15ffb576) ^ 0x15ffb568) * -2 + 0x15778576 ^
          *(uint *)(puVar65 + (ulong)(uVar3 ^ 0x32) * 4 + -0xf);
  uVar3 = *(int *)(puVar69 + (ulong)(uVar32 >> 8 & 0xff ^ 0x73) * 4 + -10) + 0x82923b6fU ^ uVar29 ^
          uVar3 + (uVar3 & 0x4fd1f582) * -2 + 0x4fd1f582;
  uVar5 = uVar3 ^ 0xde575bdd;
  uVar12 = *(uint *)(puVar64 + (ulong)(uVar32 >> 0x10 & 0xff ^ 0x40) * 4 + -6) ^ 0x7eab6905;
  iVar4 = uVar12 - (*(uint *)(puVar64 + (ulong)(uVar32 >> 0x10 & 0xff ^ 0x40) * 4 + -6) ^ uVar5 ^
                   0xba37c7c4);
  uVar33 = uVar32 >> 0x18 ^ 0x36;
  uVar34 = (uVar33 - (uVar33 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
           *(uint *)(puVar70 + (ulong)(uVar32 >> 0x18 ^ 0x8a) * 4 + -7) ^
           (uVar5 ^ -(uVar3 ^ 0x1acbf51c) ^ (iVar4 - (iVar4 * 2 & 0x8fbad054U)) + 0x47dd682a ^
           0x8341c6eb) + uVar12;
  uVar3 = uVar34 ^ 0x5e2bc04c;
  uVar35 = uVar30 ^ uVar3;
  uVar36 = uVar35 ^ 0x8d84a556;
  uVar28 = uVar36 ^ uVar28;
  uVar5 = uVar28 & 0xff ^ 0xc49caec6;
  uVar5 = (uVar5 - ((uVar5 & 0x76) << 1 | 0x8290800)) + 0x15778576 ^
          *(uint *)(puVar65 + (ulong)(uVar28 & 0xff ^ 0x2b) * 4 + -0xf);
  uVar12 = uVar28 >> 0x18 ^ 0x4c;
  uVar37 = *(uint *)(puVar70 + (ulong)(uVar28 >> 0x18 ^ 0xf0) * 4 + -7) ^
           *(uint *)(puVar64 + (ulong)(uVar28 >> 0x10 & 0xff ^ 0x2f) * 4 + -6) ^
           (*(int *)(puVar69 + (ulong)((uVar28 >> 8 ^ 0xffffffff) & 0xff) * 4 + -10) * 2 +
            0x13e51f6cU & 0x713f5772) +
           (*(int *)(puVar69 + (ulong)((uVar28 >> 8 ^ 0xffffffff) & 0xff) * 4 + -10) + 0xc9f28fb6U ^
           0xb89fabb9) ^ uVar3 ^ (uVar12 - (uVar12 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
           uVar5 + (uVar5 & 0x5e2bc04c) * -2 + 0x5e2bc04c;
  uVar31 = uVar36 ^ uVar31;
  uVar5 = uVar37 ^ 0xa8938483;
  uVar36 = uVar36 ^ uVar5;
  uVar38 = uVar36 ^ 0x5c685d36;
  uVar39 = uVar38 ^ uVar31;
  uVar40 = uVar39 ^ uVar28;
  uVar3 = uVar40 >> 0x18 ^ 0xf4;
  uVar3 = (uVar3 - (uVar3 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
          *(uint *)(puVar70 + (ulong)(uVar40 >> 0x18 ^ 0x48) * 4 + -7);
  uVar5 = uVar3 + (uVar3 & 0x3b38ed86) * -2 + 0x3b38ed86 ^ uVar5;
  uVar3 = ((uVar40 ^ 0xef68065b) & 0xff) + ((uVar40 ^ 0xef68065b) & 0x36) * -2 + 0x3a312336;
  uVar33 = *(uint *)(puVar65 + (ulong)(uVar40 & 0xff ^ 0xbf) * 4 + -0xf);
  uVar12 = uVar3 ^ 0xfead8d3f;
  uVar3 = uVar12 - (uVar33 ^ uVar3 ^ 0xebda0849) ^ -(uVar33 ^ 0x15778576);
  uVar41 = *(int *)(puVar69 + (ulong)(uVar40 >> 8 & 0xff ^ 0xfe) * 4 + -10) + 0x82923b6fU ^
           *(uint *)(puVar64 + (ulong)(uVar40 >> 0x10 & 0xff ^ 0x8d) * 4 + -6) ^
           (uVar3 + (uVar3 & 0x15778576) * -2 + 0x15778576 ^ uVar33) + uVar12 ^
           uVar5 + (uVar5 & 0x7eab6905) * -2 + 0x7eab6905;
  uVar42 = uVar41 ^ uVar31;
  uVar43 = uVar42 ^ uVar40 ^ 0x6e5ac8d9;
  uVar3 = uVar43 & 0xff ^ 0xc49cae97;
  uVar3 = *(uint *)(puVar65 + (ulong)(uVar43 & 0xff ^ 0x7a) * 4 + -0xf) ^
          *(int *)(puVar69 + (ulong)(uVar43 >> 8 & 0xff ^ 0xa3) * 4 + -10) + 0x82923b6fU ^ uVar41 ^
          (uVar3 - ((uVar3 & 0x76) << 1 | 0x8290800)) + 0x15778576 ^ 0xde782d98;
  uVar5 = uVar43 >> 0x18 ^ 0xb0;
  uVar44 = *(uint *)(puVar70 + (ulong)(uVar43 >> 0x18 ^ 0xc) * 4 + -7) ^
           *(uint *)(puVar64 + (ulong)(uVar43 >> 0x10 & 0xff ^ 0xf1) * 4 + -6) ^
           (uVar5 - (uVar5 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
           uVar3 + (uVar3 & 0x7eab6905) * -2 + 0x7eab6905;
  uVar38 = uVar41 ^ uVar38;
  uVar45 = uVar38 ^ uVar44 ^ 0xfacd8d8d;
  uVar46 = uVar45 ^ uVar40 ^ 0x6e5ac8d9;
  uVar3 = uVar46 >> 0x18 ^ 0xac;
  uVar3 = (uVar3 - (uVar3 * 2 + 0x246 & 0x254 | 0xc8890000)) + 0xe23c0a4d ^
          *(uint *)(puVar70 + (ulong)(uVar46 >> 0x18 ^ 0x10) * 4 + -7);
  uVar5 = *(int *)(puVar69 + (ulong)(uVar46 >> 8 & 0xff ^ 2) * 4 + -10) + 0x82923b6fU ^ uVar44 ^
          uVar3 + (uVar3 & 0x9662c6d) * -2 + 0x9662c6d ^ 0x9662c6d;
  uVar3 = ((uVar46 ^ 0xc1bfc8d8) & 0xff) + ((uVar46 ^ 0xc1bfc8d8) & 0xf3) * -2 + 0x44c831f3;
  uVar5 = uVar5 + (uVar5 & 0x7eab6905) * -2 + 0x7eab6905 ^
          *(uint *)(puVar64 + (ulong)(uVar46 >> 0x10 & 0xff ^ 0xe6) * 4 + -6);
  uVar47 = uVar3 ^ *(uint *)(puVar65 + (ulong)(uVar3 ^ 0x44c831d0) * 4 + -0xf) ^
           uVar5 + (uVar5 & 0x15778576) * -2 + 0x15778576;
  uVar48 = uVar46 ^ uVar47 ^ uVar42 ^ 0x3cccedab;
  uVar49 = uVar47 ^ uVar42 ^ 0xd59b4d21;
  iVar4 = (uVar24 ^ 0x2a09be64) - (uVar13 ^ 0x4f54299);
  uVar3 = (iVar4 - (iVar4 * 2 & 0xb1f45ca4U)) + 0x58fa2e52;
  iVar4 = (uVar46 ^ 0x2849af33) + (uVar16 ^ 0x487fb4cc);
  uVar54 = 0x91cb18c8 - (iVar4 + (iVar4 * -2 + 0x989ef21aU & 0x8af73f76));
  uVar5 = ((byte)puVar66[((ulong)*(byte *)(lVar73 + 0xa5) ^ 0x52) - 2] ^ 7) << 0x10 |
          ((byte)puVar72[((ulong)*(byte *)(lVar73 + 0xa6) ^ 0xe2) - 7] ^ 0x95) << 8 |
          (byte)puVar62[((ulong)*(byte *)(lVar73 + 0xa7) ^ 0xfe) - 0xb] ^ 0x71 |
          ((byte)puVar67[((ulong)*(byte *)(lVar73 + 0xa4) ^ 8) - 6] + 0x4b ^ 0xca) << 0x18;
  uVar12 = ((byte)puVar72[((ulong)*(byte *)(lVar73 + 0x9a) ^ 0x25) - 7] ^ 0xa1) << 8 |
           ((byte)puVar66[((ulong)*(byte *)(lVar73 + 0x99) ^ 0x47) - 2] ^ 0x25) << 0x10 |
           (byte)puVar62[((ulong)*(byte *)(lVar73 + 0x9b) ^ 0xa0) - 0xb] ^ 0x73 |
           ((byte)puVar67[((ulong)*pbVar2 ^ 0x21) - 6] + 0x4b ^ 0xe5) << 0x18;
  uVar33 = ((byte)puVar67[((ulong)*(byte *)(lVar73 + 0x9c) ^ 0x5b) - 6] + 0x4b ^ 0x51) << 0x18 |
           ((byte)puVar72[((ulong)*(byte *)(lVar73 + 0x9e) ^ 0xa4) - 7] ^ 0xd0) << 8 |
           (byte)puVar62[((ulong)*(byte *)(lVar73 + 0x9f) ^ 0xae) - 0xb] ^ 0x79 |
           ((byte)puVar66[((ulong)*(byte *)(lVar73 + 0x9d) ^ 0x23) - 2] ^ 0xfb) << 0x10;
  uVar50 = uVar3 ^ 0xeab3aab1;
  puVar69 = tables[iVar57 + -0x37];
  puVar64 = tables[iVar57 + 0xe];
  puVar65 = tables[iVar57 + 0x20];
  puVar70 = tables[iVar57 + 0x4d];
  uVar7 = ((byte)puVar69[((ulong)pbVar51[8] ^ 0xab) - 0xf] ^ 0xc6) << 0x18 |
          ((byte)puVar65[((ulong)pbVar51[9] ^ 0x8d) - 2] ^ 0xc6) << 0x10 |
          (byte)puVar70[((ulong)pbVar51[0xb] ^ 0xf8) - 0xb] ^ 0x5f |
          ((byte)puVar64[((ulong)pbVar51[10] ^ 0xa5) - 0xe] ^ 0xf2) << 8;
  uVar8 = ((byte)puVar69[((ulong)pbVar51[0xc] ^ 0xd) - 0xf] ^ 0x78) << 0x18 |
          ((byte)puVar64[((ulong)pbVar51[0xe] ^ 0xb4) - 0xe] ^ 0xd5) << 8 |
          (byte)puVar70[((ulong)pbVar51[0xf] ^ 0x56) - 0xb] ^ 0x30 |
          ((((byte)puVar65[((ulong)pbVar51[0xd] ^ 0xa8) - 2] ^ 0x38) -
           (((byte)puVar65[((ulong)pbVar51[0xd] ^ 0xa8) - 2] & 0x27) << 1 ^ 0x40)) + 0x27 & 0xff) <<
          0x10;
  uVar9 = ((byte)puVar65[((ulong)pbVar51[5] ^ 0xc0) - 2] ^ 0xbf) << 0x10 |
          ((byte)puVar69[((ulong)pbVar51[4] ^ 0x91) - 0xf] ^ 0xb3) << 0x18 |
          (byte)puVar70[((ulong)pbVar51[7] ^ 1) - 0xb] ^ 0xaa |
          ((byte)puVar64[((ulong)pbVar51[6] ^ 0xd8) - 0xe] ^ 0x4c) << 8;
  uVar53 = ((byte)puVar69[((ulong)*pbVar51 ^ 0xd3) - 0xf] ^ 0x2b) << 0x18 |
           ((byte)puVar64[((ulong)pbVar51[2] ^ 0x1a) - 0xe] ^ 0x6a) << 8 |
           ((byte)puVar65[((ulong)pbVar51[1] ^ 0x58) - 2] ^ 0xf6) << 0x10 |
           (byte)puVar70[((ulong)pbVar51[3] ^ 0x2a) - 0xb] ^ 0xd5;
  uVar52 = uVar52 ^ uVar54 ^ 0xff1b3640 ^
           (((byte)puVar67[((ulong)*(byte *)(lVar73 + 0xa0) ^ 0xeb) - 6] + 0x4b ^ 0xa7) << 0x18 |
           (byte)puVar62[((ulong)*(byte *)(lVar73 + 0xa3) ^ 0xd5) - 0xb] ^ 0xd2 |
           ((byte)puVar72[((ulong)*(byte *)(lVar73 + 0xa2) ^ 0x18) - 7] ^ 0x9e) << 8 |
           ((byte)puVar66[((ulong)*(byte *)(lVar73 + 0xa1) ^ 0xf2) - 2] ^ 0xad) << 0x10) ^
           0x78455d9e ^ uVar7 + 0x78455d42 + (uVar7 & 0x78455d42) * -2;
  uVar5 = uVar6 ^ uVar54 ^ 0x21946040 ^ uVar5 + (uVar5 & 0x676d7192) * -2 + 0x676d7192 ^
          uVar8 + 0x676d7192 + (uVar8 & 0x676d7192) * -2;
  uVar52 = uVar52 + 0x457b9fbb + (uVar52 & 0x457b9fbb) * -2 ^ uVar54;
  uVar5 = uVar5 + 0x457b9fbb + (uVar5 & 0x457b9fbb) * -2 ^ uVar54;
  uVar11 = uVar3 ^ uVar11 ^ uVar12 + (uVar12 & 0xa6d9693) * -2 + 0xa6d9693 ^
           uVar53 + 0xa6d9693 + (uVar53 & 0xa6d9693) * -2;
  uVar10 = uVar10 ^ uVar3 ^ 0x67ee5dfb ^ uVar33 + (uVar33 & 0x3a07b405) * -2 + 0x3a07b405 ^
           uVar9 + 0x3a07b405 + (uVar9 & 0x3a07b405) * -2;
  uVar7 = (uVar11 ^ 0xe50a8f18) + (uVar3 ^ 0xbcb0b60f) +
          ((uVar11 ^ 0x650a8f18) & (uVar3 ^ 0x3cb0b60f)) * -2;
  uVar10 = uVar10 + 0x56031cbe + (uVar10 & 0x56031cbe) * -2 ^ uVar50;
  puVar65 = tables[iVar57 + 0x1b];
  puVar69 = tables[iVar57 + 0x2b];
  uVar11 = *(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 199) * 4 + -3) + 0xce853ebaU ^
           *(uint *)(puVar65 + (ulong)(uVar7 >> 0x18 ^ 0x2c) * 4 + -3) ^
           ((uVar7 >> 0x18 ^ 0x33c08c3c) + 0x377d6c81) - (uVar7 >> 0x17 & 0x102 | 0x66801800);
  puVar62 = tables[iVar57 + -0x36];
  uVar12 = *(int *)(puVar69 + (ulong)(uVar7 & 0xff ^ 0xd8) * 4 + -3) + 0xce853ebaU ^
           *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0x68) * 4 + -3) ^
           ((uVar10 >> 0x18 ^ 0x33c08c78) + 0x377d6c81) - (uVar10 >> 0x17 & 0x102 | 0x66801800);
  uVar12 = uVar12 + 0x4eb7d448 + (uVar12 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar62 + (ulong)(uVar5 >> 8 & 0xff ^ 0xa5) * 4 + -2);
  puVar64 = tables[iVar57 + -0x13];
  uVar11 = uVar11 + 0xe4b91e3 + (uVar11 & 0xe4b91e3) * -2 ^
           *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0x55) * 4 + -0xb);
  uVar6 = (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xa5) * 4 + -3) + 0x1d3d1302) -
          (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xa5) * 4 + -3) * 2 + 0x9d0a7d74U & 0x9d6fa890
          ) ^ *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0xb4) * 4 + -2);
  uVar33 = uVar52 >> 0x18 ^ 0x33c08c9c;
  uVar33 = *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0x8c) * 4 + -3) ^
           (uVar33 + 0x377d6c81) - ((uVar33 & 0x81) << 1 | 0x66801800);
  uVar6 = ((uVar5 >> 0x18 ^ 0x33c08c4a) + 0x377d6c81) - (uVar5 >> 0x17 & 0x102 | 0x66801800) ^
          *(uint *)(puVar64 + (ulong)(uVar7 >> 0x10 & 0xff ^ 0x26) * 4 + -0xb) ^
          *(uint *)(puVar65 + (ulong)(uVar5 >> 0x18 ^ 0x5a) * 4 + -3) ^
          uVar6 + 0xe4b91e3 + (uVar6 & 0xe4b91e3) * -2;
  uVar10 = uVar33 + 0x4eb7d448 + (uVar33 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar62 + (ulong)(uVar7 >> 8 & 0xff ^ 0x98) * 4 + -2) ^
           *(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0x9a) * 4 + -3) + 0xce853ebaU;
  uVar11 = uVar11 + 0x4eb7d448 + (uVar11 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x3e) * 4 + -2);
  uVar10 = uVar15 ^ uVar54 ^ 0xfc9cc0eb ^
           *(uint *)(puVar64 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0xea) * 4 + -0xb) ^
           uVar10 + 0xe4b91e3 + (uVar10 & 0xe4b91e3) * -2;
  uVar10 = uVar10 + 0x457b9fbb + (uVar10 & 0x457b9fbb) * -2 ^ uVar54;
  uVar5 = uVar16 ^ 0x6bff3f42 ^ uVar6 + (uVar6 & 0x23808b8e) * -2 + 0x23808b8e;
  uVar52 = uVar14 ^ uVar54 ^ 0x8597d59d ^
           *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x59) * 4 + -0xb) ^
           uVar12 + 0xe4b91e3 + (uVar12 & 0xe4b91e3) * -2;
  uVar52 = uVar52 + 0x457b9fbb + (uVar52 & 0x457b9fbb) * -2 ^ uVar54;
  uVar12 = (*(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0xa7) * 4 + -3) + -0x232f2f63) -
           (*(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0xa7) * 4 + -3) * 2 + 0x1d0a7d74U & 0x1c9723c6
           ) ^ *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x34) * 4 + -0xb);
  uVar6 = uVar10 >> 0x18 ^ 0x33c08c17;
  uVar6 = (uVar6 + 0x377d6c81) - ((uVar6 & 0x81) << 1 | 0x66801800) ^
          *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 7) * 4 + -3);
  uVar11 = uVar13 ^ 0x5116330 ^ uVar11 + (uVar11 & 0x1e421a9) * -2 + 0x1e421a9;
  uVar33 = *(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0x19) * 4 + -3) + 0xce853ebaU ^
           *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0x4a) * 4 + -3) ^
           ((uVar52 >> 0x18 ^ 0x33c08c5a) + 0x377d6c81) - (uVar52 >> 0x17 & 0x102 | 0x66801800);
  uVar33 = uVar33 + 0x4eb7d448 + (uVar33 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar62 + (ulong)(uVar5 >> 8 & 0xff ^ 0x68) * 4 + -2);
  uVar7 = uVar5 >> 0x18 ^ 0x33c08cf4;
  uVar7 = (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
          *(uint *)(puVar65 + (ulong)(uVar5 >> 0x18 ^ 0xe4) * 4 + -3);
  uVar5 = uVar6 + 0xe4b91e3 + (uVar6 & 0xe4b91e3) * -2 ^
          *(uint *)(puVar64 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0x6e) * 4 + -0xb);
  uVar6 = *(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0xb6) * 4 + -3) + 0xce853ebaU ^
          *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x94) * 4 + -2) ^
          uVar7 + 0x4eb7d448 + (uVar7 & 0x4eb7d448) * -2;
  uVar7 = uVar11 >> 0x18 ^ 0x33c08c89;
  uVar12 = uVar18 ^ 0x964f96f7 ^ *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0x8a) * 4 + -2) ^
           uVar12 + 0x4eb7d448 + (uVar12 & 0x4eb7d448) * -2 ^
           (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar65 + (ulong)(uVar11 >> 0x18 ^ 0x99) * 4 + -3);
  uVar10 = uVar14 ^ uVar19 ^ 0xf30b7afb ^
           *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0xe2) * 4 + -0xb) ^
           uVar33 + 0xe4b91e3 + (uVar33 & 0xe4b91e3) * -2;
  uVar52 = uVar20 ^ 0x1adb87e7 ^ *(uint *)(puVar62 + (ulong)(uVar11 >> 8 & 0xff ^ 0x36) * 4 + -2) ^
           *(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0x9b) * 4 + -3) + 0xce853ebaU ^
           uVar5 + 0x4eb7d448 + (uVar5 & 0x4eb7d448) * -2;
  uVar11 = uVar20 ^ uVar17 ^ 0x318b8018 ^
           *(uint *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0xc5) * 4 + -0xb) ^
           uVar6 + 0xe4b91e3 + (uVar6 & 0xe4b91e3) * -2;
  uVar5 = (*(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0x3f) * 4 + -3) + 0x1d3d1302) -
          (*(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0x3f) * 4 + -3) * 2 + 0x9d0a7d74U & 0x9d6fa890
          ) ^ *(uint *)(puVar62 + (ulong)(uVar12 >> 8 & 0xff ^ 0xfb) * 4 + -2);
  uVar5 = *(uint *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0x97) * 4 + -0xb) ^ 0x377d6cee ^
          *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0x2c) * 4 + -3) ^ uVar52 >> 0x18 ^
          uVar5 + 0xe4b91e3 + (uVar5 & 0xe4b91e3) * -2;
  uVar6 = uVar10 >> 0x18 ^ 0x377d6c9b ^ *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0x59) * 4 + -3)
  ;
  uVar6 = uVar6 + 0xe4b91e3 + (uVar6 & 0xe4b91e3) * -2 ^
          *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x2f) * 4 + -0xb);
  uVar33 = (*(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0xb3) * 4 + -3) + -0x232f2f63) -
           (*(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0xb3) * 4 + -3) * 2 + 0x1d0a7d74U &
           0x1c9723c6) ^ *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0x84) * 4 + -0xb);
  uVar7 = uVar11 >> 0x18 ^ 0x33c08cf8;
  uVar8 = uVar12 >> 0x18 ^ 0x33c08cc9;
  uVar10 = uVar21 ^ 0x90ae848f ^
           *(uint *)(puVar64 + (ulong)(uVar12 >> 0x10 & 0xff ^ 0x36) * 4 + -0xb) ^
           *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0xc1) * 4 + -2) ^
           (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar65 + (ulong)(uVar11 >> 0x18 ^ 0xe8) * 4 + -3) ^
           *(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0x30) * 4 + -3) + 0xce853ebaU;
  uVar5 = uVar24 ^ 0x626932c5 ^ uVar5 + (uVar5 & 0x48608ca1) * -2 + 0x48608ca1;
  uVar11 = *(uint *)(puVar62 + (ulong)(uVar11 >> 8 & 0xff ^ 0xad) * 4 + -2) ^ uVar23 ^
           *(int *)(puVar69 + (ulong)(uVar12 & 0xff ^ 0x72) * 4 + -3) + 0xce853ebaU ^
           uVar6 + 0x4eb7d448 + (uVar6 & 0x4eb7d448) * -2;
  uVar52 = uVar22 ^ 0x45c73e3d ^ (uVar8 + 0x377d6c81) - ((uVar8 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x8e) * 4 + -2) ^
           *(uint *)(puVar65 + (ulong)(uVar12 >> 0x18 ^ 0xd9) * 4 + -3) ^
           uVar33 + 0x4eb7d448 + (uVar33 & 0x4eb7d448) * -2;
  uVar12 = (*(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0xab) * 4 + -3) + -0x232f2f63) -
           (*(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0xab) * 4 + -3) * 2 + 0x1d0a7d74U &
           0x1c9723c6) ^ *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0x66) * 4 + -0xb);
  uVar6 = uVar11 >> 0x18 ^ 0x33c08c3d;
  uVar6 = *(uint *)(puVar65 + (ulong)(uVar11 >> 0x18 ^ 0x2d) * 4 + -3) ^
          *(uint *)(puVar64 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0xe) * 4 + -0xb) ^
          (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xbd) * 4 + -3) + -0x232f2f63) -
          (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xbd) * 4 + -3) * 2 + 0x1d0a7d74U & 0x1c9723c6
          ) ^ (uVar6 + 0x377d6c81) - ((uVar6 & 0x81) << 1 | 0x66801800);
  uVar33 = uVar52 >> 0x18 ^ 0xe4b9124 ^
           *(uint *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0x44) * 4 + -0xb);
  uVar33 = uVar33 + 0x377d6c81 + (uVar33 & 0x377d6c81) * -2 ^
           *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0x84) * 4 + -3);
  uVar7 = uVar10 >> 0x18 ^ 0x33c08ce3;
  uVar7 = (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
          *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0xf3) * 4 + -3) ^
          *(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0xa2) * 4 + -3) + 0xce853ebaU;
  uVar11 = uVar7 + 0x4eb7d448 + (uVar7 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar62 + (ulong)(uVar11 >> 8 & 0xff ^ 0xe) * 4 + -2);
  uVar7 = uVar5 >> 0x18 ^ 0x33c08c37;
  uVar12 = uVar27 ^ 0xdacebdbd ^ (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x4a) * 4 + -2) ^
           uVar12 + 0x4eb7d448 + (uVar12 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar65 + (ulong)(uVar5 >> 0x18 ^ 0x27) * 4 + -3);
  uVar5 = uVar25 ^ 0x348b8bca ^ *(uint *)(puVar62 + (ulong)(uVar5 >> 8 & 0xff ^ 0xa7) * 4 + -2) ^
          *(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0xa6) * 4 + -3) + 0xce853ebaU ^
          uVar33 + 0x4eb7d448 + (uVar33 & 0x4eb7d448) * -2;
  uVar52 = uVar27 ^ uVar21 ^ 0x249841 ^
           *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0xf7) * 4 + -0xb) ^
           uVar11 + 0xe4b91e3 + (uVar11 & 0xe4b91e3) * -2;
  uVar10 = uVar26 ^ uVar23 ^ 0x3d1e55f7 ^
           *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0x8d) * 4 + -2) ^
           uVar6 + 0x4eb7d448 + (uVar6 & 0x4eb7d448) * -2;
  uVar11 = (*(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0x55) * 4 + -3) + 0x1d3d1302) -
           (*(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0x55) * 4 + -3) * 2 + 0x9d0a7d74U &
           0x9d6fa890) ^ *(uint *)(puVar62 + (ulong)(uVar5 >> 8 & 0xff ^ 3) * 4 + -2);
  uVar6 = ((uVar10 >> 0x18 ^ 0x33c08c28) + 0x377d6c81) - (uVar10 >> 0x17 & 0x102 | 0x66801800) ^
          *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0x38) * 4 + -3) ^
          *(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0x19) * 4 + -3) + 0xce853ebaU;
  uVar6 = uVar6 + 0xe4b91e3 + (uVar6 & 0xe4b91e3) * -2 ^
          *(uint *)(puVar64 + (ulong)(uVar12 >> 0x10 & 0xff ^ 0xf4) * 4 + -0xb);
  uVar33 = (*(int *)(puVar69 + (ulong)(uVar12 & 0xff ^ 0x55) * 4 + -3) + 0x1d3d1302) -
           (*(int *)(puVar69 + (ulong)(uVar12 & 0xff ^ 0x55) * 4 + -3) * 2 + 0x9d0a7d74U &
           0x9d6fa890) ^ *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0x60) * 4 + -2) ^
           *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0x18) * 4 + -3) ^
           ((uVar52 >> 0x18 ^ 0x33c08c08) + 0x377d6c81) - (uVar52 >> 0x17 & 0x102 | 0x66801800);
  uVar10 = *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0x55) * 4 + -0xb) ^
           ((uVar5 >> 0x18 ^ 0x33c08c22) + 0x377d6c81) - (uVar5 >> 0x17 & 0x102 | 0x66801800) ^
           (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xf4) * 4 + -3) + -0x232f2f63) -
           (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xf4) * 4 + -3) * 2 + 0x1d0a7d74U &
           0x1c9723c6) ^ *(uint *)(puVar65 + (ulong)(uVar5 >> 0x18 ^ 0x32) * 4 + -3);
  uVar13 = uVar10 + 0x4eb7d448 + (uVar10 & 0x4eb7d448) * -2;
  uVar10 = *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x9c) * 4 + -0xb) ^
           uVar11 + 0xe4b91e3 + (uVar11 & 0xe4b91e3) * -2;
  uVar11 = uVar32 ^ 0x6717c8f5 ^
           *(uint *)(puVar64 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0x99) * 4 + -0xb) ^
           uVar33 + 0xe4b91e3 + (uVar33 & 0xe4b91e3) * -2;
  uVar10 = uVar27 ^ uVar30 ^ 0x20c9a3b4 ^ uVar12 >> 0x18 ^
           *(uint *)(puVar65 + (ulong)(uVar12 >> 0x18 ^ 0xb1) * 4 + -3) ^
           uVar10 + 0x377d6c81 + (uVar10 & 0x377d6c81) * -2;
  uVar52 = uVar30 ^ 0x3eea361e ^ *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x74) * 4 + -2) ^
           uVar6 + 0x4eb7d448 + (uVar6 & 0x4eb7d448) * -2;
  uVar5 = uVar29 ^ 0x87746b7f ^ *(uint *)(puVar62 + (ulong)(uVar12 >> 8 & 0xff ^ 199) * 4 + -2);
  uVar12 = uVar5 ^ uVar13;
  uVar6 = uVar10 >> 0x18 ^ 0x33c08cb9;
  uVar6 = (uVar6 + 0x377d6c81) - ((uVar6 & 0x81) << 1 | 0x66801800) ^
          *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0xa9) * 4 + -3);
  uVar33 = uVar52 >> 0x18 ^ 0x33c08caf;
  uVar33 = (uVar33 + 0x377d6c81) - ((uVar33 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0xbf) * 4 + -3);
  uVar33 = uVar33 + 0xe4b91e3 + (uVar33 & 0xe4b91e3) * -2 ^
           *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0x48) * 4 + -0xb);
  uVar7 = uVar12 >> 0x18 ^ 0x33c08c43;
  uVar6 = *(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xe3) * 4 + -3) + 0xce853ebaU ^
          *(uint *)(puVar62 + (ulong)(uVar12 >> 8 & 0xff ^ 0x1f) * 4 + -2) ^
          uVar6 + 0x4eb7d448 + (uVar6 & 0x4eb7d448) * -2;
  uVar6 = uVar31 ^ 0x50321738 ^
          *(uint *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0xec) * 4 + -0xb) ^
          uVar6 + 0xe4b91e3 + (uVar6 & 0xe4b91e3) * -2;
  uVar8 = uVar28 ^ 0x13e1fc0c ^
          (*(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0xdf) * 4 + -3) + 0x1d3d1302) -
          (*(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0xdf) * 4 + -3) * 2 + 0x9d0a7d74U & 0x9d6fa890
          ) ^ *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0xad) * 4 + -2);
  uVar9 = uVar11 >> 0x18 ^ 0x33c08c7b;
  uVar52 = uVar34 ^ 0x8252f3da ^ *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0x14) * 4 + -2) ^
           *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x27) * 4 + -0xb) ^
           *(uint *)(puVar65 + (ulong)(uVar12 >> 0x18 ^ 0x53) * 4 + -3) ^
           (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
           *(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0xe0) * 4 + -3) + 0xce853ebaU;
  uVar10 = uVar8 + 0xe4b91e3 + (uVar8 & 0xe4b91e3) * -2 ^
           (uVar9 + 0x377d6c81) - ((uVar9 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar65 + (ulong)(uVar11 >> 0x18 ^ 0x6b) * 4 + -3) ^
           *(uint *)(puVar64 + (ulong)(uVar12 >> 0x10 & 0xff ^ 0x44) * 4 + -0xb);
  uVar11 = uVar35 ^ 0xf300959b ^ *(uint *)(puVar62 + (ulong)(uVar11 >> 8 & 0xff ^ 100) * 4 + -2) ^
           *(int *)(puVar69 + (ulong)((uVar5 ^ uVar13 ^ 0xffffffff) & 0xff) * 4 + -3) + 0xce853ebaU
           ^ uVar33 + 0x4eb7d448 + (uVar33 & 0x4eb7d448) * -2;
  uVar5 = uVar11 >> 0x18 ^ 0x33c08c9b;
  uVar5 = (uVar5 + 0x377d6c81) - ((uVar5 & 0x81) << 1 | 0x66801800) ^
          *(uint *)(puVar65 + (ulong)(uVar11 >> 0x18 ^ 0x8b) * 4 + -3);
  uVar12 = (*(int *)(puVar69 + (ulong)(uVar10 & 0xff) * 4 + -3) + 0x1d3d1302) -
           (*(int *)(puVar69 + (ulong)(uVar10 & 0xff) * 4 + -3) * 2 + 0x9d0a7d74U & 0x9d6fa890) ^
           *(uint *)(puVar62 + (ulong)(uVar6 >> 8 & 0xff ^ 0x41) * 4 + -2);
  uVar33 = uVar6 >> 0x18 ^ 0x377d6cb8 ^ *(uint *)(puVar65 + (ulong)(uVar6 >> 0x18 ^ 0x7a) * 4 + -3);
  uVar33 = uVar33 + 0xe4b91e3 + (uVar33 & 0xe4b91e3) * -2 ^
           *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0x75) * 4 + -0xb);
  uVar5 = uVar5 + 0xe4b91e3 + (uVar5 & 0xe4b91e3) * -2 ^
          *(uint *)(puVar64 + (ulong)(uVar6 >> 0x10 & 0xff ^ 0xeb) * 4 + -0xb);
  uVar7 = uVar52 >> 0x18 ^ 0x33c08c25;
  uVar8 = uVar10 >> 0x18 ^ 0x33c08c75;
  uVar5 = uVar36 ^ 0x215e2ccb ^ *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0xdc) * 4 + -2) ^
          uVar5 + 0x4eb7d448 + (uVar5 & 0x4eb7d448) * -2 ^
          *(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xab) * 4 + -3) + 0xce853ebaU;
  uVar10 = uVar40 ^ 0x1f432ca8 ^
           *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x51) * 4 + -0xb) ^
           *(uint *)(puVar62 + (ulong)(uVar11 >> 8 & 0xff ^ 0xd7) * 4 + -2) ^
           (uVar8 + 0x377d6c81) - ((uVar8 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0x65) * 4 + -3) ^
           *(int *)(puVar69 + (ulong)(uVar6 & 0xff ^ 0xfe) * 4 + -3) + 0xce853ebaU;
  uVar6 = uVar39 ^ 0x7a337c23 ^ *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x75) * 4 + -2) ^
          uVar33 + 0x4eb7d448 + (uVar33 & 0x4eb7d448) * -2 ^
          *(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0x2f) * 4 + -3) + 0xce853ebaU;
  uVar52 = uVar37 ^ 0xd81f3004 ^
           *(uint *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0xaf) * 4 + -0xb) ^
           (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0x35) * 4 + -3) ^
           uVar12 + 0xe4b91e3 + (uVar12 & 0xe4b91e3) * -2;
  uVar11 = ((uVar10 >> 0x18 ^ 0x33c08c81) + 0x377d6c81) - (uVar10 >> 0x17 & 0x102 ^ 0x66801902) ^
           *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0x91) * 4 + -3);
  uVar11 = uVar11 + 0x4eb7d448 + (uVar11 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar62 + (ulong)(uVar5 >> 8 & 0xff ^ 0x26) * 4 + -2);
  uVar12 = uVar52 >> 0x18 ^ 0x33c08c0b;
  uVar12 = *(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0x9b) * 4 + -3) + 0xce853ebaU ^
           (uVar12 + 0x377d6c81) - ((uVar12 & 0x81) << 1 | 0x66801800) ^
           *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0x1b) * 4 + -3);
  uVar12 = uVar12 + 0xe4b91e3 + (uVar12 & 0xe4b91e3) * -2 ^
           *(uint *)(puVar64 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0x4d) * 4 + -0xb);
  uVar33 = uVar6 >> 0x18 ^ 0x377d6c63 ^
           (*(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0xaf) * 4 + -3) + 0x1d3d1302) -
           (*(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0xaf) * 4 + -3) * 2 + 0x9d0a7d74U & 0x9d6fa890
           ) ^ *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x83) * 4 + -2) ^
           *(uint *)(puVar65 + (ulong)(uVar6 >> 0x18 ^ 0xa1) * 4 + -3);
  uVar7 = uVar5 >> 0x18 ^ 0x33c08cff;
  uVar5 = (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800) ^
          *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0x48) * 4 + -2) ^
          (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xde) * 4 + -3) + 0x1d3d1302) -
          (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xde) * 4 + -3) * 2 + 0x9d0a7d74U & 0x9d6fa890
          ) ^ *(uint *)(puVar65 + (ulong)(uVar5 >> 0x18 ^ 0xef) * 4 + -3);
  uVar10 = uVar42 ^ 0x5aa762b4 ^
           *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0x89) * 4 + -0xb) ^
           uVar33 + 0xe4b91e3 + (uVar33 & 0xe4b91e3) * -2;
  uVar52 = uVar43 ^ 0x8780aca1 ^ *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 2) * 4 + -0xb)
           ^ uVar11 + 0xe4b91e3 + (uVar11 & 0xe4b91e3) * -2 ^
           *(int *)(puVar69 + (ulong)(uVar6 & 0xff ^ 0x3b) * 4 + -3) + 0xce853ebaU;
  uVar11 = uVar41 ^ 0x9a463139 ^ uVar12 + 0x4eb7d448 + (uVar12 & 0x4eb7d448) * -2 ^
           *(uint *)(puVar62 + (ulong)(uVar6 >> 8 & 0xff ^ 0x2b) * 4 + -2);
  uVar5 = uVar38 ^ 0x8fa0bf20 ^ *(uint *)(puVar64 + (ulong)(uVar6 >> 0x10 & 0xff ^ 0xe7) * 4 + -0xb)
          ^ uVar5 + 0xe4b91e3 + (uVar5 & 0xe4b91e3) * -2;
  uVar12 = (*(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0x77) * 4 + -3) + -0x232f2f63) -
           (*(int *)(puVar69 + (ulong)(uVar5 & 0xff ^ 0x77) * 4 + -3) * 2 + 0x1d0a7d74U & 0x1c9723c6
           ) ^ *(uint *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0xca) * 4 + -0xb);
  uVar6 = uVar11 >> 0x18 ^ 0x33c08c8a;
  uVar6 = *(uint *)(puVar65 + (ulong)(uVar11 >> 0x18 ^ 0x9a) * 4 + -3) ^
          *(uint *)(puVar64 + (ulong)(uVar5 >> 0x10 & 0xff ^ 0x92) * 4 + -0xb) ^
          (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xfc) * 4 + -3) + -0x232f2f63) -
          (*(int *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0xfc) * 4 + -3) * 2 + 0x1d0a7d74U & 0x1c9723c6
          ) ^ (uVar6 + 0x377d6c81) - ((uVar6 & 0x81) << 1 | 0x66801800);
  uVar33 = uVar52 >> 0x18 ^ 0x33c08c1d;
  uVar33 = *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0xd) * 4 + -3) ^
           (uVar33 + 0x377d6c81) - ((uVar33 & 0x81) << 1 | 0x66801800);
  uVar12 = uVar10 >> 0x18 ^ 0x33c08c03 ^
           *(uint *)(puVar65 + (ulong)(uVar10 >> 0x18 ^ 0x13) * 4 + -3) ^
           uVar12 + 0x377d6c81 + (uVar12 & 0x377d6c81) * -2;
  uVar7 = uVar5 >> 0x18 ^ 0x33c08c5d;
  uVar7 = *(uint *)(puVar65 + (ulong)(uVar5 >> 0x18 ^ 0x4d) * 4 + -3) ^
          (uVar7 + 0x377d6c81) - ((uVar7 & 0x81) << 1 | 0x66801800);
  uVar33 = uVar33 + 0xe4b91e3 + (uVar33 & 0xe4b91e3) * -2 ^
           *(uint *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0xb9) * 4 + -0xb) ^
           *(int *)(puVar69 + (ulong)(uVar10 & 0xff ^ 0x24) * 4 + -3) + 0xce853ebaU;
  uVar12 = uVar42 ^ uVar45 ^ uVar54 ^
           *(uint *)(puVar62 + (ulong)(uVar11 >> 8 & 0xff ^ 0xf2) * 4 + -2) ^
           uVar12 + 0x4eb7d448 + (uVar12 & 0x4eb7d448) * -2 ^ 0x86e37f37;
  uVar5 = uVar33 + 0x4eb7d448 + (uVar33 & 0x4eb7d448) * -2 ^
          *(uint *)(puVar62 + (ulong)(uVar5 >> 8 & 0xff ^ 0xf1) * 4 + -2);
  uVar11 = uVar7 + 0xe4b91e3 + (uVar7 & 0xe4b91e3) * -2 ^
           *(uint *)(puVar64 + (ulong)(uVar10 >> 0x10 & 0xff ^ 0xf7) * 4 + -0xb) ^
           *(int *)(puVar69 + (ulong)(uVar11 & 0xff ^ 0xe3) * 4 + -3) + 0xce853ebaU;
  uVar52 = uVar3 ^ uVar45 ^ *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0xd0) * 4 + -2) ^
           uVar11 + 0x4eb7d448 + (uVar11 & 0x4eb7d448) * -2 ^ 0x4fd66f27;
  uVar52 = uVar52 + 0x56031cbe + (uVar52 & 0x56031cbe) * -2 ^ uVar50;
  uVar11 = uVar46 ^ 0x525cd2bd ^ uVar5 + (uVar5 & 0x7a157d8e) * -2 + 0x7a157d8e;
  uVar3 = uVar3 ^ uVar44 ^ 0x4f5c5bdc ^
          *(uint *)(puVar62 + (ulong)(uVar10 >> 8 & 0xff ^ 0xa1) * 4 + -2) ^
          uVar6 + 0x4eb7d448 + (uVar6 & 0x4eb7d448) * -2;
  uVar50 = uVar3 + 0x56031cbe + (uVar3 & 0x56031cbe) * -2 ^ uVar50;
  uVar54 = uVar12 + 0x457b9fbb + (uVar12 & 0x457b9fbb) * -2 ^ uVar54;
  puVar62 = tables[iVar57 + 0x7b];
  puVar65 = tables[iVar57 + 0x52];
  puVar69 = tables[iVar57 + 0x2c];
  uVar3 = *(uint *)(puVar65 + (ulong)(uVar52 >> 0x18 ^ 0xf) * 4 + -10) ^ 0x6e4f6830 ^
          *(uint *)(puVar69 + (ulong)(uVar50 & 0xff ^ 0x7a) * 4 + -6);
  puVar64 = tables[iVar57 + 3];
  uVar10 = (*(int *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0xee) * 4 + -0xb) + -0x6ddeed1c) -
           (*(int *)(puVar64 + (ulong)(uVar11 >> 0x10 & 0xff ^ 0xee) * 4 + -0xb) * 2 + 0x42ace458U &
           0xe1954170) ^ *(uint *)(puVar62 + (ulong)(uVar50 >> 8 & 0xff ^ 0x4c) * 4 + -3);
  uVar10 = uVar10 + 0x1f226eb6 + (uVar10 & 0x1f226eb6) * -2 ^
           *(uint *)(puVar69 + (ulong)(uVar52 & 0xff ^ 0x84) * 4 + -6);
  uVar5 = (*(int *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x1b) * 4 + -0xb) + -0x6ddeed1c) -
          (*(int *)(puVar64 + (ulong)(uVar52 >> 0x10 & 0xff ^ 0x1b) * 4 + -0xb) * 2 + 0x42ace458U &
          0xe1954170) ^ *(uint *)(puVar62 + (ulong)(uVar54 >> 8 & 0xff ^ 0xcd) * 4 + -3);
  uVar5 = uVar5 + 0x1f226eb6 + (uVar5 & 0x1f226eb6) * -2 ^
          *(uint *)(puVar69 + (ulong)(uVar11 & 0xff ^ 1) * 4 + -6);
  uVar12 = *(uint *)(puVar69 + (ulong)(uVar54 & 0xff ^ 0x3c) * 4 + -6) ^ 0x6e4f6830 ^
           *(uint *)(puVar65 + (ulong)(uVar11 >> 0x18 ^ 0x24) * 4 + -10) ^
           *(int *)(puVar64 + (ulong)(uVar50 >> 0x10 & 0xff ^ 0xd8) * 4 + -0xb) + 0x2156722cU;
  uVar5 = uVar47 ^ 0x95821f3f ^ *(uint *)(puVar65 + (ulong)(uVar50 >> 0x18 ^ 0x67) * 4 + -10) ^
          uVar5 + 0x716d0686 + (uVar5 & 0x716d0686) * -2;
  uVar10 = uVar49 + (uVar49 & 0x56031cbe) * -2 + 0x56031cbe ^ 0xe8b64565 ^
           *(uint *)(puVar65 + (ulong)(uVar54 >> 0x18 ^ 0x15) * 4 + -10) ^
           uVar10 + 0x716d0686 + (uVar10 & 0x716d0686) * -2;
  uVar3 = uVar45 ^ uVar47 ^ 0xc0df89ef ^ uVar3 + 0x70caa0b8 + (uVar3 & 0x70caa0b8) * -2 ^
          *(uint *)(puVar62 + (ulong)(uVar11 >> 8 & 0xff ^ 0x24) * 4 + -3) ^
          *(int *)(puVar64 + (ulong)(uVar54 >> 0x10 & 0xff ^ 0xc3) * 4 + -0xb) + 0x2156722cU;
  uVar52 = uVar48 + (uVar48 & 0x56031cbe) * -2 + 0x56031cbe ^ 0x4b867b29 ^
           *(uint *)(puVar62 + (ulong)(uVar52 >> 8 & 0xff ^ 0x61) * 4 + -3) ^
           uVar12 + 0x70caa0b8 + (uVar12 & 0x70caa0b8) * -2;
  puVar69 = tables[iVar57 + 0x46];
  pbVar51[0xe] = puVar69[((ulong)(uVar52 >> 8 & 0xff) ^ 10) - 6] ^
                 ((byte)(uVar52 >> 8) ^ 0x9f) + 0x78 ^ 0x31;
  puVar62 = tables[iVar57 + 0xa8];
  pbVar51[7] = (puVar62[((ulong)(uVar3 & 0xff) ^ 0xee) - 10] - ((byte)uVar3 ^ 0x65)) - 3 ^ 0x73;
  bVar55 = (byte)(uVar10 >> 8) ^ 0x5e;
  pbVar51[10] = puVar69[((ulong)(uVar10 >> 8 & 0xff) ^ 0xcb) - 6] ^
                (bVar55 - (bVar55 * '\x02' - 0x10 & 0xb2)) - 0x2f ^ 0x80;
  pbVar51[3] = (puVar62[((ulong)(uVar5 & 0xff) ^ 0xef) - 10] - ((byte)uVar5 ^ 100)) - 3 ^ 0x8b;
  puVar64 = tables[iVar57 + 0x91];
  pbVar51[8] = ((byte)(uVar10 >> 0x18) ^ 0xde) + puVar64[((ulong)(uVar10 >> 0x18) ^ 0xe8) - 7] +
               0xa5 ^ 0xa6;
  pbVar51[4] = ((byte)(uVar3 >> 0x18) ^ 0x28) + puVar64[((ulong)(uVar3 >> 0x18) ^ 0x1e) - 7] + 0xa5
               ^ 0xda;
  puVar65 = tables[iVar57 + 0x79];
  pbVar51[1] = puVar65[((ulong)(uVar5 >> 0x10 & 0xff) ^ 0xc2) - 0xf] ^ 0x36;
  bVar55 = (byte)(uVar5 >> 8) ^ 0x6d;
  pbVar51[2] = (bVar55 - (bVar55 * '\x02' - 0x10 & 0xb2)) - 0x2f ^
               puVar69[((ulong)(uVar5 >> 8 & 0xff) ^ 0xf8) - 6] ^ 0xa9;
  pbVar51[0xd] = puVar65[((ulong)(uVar52 >> 0x10 & 0xff) ^ 0x1d) - 0xf] ^ 0x6a;
  *pbVar51 = ((byte)(uVar5 >> 0x18) ^ 0x41) + puVar64[((ulong)(uVar5 >> 0x18) ^ 0x77) - 7] + 0xa5 ^
             0xe6;
  pbVar51[0xb] = (puVar62[((ulong)(uVar10 & 0xff) ^ 0x6a) - 10] - ((byte)uVar10 ^ 0xe1)) - 3 ^ 0xd6;
  pbVar51[5] = puVar65[((ulong)(uVar3 >> 0x10 & 0xff) ^ 0x2c) - 0xf] ^ 0x1a;
  bVar55 = (byte)(uVar3 >> 8) ^ 0x15;
  pbVar51[6] = (bVar55 - (bVar55 * '\x02' - 0x10 & 0xb2)) - 0x2f ^
               puVar69[((ulong)(uVar3 >> 8 & 0xff) ^ 0x80) - 6] ^ 0xe4;
  pbVar51[9] = puVar65[((ulong)(uVar10 >> 0x10 & 0xff) ^ 0x2e) - 0xf] ^ 0x3d;
  pbVar51[0xf] = (puVar62[((ulong)(uVar52 & 0xff) ^ 0xf8) - 10] - ((byte)uVar52 ^ 0x73)) - 3 ^ 0xa2;
  pbVar51[0xc] = ((byte)(uVar52 >> 0x18) ^ 0x5a) + puVar64[((ulong)(uVar52 >> 0x18) ^ 0x6c) - 7] +
                 0xa5 ^ 0x65;
  uVar52 = (uint)(iVar58 + 0x8758a9d4U < 0x62b75989 || 0x62b75999 < iVar58 + 0x8758a9d4U);
  uVar10 = uVar52 ^ 1;
  *plVar1 = *plVar1 + -0x10;
  uVar3 = (iVar68 - (iVar68 * 2 + 0x413774e0U & 0x4425a6f0)) + 0xc2ae8de8;
  iVar58 = (uVar3 ^ 0x6d2e6ca7) + (uVar3 * 2 & 0x9e797fbe ^ 0x42126b0) + -0x40010d0;
  iVar68 = (uVar10 | uVar10 << 3) + iVar57 + uVar52 * -0x29 + 0xe +
           (uint)(iVar58 == 0x4b3caf0f) * 0x13;
  do {
    uVar59 = (ulong)(iVar58 + 0xb4c350f0);
    uVar71 = uVar59 & 0xf;
    pbVar51[uVar59] =
         tables[iVar68 + -0x2d][uVar71 - 2] ^ pbVar51[uVar59] ^
         tables[iVar68 + 0x84][uVar71 - 2] ^ (char)uVar71 * '\v'
         ^ tables[iVar68 + -0xd][uVar71];
    iVar58 = iVar58 + -1;
    iVar68 = iVar68 + (uint)(iVar58 == 0x4b3caf0f) * 0x13;
  } while (iVar58 != 0x4b3caf0f);
  *param_1 = 0x44df225b;
  return;
}


static int from_hex(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}
static void parse16(const char *text, uint8_t out[16]) {
    if (strlen(text) != 32) { fprintf(stderr, "expected 32 hex chars\n"); exit(2); }
    for (int i = 0; i < 16; i++) {
        int hi = from_hex(text[i * 2]), lo = from_hex(text[i * 2 + 1]);
        if (hi < 0 || lo < 0) { fprintf(stderr, "bad hex\n"); exit(2); }
        out[i] = (uint8_t)((hi << 4) | lo);
    }
}
static void transform_one(const char *source_hex, const char *work_hex,
                          const char *constant_hex) {
    uint8_t work[16], constant[16], io[128] = {0}, context[512] = {0};
    parse16(source_hex, io + 0x20);
    parse16(work_hex, work);
    parse16(constant_hex, constant);
    uint64_t arg[8] = {0};
    arg[1] = (uint64_t)context;
    arg[2] = (uint64_t)work;
    arg[3] = (uint64_t)constant;
    arg[5] = (uint64_t)(io + 0x60);
    arg[6] = (uint64_t)(io + 0x20);
    uint32_t encoded = ((uint32_t)(uintptr_t)arg ^ 0xd0acfb92U) * 0x66cb1a07U;
    ((uint32_t *)arg)[1] = encoded ^ 0x62U;
    ((uint32_t *)arg)[14] = encoded ^ 0x6d32f7e4U;
    _fp_dh_b61f5b64e1495c6c24e155e10f02a1db((undefined4 *)arg);
    for (int i = 0; i < 16; i++) printf("%02x", io[0x60 + i]);
    printf("\n");
    fflush(stdout);
}
int main(int argc, char **argv) {
    bool batch = argc == 3 && strcmp(argv[1], "--batch") == 0;
    if (!batch && argc != 5) {
        fprintf(stderr, "usage: %s const-data.bin source16 work16 constant16\n"
                        "       %s --batch const-data.bin < requests.txt\n", argv[0], argv[0]);
        return 2;
    }
    FILE *fp = fopen(batch ? argv[2] : argv[1], "rb");
    if (!fp || fread(const_data, 1, sizeof const_data, fp) != sizeof const_data ||
        fgetc(fp) != EOF) { fprintf(stderr, "constant data missing or wrong size\n"); return 2; }
    fclose(fp);
    for (int i = 0; i < 0x128; i++)
        tables[i] = table_offsets[i] < 0 ? NULL : const_data + table_offsets[i];
    if (batch) {
        char line[128], source[33], work[33], constant[33], extra;
        while (fgets(line, sizeof line, stdin)) {
            if (sscanf(line, "%32s %32s %32s %c", source, work,
                       constant, &extra) != 3) {
                fprintf(stderr, "invalid batch request\n");
                return 2;
            }
            transform_one(source, work, constant);
        }
        if (ferror(stdin)) { fprintf(stderr, "batch input failed\n"); return 2; }
    } else {
        transform_one(argv[2], argv[3], argv[4]);
    }
    return 0;
}
