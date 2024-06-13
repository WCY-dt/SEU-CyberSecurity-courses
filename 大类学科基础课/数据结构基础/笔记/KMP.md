---
title: KMP
date: 2020-10-1 00:00:21
categories:
- 编程
- 算法
tags:
- KMP
mathjax: true
---

本文介绍一种串的模式匹配算法：KMP

<!-- more -->

![](https://img.shields.io/badge/reproduced-SIAM-blue) ![](https://img.shields.io/badge/reproduced-Wiki-blue) ![](https://img.shields.io/badge/reproduced-cnblog-blue) ![](https://img.shields.io/badge/original-chenyang-blue) ![](https://img.shields.io/badge/license-GPL 3.0-orange) ![](https://img.shields.io/badge/article quality-A-green)



# brute-Force

这是串的模式匹配算法中，最无脑的一种方法。

我们可以这样初始化：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17083647-9dfd3e4a709c40dd98d9817927651960.png)

之后我们只需要比较`i`指针指向的字符和`j`指针指向的字符是否一致。如果一致就都向后移动，如果不一致，如下图：

![img](https://images0.cnblogs.com/blog/416010/201308/17083659-e6718026bf4f48a0be2d5d6076be4c55.png) 

 

`A`和`E`不相等，那就把`i`指针移回第`1`位（假设下标从`0`开始），`j`移动到模式串的第`0`位，然后又重新开始这个步骤：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17083714-7de56d2c1cc84dbfa376cf410ba6f053.png)

时间复杂度高达$O(nm)$

# KMP

该改进算法是由D.E.Knuth ，J.H.Morris和V.R.Pratt提出来的，简称为KMP算法。其改进在于：每当一趟匹配过程出现字符不相等时，主串指示器不用回溯，而是利用已经得到的“部分匹配”结果，将模式串的指示器向右“滑动”尽可能远的一段距离后，继续进行比较。

参考Brute-Force，我们串中的位置指针`i`,`j`来说明，第一个位置下标以`0`开始，我们称为第`0`位。下面看看，如果是人为来寻找的话，肯定不会再把`i`移动回第`1`位，因为主串匹配失败的位置(`i=3`)前面除了第一个A之外再也没有A了，我们为什么能知道主串前面只有一个`A`？因为我们已经知道前面三个字符都是匹配的。移动过去肯定也是不匹配的。有一个想法，**`i`可以不动，我们只需要移动`j`即可**，如下图：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17083828-cdb207f5460f4645982171e58571a741.png)

上面的这种情况还是比较理想的情况，我们最多也就多比较了再次。但假如是在主串`SSSSSSSSSSSSSA`中查找`SSSSB`，比较到最后一个才知道不匹配，然后`i`回溯，这个的效率是显然是最低的。

KMP算法就如同我们上边所看到的一样：利用已经部分匹配这个有效信息，保持`i`指针不回溯，通过修改`j`指针，让模式串尽量地移动到有效的位置。所以，整个KMP的重点就在于当某一个字符与主串不匹配时，我们应该知道`j`指针要移动到哪？

接下来我们自己来发现j的移动规律：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17083912-49365b7e67cd4877b2f501074dae68d2.png)

如图：`C`和`D`不匹配了，我们要把`j`移动到哪？显然是第`1`位。为什么？因为前面有一个`A`相同：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17083929-a9ccfb08833e4cf1a42c30f05608f8f5.png)

如下图也是一样的情况：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17084030-82e4b71b85a440c5a636d57503931415.png)

可以把`j`指针移动到第`2`位，因为前面有两个字母是一样的：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17084037-cc3c34200809414e9421c316ceba2cda.png)

至此我们可以大概看出一点端倪，当匹配失败时，`j`要移动的下一个位置`k`存在着这样的性质：最前面的`k`个字符和`j`之前的最后`k`个字符是一样的。

`P[0..k-1] == P[j-k..j-1]`

这个相当重要，如果觉得不好记的话，可以通过下图来理解：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17084056-66930855432b4357bafbf8d6c76c1840.png)

弄明白了这个就应该可能明白为什么可以直接将`j`移动到`k`位置了。

 上面说的,如果分解成计算机的步骤，则是如下的过程：

- 找出前缀`pre`，设为`pre[0..m]`；

- 找出后缀`post`，设为`post[0..n]`；

- 从前缀`pre`里，先以最大长度的`s[0..m]`为子串，即设`k`初始值为`m`,跟`post[n-m+1..n]`进行比较：
  - 如果相同，则`pre[0..m]`则为最大重复子串，长度为`m`，则`k=m`
  - 如果不相同，则`k=k-1`;缩小前缀的子串一个字符，在跟后缀的子串按照尾巴对齐，进行比较，是否相同。

- 如此下去，直到找到重复子串，或者`k`没找到。

根据上面的求解过程，我们知道子串的`j`位前面，有`j`个字符，前后缀必然少掉首尾一个字符，因此重复子串的最大值为`j-1`，因此知道下一次的j指针最多移到第`j-1`位。 

## `next`数组

好，接下来就是重点了，怎么求这个`k`呢？因为在`P`的每一个位置都可能发生不匹配，也就是说我们要计算每一个位置`j`对应的`k`，所以用一个数组`next`来保存，`next[j]=k`，表示当`T[i]!=P[j]`时，`j`指针的下一个位置。另一个非常有用且恒等的定义，因为下标从`0`开始的，`k`值实际是`j`位前的子串的最大重复子串的长度。请时刻牢记`next`数组的定义，下面的解释是死死地围绕着这个定义来解释的。

```cpp
void getNext(string p,int *next)
{
    next[0]=-1;
    int j=0;
    int k=-1;
	while (j<p.length()-1)
	{
		if (k==-1 || p[j]==p[k]) next[++j]=++k;
        else k = next[k];
	}
 }
```

先来看第一个：当`j`为`0`时，如果这时候不匹配，怎么办？

 ![img](https://images0.cnblogs.com/blog/416010/201308/17084258-efd2e95d3644427ebc0304ed3d7adefb.png)

像上图这种情况，`j`已经在最左边了，不可能再移动了，这时候要应该是`i`指针后移。所以在代码中才会有`next[0] = -1`这个初始化。

如果是当`j`为`1`的时候呢？

 ![img](https://images0.cnblogs.com/blog/416010/201308/17084310-29f9f8dbb6034151a383e7ccf6f5583e.png)

显然，`j`指针一定是后移到`0`位置的。因为它前面也就只有这一个位置了。

下面这个是最重要的，请看如下图：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17084327-8a3cdfab03094bfa9e5cace26796cae5.png) ![img](https://images0.cnblogs.com/blog/416010/201308/17084342-616036472ab546c082aa991004bb0034.png)

请仔细对比这两个图，我们发现一个规律：

当`P[k] == P[j]`时，有`next[j+1] == next[j] + 1`

那如果`P[k] != P[j]`呢？比如下图所示：

![img](https://images0.cnblogs.com/blog/416010/201308/17122358-fd7e52dd382c4268a8ff52b85bff465d.png) 

像这种情况，如果你从代码上看应该是这一句：`k = next[k]`为什么是这样子？你看下面应该就明白了。

 ![img](https://images0.cnblogs.com/blog/416010/201308/17122439-e349fed25e974e7886a27d18871ae48a.png)

现在你应该知道为什么要`k = next[k]`了吧！像上边的例子，我们已经不可能找到`[ A，B，A，B ]`这个最长的后缀串了，但我们还是可能找到`[ A，B ]`、`[ B ]`这样的前缀串的。所以这个过程像不像在定位`[ A，B，A，C ]`这个串，当`C`和主串不一样了（也就是`k`位置不一样了），那当然是把指针移动到`next[k]`啦。

```cpp
int KMP(String t,String p)
{
    int i=0; // 主串的位置
    int j=0; // 模式串的位置
    int next[]=getNext(p);
    while (i<t.length && j<p.length)
    {
        if (j==-1 || t[i]==p[j])
        { 
            // 当j为-1时，要移动的是i，当然j也要归0
            i++;
            j++;
        }
        else
        {
            // i不需要回溯了
            // i=i-j+1;
            j=next[j]; // j回到指定位置
        }
    }
    if (j==p.length) return (i-j);
    else return -1;
}
```

和暴力破解相比，就改动了4个地方。其中最主要的一点就是，i不需要回溯了。

## `next`数组求解算法优化

最后，来看一下上边的算法存在的缺陷。来看第一个例子：

![img](https://images0.cnblogs.com/blog/416010/201308/17084712-f0d6998938764b309f61923452a2b20f.png) 

显然，当我们上边的算法得到的`next`数组应该是`[ -1，0，0，1 ]`

所以下一步我们应该是把`j`移动到第`1`个元素咯：

 ![img](https://images0.cnblogs.com/blog/416010/201308/17084726-790fc1b2c48c411b8011eab9de692f6d.png)

不难发现，这一步是完全没有意义的。因为后面的`B`已经不匹配了，那前面的`B`也一定是不匹配的，同样的情况其实还发生在第`2`个元素A上。

显然，发生问题的原因在于`P[j] == P[next[j]]`。所以我们也只需要添加一个判断条件即可：

```
public static int[] getNext(String ps) {

    char[] p = ps.toCharArray();

    int[] next = new int[p.length];

    next[0] = -1;

    int j = 0;

    int k = -1;

    while (j < p.length - 1) {

       if (k == -1 || p[j] == p[k]) {

           if (p[++j] == p[++k]) { // 当两个字符相等时要跳过

              next[j] = next[k];

           } else {

              next[j] = k;

           }

       } else {

           k = next[k];

       }

    }

    return next;

}
```

该算法复杂度为$O(n+m)$

