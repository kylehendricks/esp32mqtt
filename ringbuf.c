/**
* \file
*   Ring Buffer library
*/
#include <stdio.h>
#include <string.h>
#include "ringbuf.h"

/**
* \brief init a RINGBUF object
* \param r pointer to a RINGBUF object
* \param buf pointer to a byte array
* \param size size of buf
* \param block_size is size of data as block
* \return 0 if successfull, otherwise failed
*/
int32_t rb_init(RINGBUF * r, uint8_t * buf, int32_t size, int32_t block_size)
{
  if ((r == 0) || (buf == 0) || (size < 2)) return -1;

  if ((size % block_size) != 0) return -1;

  r->p_o = r->p_r = r->p_w = buf;

  r->fill_cnt   = 0;
  r->size       = size;
  r->block_size = block_size;

  return 0;
}

/**
* \brief reset a RINGBUF object
* \param r pointer to a RINGBUF object
* \return 0 if successfull, otherwise failed
*/
int32_t rb_reset(RINGBUF * r)
{
  if (r == 0) return -1;

  r->p_r = r->p_w = r->p_o;
  r->fill_cnt = 0;

  return 0;
}

/**
* \brief put a character into ring buffer
* \param r pointer to a ringbuf object
* \param c character to be put
* \return 0 if successfull, otherwise failed
*/
int32_t rb_put(RINGBUF * r, uint8_t * c)
{
  int32_t   i;
  uint8_t * data = c;

  if (r->fill_cnt >= r->size) {
      return -1; // ring buffer is full, this should be atomic operation
  }

  // increase filled slots count, this should be atomic operation
  r->fill_cnt += r->block_size;

  for (i = 0; i < r->block_size; i++) {

    // put character into buffer
    *r->p_w = *data;

    r->p_w ++;
    data ++;
  }

  // rollback if write pointer go pass the physical boundary
  if (r->p_w >= (r->p_o + r->size)) {
    r->p_w = r->p_o;
  }

  return 0;
}

/**
* \brief get a character from ring buffer
* \param r pointer to a ringbuf object
* \param c read character
* \return 0 if successfull, otherwise failed
*/
int32_t rb_get(RINGBUF * r, uint8_t * c)
{
  int32_t   i;
  uint8_t * data = c;

  if (r->fill_cnt <= 0) {
    // ring buffer is empty, this should be atomic operation
    return -1;
  }

  // decrease filled slots count
  r->fill_cnt -= r->block_size;

  for (i = 0; i < r->block_size; i++) {
    // get the character out
    *data++ = *r->p_r++;
  }

  // rollback if write pointer go pass the physical boundary
  if (r->p_r >= (r->p_o + r->size)) {
    r->p_r = r->p_o;
  }

  return 0;
}

int32_t rb_available(RINGBUF * r)
{
  return (r->size - r->fill_cnt);
}

uint32_t rb_read(RINGBUF * r, uint8_t * buf, int len)
{
  int     n = 0;
  uint8_t data;

  while (len > 0) {
    while (rb_get(r, &data) != 0)  ;
    *buf++ = data;
    n++;
    len--;
  }

  return n;
}

uint32_t rb_write(RINGBUF * r, uint8_t * buf, int len)
{
  uint32_t wi;

  for (wi = 0; wi < len; wi++) {
    while (rb_put(r, &buf[wi]) != 0) ;
  }

  return 0;
}
