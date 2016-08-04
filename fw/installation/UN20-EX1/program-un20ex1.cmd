SET putty_path="C:\Program Files (x86)\PuTTY\putty.exe"
SET scp_path="C:\Program Files (x86)\WinSCP\WinSCP.com"
SET key=id_rsa.ppk
SET host=10.0.1.3

: start ftp server on un20ex1
%putty_path% -ssh root@%host% -i %key% -m start-ftp-service.sh

: transfer files using WinSCP to un20ex1
%scp_path% /script=winscp-ftp-commands.txt

: call install script on un20ex1
%putty_path% -ssh root@%host% -v -i %key% -m call-install-index.sh
