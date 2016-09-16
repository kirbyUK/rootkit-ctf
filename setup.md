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
    * `$ cd /usr/src/sys/i386/conf/`
    * `$ vi DEBUG`
    * Add the following:
		```
		include GENERIC
       	options KDB_UNATTENDED
       	options DDB
		options GDB
		```
    * `$ cd /usr/src`
    * `$ make buildkernel KERNCONF=DEBUG INSTKERNNAME=DEBUG`
    * Get a coffee
    * `$ make installkernel KERNCONF=DEBUG INSTKERNNAME=DEBUG`
    * reboot
4. 
