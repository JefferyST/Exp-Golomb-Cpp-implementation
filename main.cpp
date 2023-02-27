#include "bs.h"

int main(int argc, const char * argv[]) {
	
	Golomb test;
	test.bs_write_ue(0);
    test.bs_write_ue(1);
    test.bs_write_ue(2);
    test.bs_write_ue(3);
	test.bs_write_se(-5);
	test.bs_write_te(3, 9);
	test.printCode();

    test.reset();
    uint32_t code_num0 = test.bs_read_ue();
    uint32_t code_num1 = test.bs_read_ue();
    uint32_t code_num2 = test.bs_read_ue();
    uint32_t code_num3 = test.bs_read_ue();
    uint32_t se_code_num0 = test.bs_read_se();
    uint32_t te_code_num0 = test.bs_read_te(3);
	
	printf("code_num0: %d\n", code_num0);
    printf("code_num1: %d\n", code_num1);
    printf("code_num2: %d\n", code_num2);
    printf("code_num3: %d\n", code_num3);
	printf("se_code_num0: %d\n", se_code_num0);
    printf("te_code_num0: %d\n", te_code_num0);
	
	system("PAUSE");
	return 0;
}