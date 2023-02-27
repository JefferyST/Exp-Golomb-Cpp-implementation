#include "bs.h"

/* coder */
/** ue*/
void Golomb::bs_write_ue(unsigned int val)
{
    
    int i_size = 0;
    //Number of bits
    static const int i_size0_255[256] =
    {
        1,     
        1,      
        2,2,    
        3,3,3,3,   
        4,4,4,4,4,4,4,4,  
        5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,  
        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,  
        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,  
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,  
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
    };
    
    if( val == 0 ) 
    {
        bs_write_u1(1);
    }
    else
    {
    
        unsigned int tmp = ++val;
       
        if( tmp >= 0x00010000 )
        {
            i_size += 16;
            tmp >>= 16;
        }
      
        if( tmp >= 0x100 )
        {
            i_size += 8;
            tmp >>= 8;
        }
      
        i_size += i_size0_255[tmp];
        bs_write_u(2 * i_size - 1, val );
    }
}

/** se(v)*/
void Golomb::bs_write_se(int32_t v)
{
    if (v <= 0)
    {
        bs_write_ue(-v*2);
    }
    else
    {
        bs_write_ue(v*2 - 1);
    }
}

/** te(v) */
void Golomb::bs_write_te(int x, int val)
{
    if( x == 1 )
    {
        bs_write_u1(1&~val);
    }
    else if( x > 1 )
    {
        bs_write_ue(val);
    }
}


// end_of_file
int Golomb::bs_eof() 
{ 
    if (m_pCur >= m_pEnd) 
		return 1; 
	else 
		return 0; 
}

void Golomb::bs_write_u1(uint32_t val)
{
   
    m_nBits_left--;
    
    if (! bs_eof())
    {
        (*(m_pCur)) |= ((val & 0x01) << m_nBits_left);
    }
    
    if (m_nBits_left == 0) 
	{ 
		m_pCur++; 
		m_nBits_left = 8; 
	}
}

void Golomb::bs_write_u(int n, uint32_t val)
{
    int i;
    for (i = 0; i < n; i++)
    {
        bs_write_u1((val >> ( n - i - 1 ))&0x01 );
    }
}

/**decoder */
/** ue(v) */
uint32_t Golomb::bs_read_ue()
{
    int32_t r = 0; 
    int i = 0;     // leadingZeroBits
    
    //count leadingZeroBits
    while((bs_read_u1() == 0) && (i < 32) && (!bs_eof()))
    {
        i++;
    }
   
    r = bs_read_u(i);
    r += (1 << i) - 1;
    return r;
}


/** read 1 bit */
uint32_t Golomb::bs_read_u1()
{
    uint32_t r = 0; 
	m_nBits_left--;
    
    if (! bs_eof())
    {
        r = ((*(m_pCur)) >>m_nBits_left) & 0x01;
    }
   
    if (m_nBits_left == 0) 
	{ 
		m_pCur++;
		m_nBits_left = 8; 
	}
    
    return r;
}

uint32_t Golomb::bs_read_u(int n)
{
    uint32_t r = 0; 
    int i;  
    for (i = 0; i < n; i++)
    {
        r |= ( bs_read_u1() << ( n - i - 1 ) );
    }
    return r;
}

/** se(v)  */
int32_t Golomb::bs_read_se()
{
   
    int32_t r = bs_read_ue();
  
    if (r & 0x01) 
    {
        r = (r+1)/2;
    }
    else 
    {
        r = -(r/2);
    }
    return r;
}

/** te(v) */
uint32_t Golomb::bs_read_te(int x)
{
    if( x == 1 ) 
    {
        return 1 - bs_read_u1();
    }
    else if( x > 1 )
    {
        return bs_read_ue();
    }
    return 0;
}


/*print*/
void Golomb::print_binary_value(uint8_t n) {
    int byte_n = sizeof(uint8_t);
    int bit_n = byte_n * 8;
    int mask;
    for (int j = 0; j < bit_n; j++) {
        mask = bit_n-1-j;
        putchar((n&(1<<mask)) == 0? '0':'1');
    }
}

void Golomb::printCode()
{
	for (int i = 0; i < MAX_BUFFER_SIZE; i++) {
        print_binary_value(m_buffer[i]);
        printf("\n");
    }
}

void Golomb::reset()
{
	m_pCur = m_buffer;   // 初始位置与start保持一致
    m_nBits_left = 8;    // 默认剩余8比特可用
}
