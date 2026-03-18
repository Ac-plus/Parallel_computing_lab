# Experiment 3: Matrix Transpose Algorithm Using MPI

## I. Overview of the Experiment

This experiment implements **matrix transposition using MPI-based parallel computing**. By applying **problem decomposition, domain decomposition, and element-wise row–column swapping**, the transpose of a given matrix is computed.

The program is implemented using **MPI communication** and written in **C/C++** on a remote virtual machine connected via **Xshell (CentOS 7.6, icc/icpc)**.

For the implemented program, three matrix sizes are tested:

$$
6000, 12000, 30000
$$

and five problem partition configurations:

$$
1, 4, 9, 16, 25\ processes
$$

(Using one process corresponds to a more complex serial implementation.)

For each configuration, the **execution time** and **computation results** are recorded. Data analysis and performance analysis are then conducted to compare the performance of different approaches.

This experiment adopts the **block chessboard partition method** for matrix transposition.

Assume the number of processors is $p^2$. The matrix can then be divided into **$p\times p$ blocks**, resulting in **$p^2$ submatrices**, as shown in Figure 1.

Each processor performs the transpose operation on its assigned block in parallel:

1. Sub-block transpose
2. Internal transpose within each process domain
3. Merge and output the final result

<img width="1016" height="295" alt="image" src="https://github.com/user-attachments/assets/590b9c6a-61aa-4a02-942a-2bb6cc17773a" />


# II. Parallel Algorithm Analysis and Design

For the MPI-based algorithm described above, the program workflow is shown in **Figure 2**.

Under this partitioning strategy, the matrix is divided into **$\sqrt t \times\sqrt t = t$ square blocks**.

Since each block is assigned to multiple processes performing transposition concurrently, the actual computational cost becomes:

$$
n \times (\sqrt t + n) / (2t)
$$

This is equivalent to reducing the matrix scale (i.e., the number of matrix elements) to **$\frac 1 t$ of the original size**, while the time complexity remains **$O(n^2)$**.

<img width="536" height="823" alt="image" src="https://github.com/user-attachments/assets/5bebe1b9-37ea-47e0-8864-5d40eaeac60c" />


# III. Experimental Data Analysis

## 3.1 Experimental Environment (Cluster)

### (1) CPU Specifications

| Cores | Model                                   | Clock Frequency | Cache  |
| ----- | --------------------------------------- | --------------- | ------ |
| 4     | Six-Core AMD Opteron(tm) Processor 2431 | 2400.085 MHz    | 512 KB |

### (2) Memory Information

| Memory Capacity | Memory Bandwidth |
| --------------- | ---------------- |
| 8174052 kB      | 1000 Mb/s        |

### (3) Network Parameters (Campus WLAN)
```
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST> mtu 1500
inet 10.0.0.6 netmask 255.255.255.0 broadcast 10.0.0.255

eth1: flags=4163<UP,BROADCAST,RUNNING,MULTICAST> mtu 1500
inet 192.168.10.6 netmask 255.255.255.0 broadcast 192.168.10.255

lo: flags=73<UP,LOOPBACK,RUNNING> mtu 65536
inet 127.0.0.1 netmask 255.0.0.0

```

## 3.2 Experimental Data Analysis

Note: In the following figures and tables, **data size refers to the matrix dimension n**.

### 3.2.1 Program Execution Time (ns)

| Processes | 6000     | 12000    | 30000    |
| --------- | -------- | -------- | -------- |
| 1         | 25181770 | 25352716 | 31172990 |
| 4         | 25128533 | 25058746 | 25629520 |
| 9         | 25237798 | 25271892 | 25219917 |
| 16        | 24914264 | 25046825 | 25755167 |
| 25        | 23979663 | 25357723 | 25246858 |

Table 1. Program execution time.



### 3.2.2 Speedup Curve

As shown in **Figure 3**, this figure illustrates the speedup distribution of the MPI partition-based transpose algorithm.

<img width="816" height="481" alt="image" src="https://github.com/user-attachments/assets/74dc3d9a-9dfc-41d1-888f-5a54f69195c7" />



### 3.2.3 Efficiency

As shown in **Figure 4**, the figure illustrates the efficiency distribution of the MPI partition-based transpose algorithm.

<img width="909" height="487" alt="image" src="https://github.com/user-attachments/assets/62084b17-f9c8-4f17-a240-5b0871c50aa8" />


## 3.2.4 Data Analysis

1. The experimental data shows that as the **number of processes and data size increase**, the program execution time generally decreases, which is consistent with **Gustafson’s Law**.

2. The speedup improvement in this experiment is not significant due to several reasons:

   * During matrix transposition, MPI requires **inter-process communication**, since processes in MPI cannot share memory directly. Compared with the pthread method used in previous experiments, **process communication is more time-consuming than thread communication**.
   * Process 0 sends sub-block data to other processes using **MPI_Send()**, and then receives the transposed blocks using **MPI_Recv()**. These operations are performed sequentially, which introduces additional time overhead.
   * MPI processes may spend time **waiting for data transmission or reception**, which further increases execution time.
   * When the matrix size is relatively small, increasing the number of partitions increases **communication and synchronization overhead**, leading to longer execution times.

3. From Figure 3, it can be observed that **larger data sizes lead to higher speedup**, although the improvement is still limited.
   According to **Amdahl’s Law**, increasing the number of parallel processes cannot infinitely increase speedup.
   Moreover, MPI parallelism shows stronger advantages only when the **matrix scale becomes extremely large**, allowing communication overhead to be relatively negligible. In this experiment, due to limited storage space in the CentOS environment, the matrix size only reaches tens of thousands, preventing MPI from demonstrating its full performance potential.

4. Overall analysis shows that for **very large data scales**, parallel methods are slightly more suitable than serial methods, although they incur additional costs such as **network communication and node resources**.
   However, for **smaller data sizes**, the advantages of parallel computation are limited, and serial methods may perform better.

5. As the number of processes increases, the **algorithm efficiency E decreases from 100% (serial)**, because the workload is distributed among multiple processes.



# IV. Experimental Summary

## 4.1 Problems Encountered and Solutions

1. During compilation, the **icpc command** was mistakenly used, which caused errors. It was later realized that MPI programs must be compiled using **mpicxx**.

2. Initially, execution time was measured using `qstat -f`, which reports the **entire job runtime** rather than the precise program runtime. More accurate execution time was obtained by examining compiled `.o` files.

3. Through these experiments, it becomes clear that improving program performance is a complex task. Simply adding threads or calling MPI is insufficient. Many practical factors must be considered, and different methods must be applied depending on the problem characteristics. For example, **matrix transposition may be better suited to multithreading approaches**. This complexity highlights both the **challenge and appeal of parallel computing**.



# 4.2 Understanding of pthread, OpenMP, and MPI Parallel Programming

## 4.2.1 pthread and OpenMP Programming Models

The **pthread model** creates a set of threads at program startup and distributes tasks evenly among them. This approach requires substantial thread management code and does not easily scale with processor availability.

**OpenMP** is a parallel programming model designed for **shared-memory and distributed shared-memory multiprocessor systems**. It allows parallelization by simply adding directives to loops without explicitly specifying thread counts.

Advantages of OpenMP:

* Easier to implement
* Automatically adapts to available processors

Disadvantages:

* Less flexible
* Debugging can be more difficult

Compilation differences:

* **OpenMP** requires compiler directives such as `#pragma`.
* **pthread** is a library where thread creation and management are handled directly by the programmer.



## 4.2.2 MPI Programming Model

Unlike OpenMP and pthread, which rely on **shared memory systems**, **MPI (Message Passing Interface)** is designed for **distributed memory systems**.

Data sharing must be implemented through **message passing**, because MPI programs consist of separate processes that may run on different machines.

To coordinate communication between processes, MPI requires dedicated software such as **MPICH**.

Advantages of MPI:

* Works regardless of whether hardware memory is shared
* Capable of handling very large-scale problems

Disadvantages:

* Algorithms often require significant modification if errors occur
* Performance may be affected by network communication overhead



# V. Course Summary

## 5.1 Positive Aspects of the Course

1. The instructor allocated substantial **hands-on laboratory time**, allowing students to experience different parallel algorithms in practice.

2. Lecture videos uploaded to the **Zhihuishu platform** help reinforce concepts that students may not fully understand during lectures.

3. Teachers and teaching assistants actively answered questions during experiments, helping students solve problems efficiently and improving learning outcomes.


## 5.2 Suggestions for Improvement

1. Online theoretical lectures could include **live programming demonstrations**, since parallel computing is highly practical. Showing real code examples would help students better understand the concepts.

2. Uploaded lecture videos could be **edited into shorter segments** based on different topics. This would make it easier for students to review specific knowledge points and reduce fatigue from watching long recordings.



# Appendix: Analysis of Experiments and Course Concepts

| No. | Experiment Content                  | Theoretical Knowledge                                | Analysis                                                                             |
| --- | ----------------------------------- | ---------------------------------------------------- | ------------------------------------------------------------------------------------ |
| 1   | Analyze speedup and efficiency      | Performance analysis                                 | Speedup, efficiency, and scalability can evaluate parallel program performance       |
| 2   | Implement programs using MPI        | Process concept and MPI programming                  | Processes are the basic unit of resource allocation and can execute concurrent tasks |
| 3   | Remote cluster computing            | Parallel hardware environment and cluster technology | Clusters integrate multiple workstations into a unified computing environment        |
| 4   | Running programs using PBS scripts  | Parallel job management and PBS                      | PBS provides flexible batch processing and job scheduling                            |
| 5   | Limited speedup in parallel methods | Speedup laws                                         | When task size is insufficient, communication overhead can limit speedup             |
