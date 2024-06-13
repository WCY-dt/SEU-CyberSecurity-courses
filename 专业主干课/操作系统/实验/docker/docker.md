<center><font size="6">docker实验报告</font></center>

<br/>
<center><font size="4">57119101 王晨阳</font></center>
<br/>
<center><font size="4">2021年3月23日</font></center>
<br/>

[TOC]

# 实验目的

了解docker基本操作方法，将`war`包部署到`Tomcat`，并上传dockerhub

# 实验步骤

由于我的电脑很早之前已经安装过docker，故安装步骤省略。

## 制作docker镜像

创建 `docker-admin` 文件夹，并创建 `Dockerfile`

```bash
chenyang@chenyang-ubuntu:~$ mkdir docker-admin
chenyang@chenyang-ubuntu:~$ cd docker-admin
chenyang@chenyang-ubuntu:~/docker-admin$ touch Dockerfile
chenyang@chenyang-ubuntu:~/docker-admin$ gedit Dockerfile
```

修改内容为

```
FROM docker.io/tomcat
MAINTAINER rstyro:
COPY admin.war .user/local/tomcat/webapps
```

构建镜像

```bash
chenyang@chenyang-ubuntu:~/docker-admin$ sudo docker build -t admin:1.0.0 .
Sending build context to Docker daemon  40.92MB
Step 1/3 : FROM docker.io/tomcat
 ---> 08efef7ca980
Step 2/3 : MAINTAINER rstyro:
 ---> Running in 94ec3f3be6a7
Removing intermediate container 94ec3f3be6a7
 ---> 27261aea57c0
Step 3/3 : COPY admin.war /usr/local/tomcat/webapps
 ---> 5f80cd80f3c3
Successfully built 5f80cd80f3c3
Successfully tagged admin:1.0.0
chenyang@chenyang-ubuntu:~/docker-admin$ 
chenyang@chenyang-ubuntu:~/docker-admin$ sudo docker run --name=admin -p 8080:8080 -d admin:1.0.0
82f00fb08d5f92c9124dc3424b0b9accda177f0168512356d2502bb7c756811c
```

查看ip

```bash
chenyang@chenyang-ubuntu:~/docker-admin$ ifconfig
docker0   Link encap:以太网  硬件地址 02:42:27:bc:d7:27  
          inet 地址:172.17.0.1  广播:172.17.255.255  掩码:255.255.0.0
          UP BROADCAST MULTICAST  MTU:1500  跃点数:1
          接收数据包:0 错误:0 丢弃:0 过载:0 帧数:0
          发送数据包:0 错误:0 丢弃:0 过载:0 载波:0
          碰撞:0 发送队列长度:0 
          接收字节:0 (0.0 B)  发送字节:0 (0.0 B)
```

浏览器访问[`172.17.0.1:8080/admin/`](172.17.0.1:8080/admin/)即可查看页面

> 这里原来一直是404，后来通过`sudo ufw disable`关闭防火墙后即可正常访问

## docker镜像上传

镜像打标签

```bash
chenyang@chenyang-ubuntu:~/docker-admin$ sudo docker tag 5f80cd80f3c3 chenyangcn/admin1.0.0
```

登录docker hub

```bash
chenyang@chenyang-ubuntu:~/docker-admin$ sudo docker login
Authenticating with existing credentials...
WARNING! Your password will be stored unencrypted in /root/snap/docker/796/.docker/config.json.
Configure a credential helper to remove this warning. See
https://docs.docker.com/engine/reference/commandline/login/#credentials-store

Login Succeeded
```

推送镜像

```bash
chenyang@chenyang-ubuntu:~/docker-admin$ sudo docker push chenyangcn/admin1.0.0
```

登录看到已经上传

![image](E:%5C20-21-2%5C%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%5C%E5%AE%9E%E9%AA%8C%5Cdocker%5Cimage.png)

# 实验体会

通过本次实验，掌握了对docker基本使用，加深了对操作系统概念的理解。提高了动手能力，解决问题的能力得到强化。

