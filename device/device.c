#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/malloc.h>

#ifndef PASSWORD
#define PASSWORD "secret_password"
#endif /* PASSWORD */

/* Function prototypes */
d_open_t	open;
d_close_t	close;
d_read_t	read;
d_write_t	write;

static struct cdevsw device_cdevsw = {
	.d_version = 	D_VERSION,
	.d_open	=	open,
	.d_close =	close,
	.d_read =	read,
	.d_write =	write,
	.d_name =	"sr0"
};

static char buf[512];
static size_t len;

int
open(struct cdev *dev, int flag, int otyp, struct thread *td)
{
	return 0;
}

int
close(struct cdev *dev, int flag, int otyp, struct thread *td)
{
	return 0;
}

int
write(struct cdev *dev, struct uio* uio, int flag)
{
	int error, success = 0;
	char *token, *copy, delim[1] = { '\n' };

	error = copyinstr(uio->uio_iov->iov_base, &buf, 512, &len);
	copy = buf;
	if (error == 0)
	{
		token = strsep(&copy, delim);
		do
		{
			if (strcmp(token, PASSWORD) == 0)
			{
				uprintf("Well done\n");
				success = 1;
				break;
			}
		} while ((token = strsep(&copy, delim)) != NULL);

		if (success == 0)
			uprintf("Try again\n");
	}
	else
		uprintf("Error writing\n");

	return 0;
}

int
read(struct cdev *dev, struct uio *uio, int flag)
{
	uprintf("What's the password?\n");
	return 0;
}

/* Reference to the device in devfs */
static struct cdev *sdev;

/* The function called at load / unload */
static int
load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd)
	{
		case MOD_LOAD:
			sdev = make_dev(
				&device_cdevsw,
				0,
				UID_ROOT,
				GID_WHEEL,
				0600,
				"sr0"
			);
			break;

		case MOD_UNLOAD:
			destroy_dev(sdev);
			break;

		default:
			error = EOPNOTSUPP;
			break;
	}

	return error;
}

DEV_MODULE(device, load, NULL);
