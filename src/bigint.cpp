
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <bigint.h>

const int kMaxBigintLen = 1024;

int _bigint_add(int radix, int*integer1, int size1, int* integer2, int size2, int* add_out) {
  if ( radix <= 0 || NULL == integer1 || NULL == integer2 || NULL == add_out ) {
    return -1;
  }
  
  if ( size1 >= kMaxBigintLen || size2 >= kMaxBigintLen ) {
    return -1;
  }

  int m_size = std::min(size1, size2);  
  int m_cur = 0;
  int m_sum = 0;
  int m_carry_bit = 0;
  for ( int i = 0; i < m_size; ++i ) {
    m_sum = integer1[i]+integer2[i]+m_carry_bit; 
    m_cur = m_sum%radix;
    m_carry_bit = m_sum/radix;
    add_out[i] = m_cur;
  }
  if ( size1 == m_size && size2 == m_size ) {
    add_out[m_size] = m_carry_bit;
  }
  if ( size1 > m_size ) {
    for ( int i = m_size; i < size1; ++i ) {
    m_sum = integer1[i]+m_carry_bit; 
      m_cur = m_sum%radix;
      m_carry_bit = (m_sum)/radix;
      add_out[i] = m_cur;
    }
  }
  if ( size2 > m_size ) {
    for ( int i = m_size; i < size2; ++i ) {
      m_sum = integer2[i]+m_carry_bit; 
      m_cur = m_sum%radix;
      m_carry_bit = (m_sum)/radix;
      add_out[i] = m_cur;
    }
  }
  return 0; 
}


int _bigint_mul(int radix, int*integer1, int size1, int* integer2, int size2, int* mul_out) {
  if ( radix <= 0 || NULL == integer1 || NULL == integer2 || NULL == mul_out ) {
    return -1;
  }
  
  if ( size1 >= kMaxBigintLen || size2 >= kMaxBigintLen ) {
    return -1;
  }

  int m_cur = 0;
  int m_mul = 0;
  int m_carry_bit = 0;
  for ( int i = 0; i < size1; ++i ) {
    for ( int j = 0; j < size2; ++j ) {
      m_mul = mul_out[i+j]+integer1[i]*integer2[j]+m_carry_bit;
      m_cur = m_mul%radix;
      m_carry_bit = m_mul/radix;
      mul_out[i+j] = m_cur;
    }
    mul_out[i+size2] = m_carry_bit;
    m_carry_bit = 0;
  }
  m_mul = mul_out[size1+size2]+m_carry_bit;
  m_cur = m_mul%radix;
  m_carry_bit = m_mul/radix;
  mul_out[size1+size2] = m_cur;
  return 0; 
}

int parse_input(int radix, const char*input, int*output, int*output_size) {
  if ( radix <= 0 || NULL == input || NULL == output || NULL == output_size ) {
    return -1;
  }

  int m_input_size = strlen(input);
  int m_data = 0;
  int j = 0;
  for ( int i = 0; i < m_input_size; ) {
    sscanf(input+i, "%10d", &m_data);
    while (m_data) {
      output[j++] = m_data%radix;
      m_data = m_data/radix;
    }
    *output_size = j;
    i += 10;
    if ( i >= m_input_size ) {
      break;
    }
  }
  return 0;
}

int bigint_to_str(int*input, int input_len, char*output) {
  if ( NULL == input || input_len <= 0 ) {
    return -1;
  }
  output[0] = 0;
  int m_size = input_len-1;
  while ( m_size >= 0 && 0 == input[m_size--] ){}
  m_size++;
  for ( int i = m_size; i >= 0; i-- ) {
    snprintf(output+strlen(output), sizeof(input[i]), "%d", input[i]);
  }
  return 0;
}

int bigint_add(const char*input_a, const char*input_b, char*output, int radix) {
  if ( NULL == input_a || NULL == input_b || NULL == output ) {
    return 0;
  }
  int m_input_a[kMaxBigintLen] = {0};
  int m_input_b[kMaxBigintLen] = {0};
  int m_output_add[kMaxBigintLen] = {0};
  int m_input_a_len;
  int m_input_b_len;
  parse_input(radix, input_a, m_input_a, &m_input_a_len);
  parse_input(radix, input_b, m_input_b, &m_input_b_len);
  int m_rev = _bigint_add(radix, m_input_a, m_input_a_len, m_input_b, m_input_b_len, m_output_add);
  if ( 0 != m_rev ) {
    return -1;
  }
  
  bigint_to_str(m_output_add, kMaxBigintLen, output);
  
  return 0;
}

int bigint_mul(const char*input_a, const char*input_b, char*output, int radix) {
  if ( NULL == input_a || NULL == input_b || NULL == output ) {
    return 0;
  }
  int m_input_a[kMaxBigintLen] = {0};
  int m_input_b[kMaxBigintLen] = {0};
  int m_output_add[kMaxBigintLen] = {0};
  int m_input_a_len = 0;
  int m_input_b_len = 0;
  parse_input(radix, input_a, m_input_a, &m_input_a_len);
  parse_input(radix, input_b, m_input_b, &m_input_b_len);
#ifdef DEBUG
  printf("input_a:");
  for ( int i = 0; i < m_input_a_len; ++i ) {
    printf("%d", m_input_a[i]);
  }
  printf("\n");
  printf("input_b:");
  for ( int i = 0; i < m_input_b_len; ++i ) {
    printf("%d", m_input_b[i]);
  }
  printf("\n");
#endif
  int m_rev = _bigint_mul(radix, m_input_a, m_input_a_len, m_input_b, m_input_b_len, m_output_add);
  if ( 0 != m_rev ) {
    return -1;
  }
  
  bigint_to_str(m_output_add, kMaxBigintLen, output);
  
  return 0;

}
