#include <ndk/umtypes.h>
#include <ndk/rtlfuncs.h>

/*
 * @implemented
 */
int wctomb (char *mbchar, wchar_t wchar)
{
	return 1;
}

/*
 * @implemented
 */
size_t wcstombs (char *mbstr, const wchar_t *wcstr, size_t count)
{
	return 1;
}

/* EOF */
