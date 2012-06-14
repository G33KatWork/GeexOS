/*
** Copyright (C) 2011 EADS France, stephane duverger <stephane.duverger@eads.net>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <print.h>
#include <arch.h>

typedef union location
{
    void      *addr;
    uint8_t   *u8;
    uint16_t  *u16;
    uint32_t  *u32;
    uint64_t  *u64;

    char*      str;

} __attribute__((packed)) loc_t;

typedef struct buffer
{
    loc_t  data;
    size_t sz; 

} __attribute__((packed)) buffer_t;

static inline void __buf_add(buffer_t *buf, size_t len, char c)
{
    if(buf->sz < len)
        buf->data.str[buf->sz++] = c;
}

static char vprint_buffer[1024];

size_t printf(const char *format, ...)
{
    va_list params;
    size_t  retval;

    va_start(params, format);
    retval = vprintf(format, params);
    va_end(params);

    return retval;
}

size_t snprintf(char *buff, size_t len, const char *format, ...)
{
    va_list params;
    size_t  retval;

    va_start(params, format);
    retval = vsnprintf(buff, len, format, params);
    va_end(params);

    return retval;
}

size_t vprintf(const char *format, va_list params)
{
    size_t retval;

    retval = vsnprintf(vprint_buffer, sizeof(vprint_buffer), format, params);

    char* ptr = vprint_buffer;
    while(*ptr)
    {
        arch_screen_putchar(*ptr);
        ptr++;
    }

    return retval;
}

static inline void __format_add_str(buffer_t *buf, size_t len, char *s)
{
    while(*s)
        __buf_add(buf, len, *s++);
}

static inline void __format_add_chr(buffer_t *buf, size_t len, int c)
{
    __buf_add(buf, len, (char)c);
}

static inline void __format_add_bin(buffer_t *buf, size_t len, int64_t value, uint32_t n)
{
    uint32_t i, bit;

    for(i=0 ; i<n ; i++)
    {
        bit = value & (1<<(n-i));
        __buf_add(buf, len, bit?'1':'0');
    }
}

static inline void __format_add_dec(buffer_t *buf, size_t len, int64_t value)
{
    char     rep[24];
    buffer_t dec;

    if(!value)
        __buf_add(buf, len, '0');

    dec.data.str = rep;
    dec.sz = 0;

    if(value < 0)
    {
        __buf_add(buf, len, '-');
        value = -value;
    }

    while(value)
    {
        dec.data.str[dec.sz++] = (value%10) + '0';
        value /= 10;
    }

    while(dec.sz--)
        __buf_add(buf, len, dec.data.str[dec.sz]);
}

static const char hexChars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
static inline void __format_add_hex(buffer_t *buf, size_t len, uint64_t value)
{
    char     rep[24];
    buffer_t dec;

    if(!value)
        __buf_add(buf, len, '0');

    dec.data.str = rep;
    dec.sz = 0;

    /*if(value < 0)
    {
      __buf_add(buf, len, '-');
      value = -value;
    }*/

    while(value)
    {
      dec.data.str[dec.sz++] = hexChars[(value%16)];
      value /= 16;
    }

    while(dec.sz--)
        __buf_add(buf, len, dec.data.str[dec.sz]);
}

size_t vsnprintf(char *buffer, size_t len, const char *format, va_list params)
{
    buffer_t buf;
    char     c;
    bool     interp;

    buf.data.str = buffer;
    buf.sz = 0;
    interp = false;

    if(len) len--;

    while(*format)
    {
        c = *format++;

    if(interp)
    {
        if(c == 's'){
            char* value = va_arg(params, char*);
            __format_add_str(&buf, len, value);
        } else if(c == 'c'){
            int value = va_arg(params, int);
            __format_add_chr(&buf, len, value);
        } else if(c == 'b'){
            uint64_t value = va_arg(params, uint32_t);
            __format_add_bin(&buf, len, value, 32);
        } else if(c == 'B'){
            uint64_t value = va_arg(params, uint64_t);
            __format_add_bin(&buf, len, value, 64);
        } else if(c == 'd'){
            int64_t value = va_arg(params, int32_t);
            __format_add_dec(&buf, len, value);
        } else if(c == 'D'){
            int64_t value = va_arg(params, int64_t);
            __format_add_dec(&buf, len, value);
        } else if(c == 'x'){
            uint64_t value = va_arg(params, uint32_t);
            __format_add_hex(&buf, len, value);
        } else if(c == 'X'){
            uint64_t value = va_arg(params, uint64_t);
            __format_add_hex(&buf, len, value);
        } else {
            __buf_add(&buf, len, c);
        }
        interp = false;
    }
    else if(c == '%')
        interp = true;
    else
        __buf_add(&buf, len, c);
    }

    buf.data.str[buf.sz++] = 0;
    return buf.sz;
}
