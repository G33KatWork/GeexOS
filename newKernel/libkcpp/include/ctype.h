#ifndef _CTYPE_H
#define _CTYPE_H

static inline int isdigit(int ch)
{
	return (ch >= '0' && ch <= '9');
}

static inline int isxdigit(int ch)
{
	return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

static inline int islower(int ch)
{
	return (ch >= 'a' && ch <= 'z');
}

static inline int isupper(int ch)
{
	return (ch >= 'A' && ch <= 'Z');
}

static inline int isalpha(int ch)
{
	return islower(ch) || isupper(ch);
}

static inline int isspace(int ch)
{
	return (ch == '\t' || ch == '\n' || ch == '\v' || ch == '\f' || ch == '\r' || ch == ' ');
}

static inline int toupper(int ch)
{
	if(isalpha(ch))
		return ch & ~0x20;
	else
		return ch;
}

static inline int tolower(int ch)
{
	if(isalpha(ch))
		return ch | 0x20;
	else
		return ch;
}

#endif
