# Experiment 1: Multithreaded Computation of PI

## I. Overview of the Experiment

This experiment aims to compute the value of **PI** using **multithreaded parallel programming**. By applying **problem decomposition, domain decomposition, and loop summation algorithms**, PI is calculated using three different methods.  

The implementation is completed in **C++ with multithreading** on a remote virtual machine connected via **Xshell (CentOS 7.6, icc/icpc)**.

For each method, experiments are conducted with five data scales:

1e4, 1e5, 1e6, 1e7, 1e8  

and five thread configurations:

1, 2, 4, 6, 8 threads (1 thread corresponds to serial execution).

For each configuration, the **execution time** and **computed PI value** are recorded. Data analysis and performance analysis are then performed to compare the performance of the different algorithms.

### (1) Integral Method

Formula for calculation:

<img width="527" height="115" alt="image" src="https://github.com/user-attachments/assets/47dc2970-bf67-4bc8-9a93-4bd7626355c3" />

### (2) Probability Method

Randomly throw **n points** into a square.  
If **m points** fall inside the quarter circle, then:

<img width="179" height="78" alt="image" src="https://github.com/user-attachments/assets/e05ba154-c242-40f5-914d-3a20e17d00b4" />


### (3) Power Series Method

Formula for calculation:

<img width="736" height="87" alt="image" src="https://github.com/user-attachments/assets/24124ef4-62cb-43ba-8e5c-0537d2061b2b" />


# II. Parallel Algorithm Design and Analysis

Based on the three algorithms above, the pseudocode for each program is designed as follows.



## 2.1 Integral Method Pseudocode

```

Start

thread_function:
piece = n / t
start = thread_id * piece
end = start + piece


for i from start to end - 1
    compute rectangle integration sum in this domain

    pthread_mutex_lock(mutex)
    add local integration result to global pi
    pthread_mutex_unlock(mutex)


main:
Get n and t   // n = task size, t = number of threads


for i from 0 to t-1
    create thread i to execute thread_function

for i from 0 to t-1
    wait for threads to finish

output pi


End

```

 

## 2.2 Probability Method Pseudocode

```

Start

thread_sum:
length = n / t
start = thread_id * length
end = start + length


for i from 0 to length - 1
    generate random points using rand_r
    if point satisfies 0 <= x <= 1 and 0 <= y <= 1
        local_m++

pthread_mutex_lock(mutex)
add local_m to global m
pthread_mutex_unlock(mutex)


main:
Get n and t   // n = number of point pairs, t = number of threads


for i from 0 to t-1
    create thread i to execute thread_sum

for i from 0 to t-1
    wait for threads to finish

pi = 4 * m / n

output pi


End

```

 

## 2.3 Power Series Method Pseudocode

```

Start

thread_sum:
length = n / t
start = thread_id * length
end = start + length


factor = 1 if start % 2 == 0 else -1

for i from 0 to length - 1
    factor alternates sign each step
    local_sum += factor * (2*i + 1)

pthread_mutex_lock(mutex)
add local_sum to global sum
pthread_mutex_unlock(mutex)


main:
Get n and t


for i from 0 to t-1
    create thread i to execute thread_sum

for i from 0 to t-1
    wait for threads to finish

pi = 4.0 * sum

output pi


End

```

In general, the three algorithms divide the problem into **n/t sub-tasks**, and assign each sub-task to a thread using **pthread_create**.  

Each thread completes its computation and contributes its result to the final sum.

Since global variables such as **sum** and **pi** cannot be accessed simultaneously by multiple threads safely, a **mutex lock** is used to ensure mutual exclusion during updates.

 

# III. Experimental Data Analysis

## 3.1 Experimental Environment (Cluster)

### (1) CPU Specifications

| Cores | Model | Clock Frequency | Cache |
| --- | --- | --- | --- |
| 4 | Six-Core AMD Opteron(tm) Processor 2431 | 2400.085 MHz | 512 KB |

### (2) Memory Information

| Memory Capacity | Memory Bandwidth |
| --- | --- |
| 8174052 kB | 1000 Mb/s |

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

### 3.2.1 Integral Method

#### (1) Program Execution Time (ns)

| Threads | 1e4 | 1e5 | 1e6 | 1e7 | 1e8 |
|  |  |  |  |  |  |
| 1 | 23754120 | 14789343 | 45501470 | 221557140 | 1235737800 |
| 2 | 11588335 | 14107704 | 28786897 | 154502391 | 657028675 |
| 4 | 11669397 | 12889623 | 20100593 | 95278739 | 424170255 |
| 6 | 11631998 | 12343406 | 19075155 | 63381433 | 317643880 |
| 8 | 11948824 | 12694358 | 16345977 | 49281835 | 202944278 |

PI values:

| Data Size | PI |
|  |  |
| 1e4 | 3.141592741 |
| 1e5 | 3.141592741 |
| 1e6 | 3.141592741 |
| 1e7 | 3.141592502 |
| 1e8 | 3.141592629 |

#### (2) Speedup and Efficiency

<img width="1001" height="348" alt="image" src="https://github.com/user-attachments/assets/a114f218-6a71-4eaa-bb32-2386dfb932de" />


### 3.2.2 Probability Method

#### (1) Program Execution Time (ns)

| Threads | 1e4 | 1e5 | 1e6 | 1e7 | 1e8 |
|  |  |  |  |  |  |
| 1 | 19047021 | 20251274 | 72647571 | 397257804 | 3236879348 |
| 2 | 11669397 | 15562534 | 55416345 | 275893449 | 1635401725 |
| 4 | 11516332 | 13953447 | 33194065 | 156430006 | 874607324 |
| 6 | 11614799 | 12948036 | 25901794 | 128123283 | 652010917 |
| 8 | 12431859 | 12746095 | 21744489 | 103446006 | 437454223 |

PI values:

| Data Size | PI |
|  |  |
| 1e4 | 3.150400000 |
| 1e5 | 3.143640000 |
| 1e6 | 3.141600000 |
| 1e7 | 3.14178080 |
| 1e8 | 3.14157136 |

#### (2) Speedup and Efficiency

<img width="1023" height="378" alt="image" src="https://github.com/user-attachments/assets/ec7e4efc-8d20-46ce-961a-a1f8a0d699bf" />


### 3.2.3 Power Series Method

#### (1) Program Execution Time (ns)

| Threads | 1e4 | 1e5 | 1e6 | 1e7 | 1e8 |
|  |  |  |  |  |  |
| 1 | 12830734 | 10909318 | 32841920 | 138804674 | 904424428 |
| 2 | 10397672 | 9259700 | 22530317 | 81527354 | 464498519 |
| 4 | 11076211 | 10437965 | 14690160 | 55942773 | 455699682 |
| 6 | 8411407 | 8679628 | 14041423 | 50032138 | 455316066 |
| 8 | 10582923 | 8553504 | 14106988 | 49819707 | 455834150 |

PI values:

| Data Size | PI |
|  |  |
| 1e4 | 3.141492653 |
| 1e5 | 3.141582653 |
| 1e6 | 3.141591653 |
| 1e7 | 3.141592553 |
| 1e8 | 3.141592643 |

#### (2) Speedup and Efficiency

-<img width="1084" height="383" alt="image" src="https://github.com/user-attachments/assets/1a97db5b-dd6d-484c-ab46-474076ddcfc6" />


## 3.2.4 Data Analysis

1. The results show that as the **data size increases**, the computed PI value becomes more accurate (smaller error), while execution time generally increases according to **Gustafson’s Law**. Increasing the number of threads generally reduces execution time and increases speedup.

2. The **probability method** introduces randomness, resulting in probabilistic errors and lower accuracy.

3. Among the three methods, the **power series method** provides the most accurate result with relatively short computation time.

4. When the data size is small, speedup improvement is not obvious. As the task size increases, speedup may even decrease because the overhead of **thread creation, synchronization, and communication** becomes significant.

5. According to **Amdahl’s Law**, increasing the number of threads cannot infinitely increase speedup. Therefore, speedup eventually reaches a bottleneck.

6. In both the **integral method and power series method**, the computed PI values remain almost identical regardless of thread count, because parallelization only divides the problem into subdomains without changing the overall computation.

7. As the number of threads increases, the **efficiency E** of the algorithm decreases from 100% (serial execution), because the workload is distributed among multiple threads.

 

# IV. Experimental Summary

## 4.1 Problems Encountered and Solutions

1. The thread configuration was not entirely reasonable. Using **t = 6** threads sometimes caused uneven workload distribution.

2. Initially, the execution time was measured incorrectly using `qstat -f`. Later it was discovered that more accurate execution times could be obtained from the compiled `.o` file information.

3. At first, mutex locks were forgotten, causing incorrect results. This highlighted the importance of **synchronizing access to global variables**.

4. Sometimes parallel execution was slower than serial execution because the **PBS configuration allocated too few processors (ppn)**. After setting `ppn = 8`, the problem was resolved.

 

## 4.2 Understanding of pthread, OpenMP, and MPI Parallel Programming

### 4.2.1 pthread and OpenMP Programming Models

The **pthread model** creates multiple threads at program startup and distributes tasks among them. This approach requires extensive thread management code and does not easily scale with processor count.

**OpenMP** is a parallel programming model designed for **shared-memory and distributed shared-memory systems**. It allows parallelization by simply adding directives (e.g., `#pragma`) to loops. OpenMP is easier to use but less flexible, and debugging can be more difficult.

Another difference lies in compilation:

- OpenMP uses compiler directives (`#pragma`) and the compiler handles thread creation.
- pthread is a library where thread creation and management are controlled explicitly by the programmer.

 

### 4.2.2 MPI Programming Model

Unlike OpenMP and pthread, which rely on **shared memory**, **MPI (Message Passing Interface)** is designed for **distributed memory systems**.

Data sharing in MPI occurs through **message passing**, since processes may run on different machines. MPI programs require MPI software (e.g., **MPICH**) to coordinate communication between processes.

Advantages:

- Works on both shared and distributed memory systems
- Suitable for very large-scale problems

Disadvantages:

- Algorithms often require significant modification if errors occur
- Performance can be affected by network communication overhead

 

# V. Course Summary

## 5.1 Positive Aspects of the Course

1. The instructor combined personal research experience with course material, increasing student interest.

2. Lecture videos uploaded to the **Zhihuishu platform** help students review difficult concepts.

3. Teachers and teaching assistants actively answered questions during experiments, improving learning efficiency.

 

## 5.2 Suggestions for Improvement

1. Recorded lecture videos could be **edited into shorter segments** corresponding to specific topics to improve review efficiency.

2. Online lectures could include **live programming demonstrations**. Since parallel computing is highly practical, showing real code examples would improve understanding.

 

# Appendix: Analysis of Experiments and Course Concepts

| No. | Experiment Content | Theoretical Knowledge | Analysis |
| -| -| -| -|
| 1 | Analyze speedup and efficiency | Performance analysis | Speedup, efficiency, and scalability can evaluate parallel program performance |
| 2 | Implement programs using pthread | Thread concept and pthread programming | Threads enable concurrent task execution and improve performance |
| 3 | Protect mutex variables such as sum and pi | Relationship between processes and threads | Proper synchronization between variables is essential in parallel programs |
| 4 | Limited speedup in power series method | Speedup laws | When task size is fixed, increasing concurrency may not significantly increase speedup |
