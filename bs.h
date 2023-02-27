#ifndef bs_h
#define bs_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define MAX_BUFFER_SIZE (20)


class Golomb
{
private:
	uint8_t* m_pHead; 	 // 指向buf头部指针
    uint8_t* m_pCur;     // 当前指针位置
    uint8_t* m_pEnd;     // 指向buf尾部指针
    int m_nBits_left;    // 当前读取字节的剩余(可用/未读)比特个数
	uint8_t *m_buffer;
public:
	Golomb()
	{
		m_buffer = (uint8_t *)calloc(1, MAX_BUFFER_SIZE);
		m_pHead = m_buffer;    		
		m_pEnd = m_buffer + MAX_BUFFER_SIZE;   
		reset();
	}
	~Golomb()
	{
		if(m_buffer!=NULL) free(m_buffer);
	}
	
	void bs_write_ue(unsigned int val);
	void bs_write_se(int32_t v);
	void bs_write_te(int x, int val);
	int bs_eof();
	void bs_write_u1(uint32_t v);
	void bs_write_u(int n, uint32_t val);
	uint32_t bs_read_u(int n);
	uint32_t bs_read_u1();
	uint32_t bs_read_ue();
	int32_t bs_read_se();
	uint32_t bs_read_te(int x);
	void reset();
	void print_binary_value(uint8_t n);
	void printCode();

};


#endif /* bs_h */
