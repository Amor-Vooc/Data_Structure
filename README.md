# CQUPT 重庆邮电大学 A2012930 数据结构B
## Visual Studio 2026 环境下正常运行
## 课程已完结

## Task_1 Call_Record_Management_System & TCP_IP_Protocol_Stack_Simulation

Call_Record_Management_System 程序构建了一个能够高效管理通话记录的系统。采用了面向对象的设计思想，将双向链表作为核心数据结构，通过面向对象的方式将数据（CallRecord）和操作（CallLog）分离，实现了清晰、模块化的设计。不仅支持基本的增删查改功能，还利用双向链表的特性提供了双向遍历和按时间排序的特性。

TCP_IP_Protocol_Stack_Simulation 程序模拟计算机网络中数据包的封装与解封装过程，以及数据在发送端与接收端之间的传输。

## Task_2 BST & HuffmanTree

BST 程序的核心目的是演示二叉排序树的基本操作与应用。从生成随机数建树，到多种方式遍历，再到特定节点的删除，完整地模拟了动态查找表的生命周期。

HuffmanTree 程序的核心目标是：给定一段字符串，根据字符出现的频率构建一棵最优二叉树（哈夫曼树），从而为每个字符生成唯一的前缀二进制编码，以实现数据的压缩。

## Task_3 Constrained_MST & MaxRelPath

MaxRelPath 是一个基于Dijkstra算法变体的程序，旨在解决“最大可靠性路径（概率乘积最大化）”问题。

Constrained_MST 是一个典型的带约束条件的最小生成树（MST）问题求解程序。程序的核心算法基于 Kruskal 算法，结合了并查集（DSU）来管理连通性，并增加了一个额外的“传输距离”约束判断。

## Task_4 QQ_HashFinder & Student_Rank_System

Student_Rank_System 实现了一个基于单向链表的学生成绩管理系统，实现了按学号排序建立学生信息链表、按成绩降序排序（成绩相同时按姓名升序排序）、查找成绩前N名并输出相关信息（成绩相同时并列名次）功能。

QQ_HashFinder 实现了两种不同的哈希表冲突解决策略：开放定址法（具体为线性探测）和拉链法（链地址法）。通过生成大量随机数据，分别对这两种哈希表进行数据插入和查找操作，并记录其耗时和查找过程中的比较次数，从而直观地比较它们的性能差异。此外，还提供一个用户交互界面，允许用户输入QQ号进行实时查询。