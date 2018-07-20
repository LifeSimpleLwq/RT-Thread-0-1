#include <rtthread.h>


char *rt_strncpy(char *dst,const char *str,rt_ubase_t n)
{
	if(n != 0)
	{
		char *d = dst;
		const char *s = str;
		
		do
		{
			if ((*d++ = *s) == 0)
			{
				while (--n != 0)
					*d++ = 0;
				break;
			}
		}while(--n != 0);
	}
	
	return (dst);
}












