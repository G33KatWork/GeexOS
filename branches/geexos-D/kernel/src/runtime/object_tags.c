#include <lib/types.h>

#include <runtime/object_tags.h>

size_t object_tags_size; 

int add_tag(size_t size)
{
	object_tags_size += size;
	return -object_tags_size;
}
