### 课程开始前的准备

我们的基本环境为linux+oracle，linux+mysql。下面是上课需要用到的一些软件材料。

- 1  VMware Workstation 15 pro(至少12以上)
- 2  OracleLinux-R7-U4-Server-x86_64-dvd.iso https://www.cnblogs.com/mh-study/p/10454687.html
- 3  MobaXterm_Personal_11.1
- 4  LINUX.X64_193000_db_home.zip
- 5  mysql-5.7.30-linux-glibc2.12-x86_64.tar.gz
- 6  rlwrap
- 7  sqlcl-20.3.0.274.1916

以上软件除2外，我发到群里，那个OracleLinux-R7-U4-Server-x86_64-dvd.iso太大，你们从网上自己下载。上课前这些软件要先下载到本机，并先安装好VMware Workstation，其他的我会在课堂上讲怎么安装。

对计算机硬件的要求：主要是内存和硬盘，内存至少4G以上，越大越好，硬盘500G以上。





### 不知道在干什么



五    配置本地Yum软件仓库
`mkdir /iso`
安装盘的镜像文件复制到/iso下

```
mkdir /yum
# mount -o loop /iso/OracleLinux-R7-U4-Server-x86_64-dvd.iso /yum
mount -o loop /iso/OracleLinux-Release7-Update4-x86_64-dvd.iso /yum
```
如果需要启动时自动挂载：
`vim /etc/fstab`
文件追加：

```
/iso/OracleLinux-R7-U4-Server-x86_64-dvd.iso  /yum  iso9660  ro 0 0

cd /etc/yum.repos.d
mv public-yum-ol7.repo public-yum-ol7.repo.bak

vim yum.repo
```

```
[source]
#仓库名称
name=localserver
#仓库描述
baseurl=file:///yum
#仓库地址
gpgcheck=0
#检查软件是否为官方软件(0代表否，1代表是)
enabled=1
#此仓库在系统中是否生效(可不写，默认为1)
gpgkey=file:///yum/RPM-GPG-KEY-oracle

yum clean all #刷新仓库配置
yum repolist all #报告yum仓库的状态
```

六    安装安装oracle需要的RPM包
```
yum -y install binutils*
yum -y install compat*
yum -y install gcc*
yum -y install glibc*
yum -y install ksh
yum -y install libgcc*
yum -y install libstdc*
yum -y install libaio*
yum -y install libXext*
yum -y install libX11*
yum -y install libXau*
yum -y install libxcb*
yum -y install libXi*
yum -y install make*
yum -y install sysstat*
yum -y install oracleasm*
yum -y install /mnt/hgfs/share/kmod-libs-20-28.el7.x86_64.rpm
yum -y install /mnt/hgfs/share/kmod-20-28.el7.x86_64.rpm
```



七    禁用防火墙和SELNUX
```
systemctl stop firewalld.service #停止firewall
systemctl disable firewalld.service #禁止firewall开机启动
firewall-cmd --state #查看默认防火墙状态（关闭后显示not running，开启后显示running）
```
设置`vim /etc/selinux/config` 文件，将SELINUX设置为disabled
`vim /etc/selinux/config`

八  创建目录结构
`mkdir -p /u01/app/oracle/product/19.3.0/dbhome_1`

九  添加组和用户

```
groupadd -g 1001 oinstall
groupadd -g 1200 asmadmin
groupadd -g 1201 asmdba
groupadd -g 1202 asmoper
groupadd -g 1300 dba
groupadd -g 1301 oper

useradd -m -u 1101 -g oinstall -G dba,oper,asmdba -d /home/oracle -s /bin/bash oracle
```
修改密码：
`passwd oracle`
修改目录所有者
`chown -R oracle:oinstall /u01`
修改权限
`chmod -R 775 /u01`



十  配置环境变量

```
su - oracle

vim .bash_profile

ORACLE_SID=db19c;export ORACLE_SID
ORACLE_UNQNAME=db19c;export ORACLE_UNQNAME
JAVA_HOME=/usr/local/java; export JAVA_HOME
ORACLE_BASE=/u01/app/oracle; export ORACLE_BASE
ORACLE_HOME=$ORACLE_BASE/product/19.3.0/dbhome_1; export ORACLE_HOME
ORACLE_TERM=xterm; export ORACLE_TERM
NLS_LANG=american_america.ZHS16GBK; export NLS_LANG
TNS_ADMIN=$ORACLE_HOME/network/admin; export TNS_ADMIN
ORA_NLS11=$ORACLE_HOME/nls/data; export ORA_NLS11
PATH=.:${JAVA_HOME}/bin:${PATH}:$HOME/bin:$ORACLE_HOME/bin:$ORA_CRS_HOME/bin
PATH=${PATH}:/usr/bin:/bin:/usr/bin/X11:/usr/local/bin
export PATH
LD_LIBRARY_PATH=$ORACLE_HOME/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ORACLE_HOME/oracm/lib
LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/lib:/usr/lib:/usr/local/lib
export LD_LIBRARY_PATH
CLASSPATH=$ORACLE_HOME/JRE
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/rdbms/jlib
CLASSPATH=${CLASSPATH}:$ORACLE_HOME/network/jlib
export CLASSPATH
THREADS_FLAG=native; export THREADS_FLAG
export TEMP=/tmp
export TMPDIR=/tmp
umask 022
```

十一  为安装用户设置资源限制
\1)  修改`vim /etc/security/limits.conf`
以 root 用户身份，在节点上，在 `/etc/security/limits.conf` 文件中添加如下内容

```
vim /etc/security/limits.conf

oracle soft nproc 2047
oracle hard nproc 16384
oracle soft nofile 1024
oracle hard nofile 65536 
```

\2)  修改 `vim /etc/pam.d/login`

`session required pam_limits.so`


\3)  shell 的限制

对默认的 shell 启动文件进行以下更改，以便更改所有 Oracle 安装所有者的 ulimit 设置：
```
vim /etc/profile

if [ /$USER = "oracle" ] ||[ /$USER = "grid" ]; then
if [ /$SHELL = "/bin/ksh" ]; then
ulimit -p 16384
ulimit -n 65536
ulimit -s 102400
else
ulimit -u 16384 -n 65536 -s 102400
fi
umask 022
fi
```

十二  修改内核参数`/etc/sysctl.conf`
在sysctl.conf 中加入以下内容：

```
vim /etc/sysctl.conf

kernel.shmmni=4096
net.ipv4.ip_local_port_range=9000 65500
kernel.panic_on_oops=30
fs.file-max=6815744
net.core.rmem_default = 262144
net.core.rmem_max=4194304
net.core.wmem_default=262144
net.core.wmem_max=1048576 
```

`/etc/sysctl.conf`这个目录主要是配置一些系统信息,而且它的内容全部是对应于`/proc/sys/`这个目录的子目录及文件。
设置Linux内核参数 `/etc/sysctl.conf`
配置 Linux 内核参数修改后不用重启动更新: `/sbin/sysctl -p`



### 安装MySQL

1 解压到/usr/local下
mv mysql-5.7.30-linux-glibc2.12-x86_64 mysql
2 创建目录用来存放mysql的数据
mkdir -p /data/mysql
3 创建mysql用户组和用户并修改权限
groupadd mysql
useradd -r -g mysql mysql

chown -R mysql:mysql /data
chmod -R 755 /data

4  配置vim /etc/my.cnf
内容如下：

```
[mysqld]
bind-address=0.0.0.0
port=3306
user=mysql
basedir=/usr/local/mysql
datadir=/data/mysql
socket=/tmp/mysql.sock
log-error=/data/mysql/mysql.err
pid-file=/data/mysql/mysql.pid
#character config
character_set_server=utf8mb4
symbolic-links=0
explicit_defaults_for_timestamp=true
```

5  初始化数据库
进入mysql的bin目录
`cd /usr/local/mysql/bin/`

初始化：
```
./mysqld --defaults-file=/etc/my.cnf --basedir=/usr/local/mysql/ --datadir=/data/mysql/ --user=mysql --initialize
```

6 查看root的默认密码
```
cat /data/mysql/mysql.err
A temporary password is generated for root@localhost: 6yiltTcctk,t
```

7 启动mysql
先将mysql.server放置到/etc/init.d/mysql中
`cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysql`
启动
`service mysql start`

8 修改密码

首先登录mysql，前面的那个是随机生成的。
```
cd /usr/local/mysql/bin/
./mysql -u root -p  #bin目录下
emay<phvp0fH
```
再执行下面三步操作，然后重新登录。
```
SET PASSWORD=PASSWORD('123456');
ALTER USER 'root'@'localhost' PASSWORD EXPIRE NEVER;
FLUSH PRIVILEGES;         
```

这时候你如果使用远程连接……你会发现你无法连接。
这里主要执行下面三个命令(先登录数据库)
```
use mysql                      #访问mysql库
update user set host = '%' where user = 'root';   #使root能在任何host访问
FLUSH PRIVILEGES; 
quit
```

如果不希望每次都到bin目录下使用mysql命令则执行以下命令
`ln -s /usr/local/mysql/bin/mysql  /usr/bin`

9  测试登录
`mysql -u root -p`

10  导入测试数据
```
mysql> source /mnt/hgfs/share/mysql_create_hr.sql
source /mnt/hgfs/share/mysql_create_scott.sql
```





## 20220503

5  配置rlwrap工具实现oracle sqlplus上下键翻页查看功能 
1） root用户： yum -y install readline-devel*
2） 安装rlwrap包（root用户）
复制 rlwrap-0.36目录到/下
cd rlwrap-0.36
./configure && make && make install
3）配置（oracle用户）
[oracle@admin ~]$ vim .bashrc 增加以下两行,如果需要用到rman备份可以事前给rman也造造福.
alias sqlplus='rlwrap sqlplus'
alias rman='rlwrap rman'

source .bashrc 让.bashrc生效

6  简单定制glogin.sql

```
vim $ORACLE_HOME/sqlplus/admin/glogin.sql

set linesize 120
set pagesize 25
set sqlprompt _user"@"_connect_identifier>
define _EDITOR="vim"
```

配置sqldeveloper（随堂作业）
root用户：
yum -y install /mnt/hgfs/share/sqldeveloper-19.2.1.247.2212.noarch.rpm

cd /opt
chown -R oracle:oinstall sqldeveloper

oracle用户：
修改 .bash_profile文件中的path,添加/opt/sqldeveloper
source .bash_profile

sqldeveloper
第一次使用时，需要输入JDK路径，出现图形化界面



8  SQLcl
复制sqlcl目录到/u01下

```
chown -R oracle:oinstall sqlcl

mv $ORACLE_HOME/bin/sql $ORACLE_HOME/bin/sql1
vim .bash_profile
将路径sqlcl/bin添加到path下

sql scott/tiger
```

9  配置EM Express
```
desc dbms_xdb_config
select dbms_xdb_config.gethttpsport from dual;
exec dbms_xdb_config.sethttpsport(6055);
```

https://192.168.11.200:6055/em
默认为JET Oracle EM Express
功能有限，只能看到Performance

如果要回到以前使用的Flash Oracle EM Express
执行脚本：
SQL> @?/rdbms/admin/execemx emx

再切换回JET Oracle EM Express：
SQL> @?/rdbms/admin/execemx omx











## 手工创建数据库(sales)

1  准备初始化参数文件
1,$ s/db19c/sales/g
2  创建相应的目录
3  将sales设为当前例程
4  连接sales例程并启动
5  执行创建数据库的语句
create database sales 
datafile '/u01/app/oracle/oradata/sales/system01.dbf' size 400m
sysaux datafile '/u01/app/oracle/oradata/sales/sysaux01.dbf' size 400m
undo tablespace undotbs1 datafile '/u01/app/oracle/oradata/sales/undotbs01.dbf' size 50m
default temporary tablespace temp tempfile '/u01/app/oracle/oradata/sales/temp01.dbf' size 20m
default tablespace users datafile '/u01/app/oracle/oradata/sales/users01.dbf' size 20m
logfile
group 1 ('/u01/app/oracle/oradata/sales/redo01.log') size 10m,
group 2 ('/u01/app/oracle/oradata/sales/redo02.log') size 10m,
group 3 ('/u01/app/oracle/oradata/sales/redo03.log') size 10m;

6  创建数据字典视图
7  创建口令验证文件
8  创建oracle内部包
9  创建spfile
10  配置监听和服务名



system
sysaux
undo tablespace



















