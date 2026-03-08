# Experiment 2: Multithreaded Matrix Transpose Algorithm

## I. Overview of the Experiment

This experiment implements **matrix transposition using multithreaded parallel programming**. Through **problem decomposition, domain decomposition, and element-wise row–column swapping**, the transpose of a given matrix is computed.

Three methods are implemented using **C/C++ multithreading** on a remote virtual machine connected via **Xshell (CentOS 7.6, icc/icpc)**.

For each method, experiments are conducted with five matrix sizes:

60, 600, 3000, 6000, 9000

and five multithreading configurations:

1, 4, 9, 16, 25 threads

(Using one thread corresponds to a more complex serial implementation.)

For each configuration, the **execution time** and **result correctness** are recorded. Data analysis and performance analysis are then conducted to compare the performance of different methods.

### (1) Serial Method

For an **n × n matrix**, two nested `for` loops sequentially scan each row and column.
The element `M[j][i]` is assigned to `M[i][j]`.

### (2) Block Chessboard Partition Method

Assume the number of processors is $p^2$.
The matrix can then be divided into **p × p blocks**, resulting in **p² submatrices**, as shown in **Figure 1**.

Each processor performs transposition on its assigned block in parallel:

1. Sub-block transpose
2. Internal processor transpose

### (3) Rectangular Partition Method

As shown in **Figure 2**, the matrix is partitioned into **p blocks using rectangular splitting**, where each block is approximately equal in size.

Each block is transposed independently and then merged into the final output matrix.

<img width="913" height="342" alt="image" src="https://github.com/user-attachments/assets/ba188e7e-3fde-44ef-9b80-c69feaa81e85" />



# II. Parallel Algorithm Design and Analysis

For the three algorithms above, the following pseudocode is designed.

For matrix initialization, this experiment uses **sequential memory numbering**, where the value of **M[i][j]** equals its position index in the array:

```
M[0][0] = 1, M[0][1] = 2, ..., M[n−1][n−1] = n²
```



## 2.1 Serial Method Pseudocode
```
Input: matrix dimension n
Output: transposed matrix MTij

Start
Initialize each element M[i,j]
for i = 0 to n−1
 for j = 0 to i−1
  M[i,j] = M[j,i]
 end for
end for

Output each M[i,j]
free(Mij)
End
```
Under this method, the total number of transpose operations is:

$$1 + 2 + … + n = n * (1 + n) / 2$$

The time complexity of the algorithm is **O(n²)**.


## 2.2 Block Chessboard Partition Method Pseudocode
```
Input: matrix dimension n, thread count t
Output: transposed matrix MTij

Start

thread_func:
 u = thread_no / sqrt(t)
 v = thread_no % sqrt(t)

 for i = u*m to (u+1)*m − 1
  for j = v*m to (v+1)*m − 1
   perform transpose on sub-block M[i,j]
  end for
 end for

main:
 allocate memory for matrix Mij
 initialize each element M[i,j]

 for i = 0 to t − 1
  create thread i executing thread_func
 end for

 for i = 0 to t − 1
  wait for all threads to finish
 end for

 output each M[i,j]
 free(Mij)

End
```
In this method, the matrix is divided into **sqrt(t) × sqrt(t) = t square blocks**.

Since the blocks are processed concurrently by multiple threads, the effective computational cost becomes:

$$n * (\sqrt{t} + n) / (2t)$$

which is equivalent to reducing the matrix scale (number of elements) to **1/t of the original size**.


## 2.3 Rectangular Partition Method Pseudocode
```
Input: matrix dimension n, thread count t
Output: transposed matrix MTij

Start

thread_func:
 begin = ...

 if current thread is the last thread
  end = n
 else
  end = ...

 for i = begin to end − 1
  for j = 0 to i
   perform transpose on sub-block M[i,j]
  end for
 end for

main:
 allocate memory for matrix Mij
 initialize each element M[i,j]

 for i = 0 to t − 1
  create thread i executing thread_func
 end for

 for i = 0 to t − 1
  wait for all threads to finish
 end for

 output each M[i,j]
 free(Mij)

End
```
The rectangular partition method divides the matrix **unevenly along one dimension**.

To ensure roughly balanced workload among processors **P₁, P₂, …, Pn**, the following condition must hold for any k (k = 2,3,…,n):

$$i_k^2 − i_{k−1}^2 = i_{k−1}^2 − i_{k−2}^2$$

For each sub-block, the required number of operations becomes proportional to:

$$n^2 / \sqrt{t}$$

which is equivalent to reducing the matrix scale to $1 / \sqrt{t}$ of the original size.



Overall, the three algorithms divide matrix **Mij** into **n/t blocks**, then assign each block to a thread using **pthread_create**. After all threads finish their tasks and synchronize using **pthread_join**, the final result **MTij** is obtained.

Unlike the previous PI experiment, **mutex locks are unnecessary**, because each thread operates on **different matrix sub-blocks without overlap**, so there are no shared resources requiring mutual exclusion.



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
eth0:
IP: 10.0.0.6
Netmask: 255.255.255.0

eth1:
IP: 192.168.10.6
Netmask: 255.255.255.0

lo:
Loopback address: 127.0.0.1
```

## 3.2 Experimental Data Analysis

Note: In the following figures and tables, **data size refers to the matrix dimension n**.


### 3.2.1 Serial Method

#### Program Execution Time (ns)

| Matrix Size | 60       | 600     | 3000      | 6000      | 9000       |
| ----------- | -------- | ------- | --------- | --------- | ---------- |
| Time (ns)   | 16151189 | 7624626 | 149265289 | 756004190 | 1895517873 |

Table 1. Execution time of the serial method.


### 3.2.2 Block Chessboard Partition Method

#### (1) Program Execution Time (ns)

| Threads | 60       | 600      | 3000      | 6000       | 9000       |
| ------- | -------- | -------- | --------- | ---------- | ---------- |
| 1       | 11747360 | 25538206 | 359440326 | 1879810810 | 4652625560 |
| 4       | 11349439 | 18354892 | 198716878 | 743394804  | 1874439954 |
| 9       | 11737585 | 18627643 | 128213882 | 494356908  | 1066216506 |
| 16      | 12314558 | 18797636 | 115602016 | 392793655  | 897725820  |
| 25      | 13107538 | 13164043 | 102099657 | 365435333  | 811337165  |

Table 2. Execution time for the block chessboard partition method.

#### (2) Speedup and Efficiency

As shown in **Figure 3**, the distribution of speedup and efficiency for the block chessboard partition method.

<img width="1246" height="545" alt="image" src="https://github.com/user-attachments/assets/2e38e081-a8dd-446c-b719-38066d7f1886" />

Figure 3. Experimental data of the block chessboard partition method.



### 3.2.3 Rectangular Partition Method

#### (1) Program Execution Time (ns)

| Threads | 60       | 600      | 3000      | 6000      | 9000       |
| ------- | -------- | -------- | --------- | --------- | ---------- |
| 1       | 15875320 | 21618127 | 320571422 | 920583248 | 2464202165 |
| 4       | 12141704 | 19590854 | 257511615 | 718843221 | 1515985727 |
| 9       | 12228727 | 19429445 | 177459268 | 476291656 | 1100012779 |
| 16      | 12734651 | 19659519 | 148418645 | 384720087 | 864486932  |
| 25      | 12012098 | 19509315 | 102656602 | 362816333 | 867285480  |

Table 3. Execution time for the rectangular partition method.

#### (2) Speedup and Efficiency

As shown in **Figure 4**, the speedup and efficiency distribution for the rectangular partition method.

<img width="1296" height="542" alt="image" src="https://github.com/user-attachments/assets/ee60fbf6-4739-4a35-a08c-f0f9a1390cda" />

Figure 4. Experimental data for the rectangular partition method.



## 3.2.4 Data Analysis

1. Experimental results show that as **thread count and data size increase**, program execution time generally decreases, while speedup increases, consistent with **Gustafson’s Law**.

2. The overall execution time in this experiment is somewhat slow, possibly due to:

   * Freeing matrix memory (`free`) requiring additional **O(n²)** time.
   * Matrix initialization requiring **malloc allocation followed by sequential assignment**.
   * Frequent function calls such as `trans()`, `init()`, and `Swp_itm()`.

3. The pure serial implementation (without pthread) sometimes runs faster than the **t = 1 thread configuration**, because the latter still performs redundant thread operations such as **create and join**.

4. In the block chessboard method, when **n = 60**, execution time with **16 or 25 threads** becomes slower than with **1 thread**, because thread creation and synchronization overhead offsets the benefits of parallelization.

5. In the rectangular partition method, exact partitioning is difficult because the division formula involves **square roots**, so perfect workload balance cannot always be achieved.

6. When the matrix size is large, parallel methods outperform serial methods. However, for small matrices, the advantages of parallelism diminish.

7. According to **Amdahl’s Law**, speedup cannot increase indefinitely as thread count grows. In this experiment, speedup growth becomes limited once thread count exceeds **16**.

8. As thread count increases, **algorithm efficiency E decreases from 100%**, because workload is divided among multiple threads.



# IV. Experimental Summary

## 4.1 Problems Encountered and Solutions

1. Execution time was initially measured using `qstat -f`, which reports the **total job runtime** rather than precise program runtime. More accurate results were obtained by examining compiled `.o` files.

2. Parallel execution initially appeared slower than serial execution because the **PBS script allocated too few processors (ppn)**. Setting `ppn = 8` solved the problem.

3. After compilation, using the `cat` command sometimes produced **No such file** errors when reading output files, possibly due to unstable network or environment conditions. Opening files with `vim` resolved the issue.

4. These experiments demonstrate that improving program performance is complex and involves many practical considerations beyond simply adding threads. This complexity is both a challenge and an attraction of **parallel computing**.



## 4.2 Understanding of pthread, OpenMP, and MPI Parallel Programming

### 4.2.1 pthread and OpenMP Programming Models

The **pthread model** creates a set of threads at program startup and distributes tasks among them. This method requires extensive thread management code and does not automatically scale with processor availability.

**OpenMP** is a parallel programming model for **shared-memory and distributed shared-memory multiprocessor systems**. It allows parallelization by adding directives to loops without specifying thread counts explicitly.

Advantages of OpenMP:

* Easier to use
* Automatically adapts to processor availability

Disadvantages:

* Less flexible
* Debugging is more difficult

Compilation differences:

* **OpenMP** requires compiler directives such as `#pragma`.
* **pthread** is a library where thread creation and management are explicitly implemented by the programmer.



### 4.2.2 MPI Programming Model

Unlike OpenMP and pthread, which rely on **shared memory**, **MPI (Message Passing Interface)** is designed for **distributed memory systems**.

Data sharing is achieved through **message passing**, since processes may run on different machines.

MPI programs require software such as **MPICH** to coordinate communication between processes.

Advantages:

* Works regardless of whether hardware memory is shared
* Suitable for large-scale problems

Disadvantages:

* Algorithm modifications may be required when errors occur
* Network communication may affect performance



# V. Course Summary

## 5.1 Positive Aspects of the Course

1. The instructor allocated significant **hands-on practice time**, allowing students to experience various parallel algorithms directly.

2. Lecture videos uploaded to the **Zhihuishu platform** help reinforce difficult concepts.

3. Teachers and teaching assistants actively answered questions during experiments, greatly improving learning efficiency.



## 5.2 Suggestions for Improvement

1. Online theoretical lectures could include **live programming demonstrations**, allowing students to see real implementations of parallel algorithms.

2. Uploaded lecture videos could be **edited into shorter segments**, making them easier to review and reducing fatigue from watching long recordings.

---

# Appendix: Analysis of Experiments and Course Concepts

| No. | Experiment Content                  | Theoretical Knowledge                                | Analysis                                                                                  |
| --- | ----------------------------------- | ---------------------------------------------------- | ----------------------------------------------------------------------------------------- |
| 1   | Analyze speedup and efficiency      | Performance analysis                                 | Speedup, efficiency, and scalability can evaluate parallel program performance            |
| 2   | Implement programs using pthread    | Thread concept and pthread programming               | Threads are the basic unit of program scheduling and enable concurrent execution          |
| 3   | Remote cluster computing            | Parallel hardware environment and cluster technology | Clusters combine multiple high-performance workstations under a unified interface         |
| 4   | Running programs using PBS scripts  | Parallel job management and PBS                      | PBS provides flexible batch processing and job scheduling                                 |
| 5   | Limited speedup in parallel methods | Speedup laws                                         | When task size remains constant, increasing concurrency may only slightly improve speedup |

