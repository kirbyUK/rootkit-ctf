# Setup Instructions

1. Download FreeBSD 10.3 i386 DVD1
2. Install
    * Select UK keymap
    * Auto disc partitioning is fine
    * Tick system source and untick games
    * Create a root password
    * Default network setup is fine
    * Timezone is Europe -> United Kingdom
    * Make sure sshd and crash dumps are ticked in System Configuration
    * Yes to add users - make a user called 'ctf' with password 'mwr'. Default
      for all other fields.
3. Compile debug kernel
    * Login as root
    * `$ svnlite checkout http://svn.freebsd.org/base/releng/10.3/ /usr/src`
    * `$ cd /usr/src/sys/i386/conf/`
    * `$ vi DEBUG`
    * Add the following:
	    - include GENERIC
        - options KDB_UNATTENDED
        - options DDB
		- options GDB
    * `$ cd /usr/src`
    * `$ make buildkernel KERNCONF=DEBUG INSTKERNNAME=DEBUG`
    * Get a coffee
    * `$ make installkernel KERNCONF=DEBUG INSTKERNNAME=DEBUG`
    * `$ reboot`
4. Configure SSH
    * File is located at `/etc/ssh/sshd_config`
    * Allow password authentication, disable root login
    * service sshd restart
5. Install packages
	* `$ pkg install git lsof sudo cowsay`
6. Configure sudo
    * `$ visudo`
    * Add the following line under `root ALL=(ALL) ALL`
        - `ctf ALL=(root) NOPASSWD: /usr/bin/fstat, /usr/bin/truss, /usr/bin/ktrace, /usr/bin/kdump, /usr/bin/kgdb`
        - It is also recommended to add some dummy commands that would not harm
          the CTF to obfuscate the commands needed to solve the challenge. Some
          good examples include:
             - ifconfig
             - nc
             - ping
             - finger
        - The 'redherring' directory is also provided in the repository with
          dummy binaries, copy them to /usr/bin and add entries for them in the
          sudoers file
7. Build and install kernel module
    * `$ git clone https://github.com/kirbyUK/rootkit-ctf`
    * `$ cd rootkit-ctf/rootkit`
    * `$ make`
    * `$ make install`
    * `$ vi /boot/loader.conf`
        - Add the line `logging_load="YES"`
8. Build and installer logger program
    * `$ cd ../keys`
    * `$ make`
    * `$ make install`
    * Double check `/etc/rc.conf` has the line `keys_enable="YES"`
9. Reboot
