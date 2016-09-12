# Setup Instructions

1. Download FreeBSD 10.3 i386 DVD1
2. Install
    * Select UK keymap
    * Auto disc partitioning is fine
    * Tick system source and untick games
    * Create a root password
    * Default network setup is fine
    * Timezone is Europe -> United Kingdom
    * Make sure sshd is ticked in System Configuration
    * Yes to add users - make a user called 'ctf' with password 'mwr'. Default
      for all other fields.
3. Install packages
    * Reboot and login as root
    * portsnap fetch
    * portsnap extract
    * pkg install samba43
    * [[ samba config ]]
    * /etc/rc.conf - add 'samba_enable="YES"'
    * service samba start
