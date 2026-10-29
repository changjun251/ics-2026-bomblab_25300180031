#include "defuser.h"
#include <string.h>
#include <inttypes.h>
#include <stdio.h>

#define MAX_FIELDS 8

/*
 * 把 values 中的 count 个整数写成一行“空格分隔的无符号十进制整数”，存入 out。
 * 成功返回 0；count 为 0（还没有实现）、snprintf 出错或结果被截断时返回非零。
 */
static int write_answer(char *out, size_t cap, const uint32_t *values, size_t count) {
    size_t used = 0;
    for (size_t i = 0; i < count; ++i) {
        int size = snprintf(out + used, cap - used, "%s%" PRIu32, i ? " " : "", values[i]);
        if (size < 0 || (size_t)size >= cap - used) return 1;
        used += (size_t)size;
    }
    return count == 0;
}

int solve_phase_1(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[1] = {0};
    size_t count = 1;
    uint64_t x = challenge;
    uint32_t x_shr = (uint32_t) (x >> 32);

    uint32_t a1 = ((uint32_t)((x >> 16) + 0xbf21)) ^ ((uint32_t)(5 * (x >> 32)));
    uint32_t a2 = (uint32_t)((x_shr ^ 0xa32b) + 4 * (uint32_t)x);
    values[0] = ((uint32_t)(a2 + 2 * a1)) ^ 0x9bd1; 
    return write_answer(out, cap, values, count);
}

int solve_phase_2(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[6] = {0};
    size_t count = 6;
    uint64_t x = challenge;
    uint32_t temp = ((uint32_t)(x >> 7)) * 2;
    values[0] = (temp & 0x1e) + 0x1a;
    int d = 0;
    int c = 13;
    for (int i = 1; i <= 5; i++) {
        d += 15;
	c += 3;
        temp = ((uint32_t)(x >> (13 + 3*i))) & 0x3f;
	temp += values[i - 1];
	temp = temp ^ d;
        values[i] = temp;
    }
    return write_answer(out, cap, values, count);
}

int solve_phase_3(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[2] = {0};
    size_t count = 2;

    uint32_t res = 0; //result	
    uint64_t x = challenge;
    values[0] = x & 7;
    
    uint64_t val1;
    uint32_t val2;
    val1 = x >> 3;
    val2 = (uint32_t)((x >> 32) ^ val1);
	
    uint32_t map[8] = {7, 6, 4, 2, 5, 1, 0, 3};
    uint32_t d = val2;
    switch (map[values[0]]) {
        case 0: {
            res = (d ^ 0x976f) + (d >> 2);
            break;
        }
        case 1: {
            res = 0x2495 + 3 * d;
            break;
        }
        case 2: {
            res = ((uint32_t)(2 * d)) ^ 0xa118;
            break;
        }
        case 3: {
            res = 0x2d442 + 5 * d;
            break;
        }
        case 4: {
            res = (d ^ 0x10ed) + (d >> 1);
            break;
        }
        case 5: {
            res = 0x145e1 + 5 * d;
            break;
        }
        case 6: {
            res = (2 * d) ^ 0x32a3;
	    break;
        }
        case 7: {
            res = 5 * d + 0x38ea;
	    break;
        }
    }
    values[1] = res;
    return write_answer(out, cap, values, count);
}

uint32_t p4_assist(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4);

int solve_phase_4(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[1] = {0};
    size_t count = 1;
    uint64_t x = challenge;
    uint32_t a1 = ((uint32_t)x) & 0x7f;
    uint32_t a2 = 0;
    uint32_t a3 = 0x7f;
    uint32_t a4 = (uint32_t)((x >> 32) ^ (x >> 7));
    values[0] = p4_assist(a1, a2, a3, a4);
     
    return write_answer(out, cap, values, count);
}

uint32_t p4_assist(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4) {
    uint32_t mid = a2 + ((a3 - a2) >> 1);
    if (mid == a1) {
        return a4 ^ (0xc0 + a1);
    }
    else if (mid < a1) {
        uint32_t res = p4_assist(a1, mid + 1, a3, a4);    //递归函数return值
	return ((mid + a4) ^ (3 * res))	^ 0x1f;				 
    }
    else {
	uint32_t res = p4_assist(a1, a2, mid - 1, a4);
	return ((mid + a4) ^ (3 * res)) ^ 0x0e;
    }
}

struct node{
    struct node* next;
    uint32_t val1;
    uint32_t val2;
};

struct node node1, node2, node3, node4, node5, node6, node7, node8, node9, node10, node11, node12;

// 3. 按照你的要求生成 node1 ~ node12 (一一对应 0x405b60 ~ 0x405c10)
struct node node1  = { &node10, 0x00000537, 0x00005be8 };  // 0x405b60
struct node node2  = { &node3,  0x0000013f, 0x00008483 };  // 0x405b70
struct node node3  = { &node7,  0x00000626, 0x00004705 };  // 0x405b80
struct node node4  = { NULL,    0x00000825, 0x0000f1fe };  // 0x405b90
struct node node5  = { &node1,  0x00000786, 0x00006e44 };  // 0x405ba0
struct node node6  = { &node5,  0x00000326, 0x00008612 };  // 0x405bb0
struct node node7  = { &node9,  0x00000b89, 0x0000e70a };  // 0x405bc0
struct node node8  = { &node6,  0x00000433, 0x0000b23d };  // 0x405bd0
struct node node9  = { &node11, 0x00000244, 0x0000e41e };  // 0x405be0
struct node node10 = { &node12, 0x0000093b, 0x00004a00 };  // 0x405bf0
struct node node11 = { &node4,  0x0000004e, 0x00003f98 };  // 0x405c00
struct node node12 = { &node2,  0x00000a2a, 0x00009419 };  // 0x405c10
							   //
							   //
struct node *p5_heads[8] = {
    &node8,  
    &node6,  
    &node5,  
    &node1,  
    &node10,  
    &node12, 
    &node2,  
    &node3
};

int solve_phase_5(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[2] = {0};
    size_t count = 2;
    uint64_t x = challenge;
    struct node * node0 = p5_heads[x & 7];
    values[1] = node0->val1;
    uint32_t eax = (uint32_t)((x >> 32) ^ (x >> 6));
    eax = eax ^ (node0->val2);
    eax = eax ^ 0xe025;
    eax += node0->val1;
    uint32_t loop_cap = ((uint32_t)(x >> 3)) & 0x3;
    for (uint32_t i = 1; i <= loop_cap; i++){
        node0 = node0 -> next;
        eax = node0->val1 + ((node0->val2) ^ eax);
	values[1] = node0->val1;
    }
    values[0] = eax;
    return write_answer(out, cap, values, count);
}

uint32_t func_ptr(uint32_t id, uint32_t a) {
    uint32_t temp;	
    switch (id){
        case 0:
	case 3:
	    return ((a >> 2) | (a << 30)); 
        case 1:
            return ((a ^ 0x115c) + 8 * a);
        case 2:
            return (a + 0xea6e) ^ (a >> 1);
        case 4:
	case 7:    
            return (0x5ebd + (a << 7)) ^ (3*a);
        case 5:
            temp = (a << 5) | (a >> 27);	    
            return temp ^ 0xffe7;
	case 6:
	    return ((a >> 2)^a) + 0xe2a3; 
    }
    return -1;
}
int solve_phase_6(uint64_t challenge, char *out, size_t cap) {
    uint32_t values[1] = {0};
    size_t count = 1;
    uint64_t x = challenge;
    uint32_t ebx = (uint32_t)(0x38 & x);
    uint32_t edi = (uint32_t)((x >> 32) ^ (x >> 6));
    uint32_t res; //result
    res = func_ptr(0x7 & x, edi);
    edi = res ^ 0x28bb;
    res = func_ptr(ebx / 8, edi);
    res += 0x3572; 
    values[0] = res;
    return write_answer(out, cap, values, count);
}

uint32_t mix(uint64_t x){
    return (uint32_t)(x ^ (x >> 32));
}
int solve_secret(uint64_t challenge, uint64_t context, char *out, size_t cap) {
    char str0[37] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAOPEN";
    str0[35] = 'N';
    uint32_t values[3] = {0};
    //size_t count = 3;
    uint64_t x = challenge;
    uint64_t y = context;
    //uint32_t x0 = (uint32_t)x;
    uint32_t y0 = (uint32_t)y;
    values[0] = 3 & y0;
    uint32_t arg1 = mix(x) ^ mix(y);  //其实是secret_mix的arg1
    uint32_t temp = (((arg1 << 3) | (arg1 >> 29)) + 0xa4e0) ^ 0xae1;
    values[1] = temp ^ (((uint32_t)(y >> 16)) ^ ((3 & y0) * 0xa4e0));
    values[2] = values[1] ^ ((mix(x) >> 3) + 0xae1 + mix(y) + (3 & y0));

    //拼接：
    size_t len = snprintf(out, cap, "%s%u %u %u", str0, values[0], values[1], values[2]);
    if (len <= 0 || len > cap) return 1;
    return 0;
}
