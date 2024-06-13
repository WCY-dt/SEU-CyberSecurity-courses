<center><font size="6">进程调度实验报告</font></center>

<br/>
<center><font size="4">57119101 王晨阳</font></center>
<br/>
<center><font size="4">2021年4月7日</font></center>

<br/>

[TOC]

# Q1

> 假定有四个进程 ($P_1$ - $P_4$)，到达时刻分别是 $0$、$10$、$20$ 和 $40$，优先级分别是 $1$、$2$、$3$ 和 $4$，执行时间分别是 $30$、$20$、$50$ 和 $20$ 毫秒， 这些进程在一个双CPU机器上被调度执行，系统中只有一个就绪队列（ready queue），假定上下文切换的开销为 $0$ 。当分别采用以下可抢占调度算法时，画出按每种调度算法调度的甘特图，并计算其平均等待时间和平均周转时间。
> - 优先级（Priority）调度
> - 轮转(Round Robin)调度，时间片为 $20\text{ms}$
> - 最短作业优先 （Shortest Job First）



| 进程  | 到达时刻 | 优先级 | 执行时间 |
| :---: | :------: | :----: | :------: |
| $P_1$ |   $0$    |  $1$   |   $30$   |
| $P_2$ |   $10$   |  $2$   |   $20$   |
| $P_3$ |   $20$   |  $3$   |   $50$   |
| $P_4$ |   $40$   |  $4$   |   $20$   |

## 1.1 优先级调度

 <img src="E:%5Ctmp%5CTyporaPic%5Cimage-20210411192140812.png" alt="image-20210411192140812" style="zoom: 33%;" />

$平均等待时间=\frac{10+0+0+0}{4}=2.5\text{ms}$

$平均周转时间=\frac{40+20+50+20}{4}=32.5\text{ms}$ 

## 1.2 轮转调度

<img src="E:%5C20-21-2%5C%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%5C%E5%AE%9E%E9%AA%8C%5CThread%20Scheduling%5C1-2.png" alt="1-2" style="zoom: 33%;" /> 

$平均等待时间=\frac{10+0+0+0}{4}=2.5\text{ms}$

$平均周转时间=\frac{40+20+50+20}{4}=32.5\text{ms}$ 

## 1.3 最短作业优先

<img src="E:%5C20-21-2%5C%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%5C%E5%AE%9E%E9%AA%8C%5CThread%20Scheduling%5C1-3.png" alt="1-3" style="zoom: 33%;" /> 

$平均等待时间=\frac{0+0+10+0}{4}=2.5\text{ms}$

$平均周转时间=\frac{30+20+60+20}{4}=32.5\text{ms}$ 



# Q2

> Consider a multi-level feedback queue in a single-CPU system. The first level (queue $0$) is given a quantum of $8$ ms, the second one a quantum of $16$ ms, the third is scheduled FCFS. Assume jobs arrive all at time zero with the following job times (in ms): $4$, $7$, $12$, $20$, $25$ and $30$, respectively. Assume the context switch overhead is zero unless otherwise stated. 
> - Show the Gantt chart for this system.
> - Compute the average waiting and turnaround time. 
> - Suppose the context switch overhead is $1$ ms. Compute the average turnaround time.

## 2.1 Gantt chart

<img src="E:%5C20-21-2%5C%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F%5C%E5%AE%9E%E9%AA%8C%5CThread%20Scheduling%5C2.png" alt="2" style="zoom:33%;" /> 

## 2.2 Average waiting and turnaround time

$\text{Average Waiting Time}=\frac{0+4+35+39+67+68}{6}=30.5\text{ms}$

$\text{Average Turnaround Time}=\frac{4+11+47+59+92+98}{6}=51.83\text{ms}$

## 2.3 Average turnaround time

$\text{Average Turnaround Time}=\frac{4+12+53+66+102+109}{6}=57.67\text{ms}$

