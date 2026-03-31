# LAB 4: Implementation of Hadoop/Spark Algorithms

## I. Experimental Content

### 1.1 Experiment Topic

This experiment implements **WordCount**, which is one of the simplest distributed application examples. Its main function is to **count the total number of occurrences of each word in the input directory**.

For example, if a text file contains the following content:
```
Hikawa Sayo
```
The output result should be:
```
Hikawa 1
Sayo 1
```
WordCount can be implemented in several ways. In this experiment, the requirement is to implement the **WordCount program using Hadoop or Spark**.

### 1.2 Experimental Environment Configuration

In this experiment, the Hadoop environment is configured on an **Ubuntu 20.04 virtual machine**. The configuration steps for each node are described as follows.

### 1.2.1 SSH Password-Free Authentication Configuration

Figure 1-1 shows the command summary for configuring **OpenSSH**.

```
sudo apt-get install openssh-server
sudo /etc/init.d/ssh start
ssh-keygen -t rsa
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
```
_Figure 1-1 Configuration of SSH password-free login_

This configuration process consists of four steps:

1. Install OpenSSH
2. Start the SSH service
3. Generate a key pair using `ssh-keygen`
4. Import the key into `authorized_keys` to enable password-free login

### 1.2.2 Java Environment Configuration

(1) First, download the **JDK** from the official website and extract it.
Then modify the `.bashrc` file and add the following configuration at the end of the file.

```
export JAVA_HOME = /usr/java/jdk1.8.0_231
export CLASSPATH = .:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export PATH = $JAVA_HOME/bin:$PATH
```
_Figure 1-2_

(2) Add Java environment variable configurations in the **Profile file**, including settings for variables such as:

- JAVA_HOME
- JAVA_BIN

as shown in Figure 1-3.

```
export JAVA_HOME=/usr/java/jdk1.8.0_231
export JAVA_BIN=$JAVA_HOME/bin
export JAVA_LIB=$JAVA_HOME/lib
export CLASSPATH=.:$JAVA_LIB/tools.jar:$JAVA_LIB/dt.jar
export PATH=$JAVA_HOME/bin:$PATH
```
_Figure 1-3 Java environment variable configuration_

(3) Open the **environment file** and add the directory paths of JDK and related libraries.

```
PATH = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/java/jdk1.8.0_231/lib:/usr/java/jdk1.8.0_231"
```
_Figure 1-4 Configuration of related directory paths_

(4) After each modification, the command
```
source + file path
```
must be executed to make the configuration effective.

After completing these steps, the **Java environment configuration is finished**.

### 1.2.3 Hadoop Environment Configuration

The Hadoop version used in this experiment is **Hadoop 2.7.4**. After downloading from the Hadoop official website, the installation is performed using the command:
```
sudo tar zxvf hadoop-2.7.4.tar.gz
```
Then Hadoop is installed into the specified directory. The configuration steps are as follows.

#### (1) Modify the `.bashrc` file

Add Hadoop-related environment variables as shown in Figure 1-6.

```
export JAVA_HOME=/usr/java/jdk1.8.0_231
export HADOOP_INSTALL=/usr/local/hadoop
export PATH=$PATH:$HADOOP_INSTALL/bin
export PATH=$PATH:$HADOOP_INSTALL/sbin
export HADOOP_MAPRED_HOME=$HADOOP_INSTALL
export HADOOP_COMMON_HOME=$HADOOP_INSTALL
export HADOOP_HDFS_HOME=$HADOOP_INSTALL
export YARN_HOME=$HADOOP_INSTALL
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_INSTALL/lib/native
export HADOOP_OPTS="-Djava.library.path=$HADOOP_INSTALL/lib"
```
_Figure 1-6 Hadoop variable configuration_

#### (2) Configure Hadoop environment files

Add the content shown in Figure 1-7 to:

- hadoop-env.sh
- yarn-env.sh

(`yarn-env.sh` only needs to define `JAVA_HOME`).

```
export JAVA_HOME=/usr/java/jdk1.8.0_231
export HADOOP=/usr/local/hadoop
export PATH=$PATH:/usr/local/hadoop/bin
```
_Figure 1-7 Filling `.sh` configuration files_

#### (3) Modify Hadoop configuration files

Edit the following files:

- core-site.xml
- hdfs-site.xml
- yarn-site.xml

Replace the code between `<configuration>` and `</configuration>`.

Figure 1-8 shows the terminal interface verifying that Hadoop has been successfully configured.

```
Hadoop 2.7.4
Subversion https://svn@git-wip-us.apache.org/repos/asf/hadoop.git -r ...
Compiled by kshvachk on 2017-08-01T00:29Z
Compiled with protoc 2.5.0
From source with checksum 50b0468318b4ce9bd24dc467b7ce1148
This command was run using /usr/local/hadoop/share/hadoop/common/hadoop-common-2.7.4.jar
```
_Figure 1-8 Hadoop configuration completion interface_

### 1.2.4 Network and Node Configuration

#### (1) Network Configuration

Change the network mode of the Ubuntu virtual machine to **Bridge Mode**. Modify the IP mapping of all nodes using the command:
```
sudo vim /etc/hosts
```

#### (2) Node Configuration

First modify the following files:

- slaves
- core-site.xml
- hdfs-site.xml
- mapred-site.xml
- yarn-site.xml

Then delete temporary files and copy the Hadoop directory under the **Master node's local directory** to each slave node.

# II. Experimental Principle and Design Implementation

## 2.1 Experimental Principle

**MapReduce** is a parallel computing programming model that can be applied to **large-scale data processing**. It mainly requires implementing two functions:

- **map function**: Processes the input `(in_key, in_value)` pair and generates intermediate results.
- **reduce function**: Aggregates all intermediate results with the same key and produces the final result. 

The overall framework is shown in Figure 2-1.

<img width="516" height="300" alt="image" src="https://github.com/user-attachments/assets/6fccb7b1-f67f-4160-9b3f-fc0d768af71d" />

_Figure 2-1 MapReduce model framework_

In this experiment, the **WordCount program is implemented in the Hadoop environment using the MapReduce model**.

## 2.2 Parallel Algorithm Design

Based on the principle described above, the map and reduce interfaces are designed as follows.

### 2.2.1 Map Function

The word information in the source file is used as the `(key, value)` pair in the map function.

Specifically:
```
input = (filename, file content)
```
The map function splits the words in the file content and generates intermediate results:
```
<word, 1>
```
#### Pseudocode
```
Function Map(input_key, input_value) : BEGIN
   input_key = file name
   input_value = file content
   for each word w in input_value do
      EmitIntermediate(w, "1")
   endfor
END
```

### 2.2.2 Reduce Function

After the map operation completes, the reduce function performs **result aggregation**.

All intermediate results corresponding to the same `output_key` are grouped together to generate the final output.

#### Pseudocode
```
Function Reduce(output_key, mid_values) : BEGIN
   output_key = word w
   mid_values = intermediate result list
   res = 0
   for each value v in mid_values do
      res += parseInt(v)
   endfor
   Emit(Turn_to_Str(res))
END
```
## 2.3 Implementation

Based on the pseudocode in section 2.2, the program is implemented in **Java**.

### (1) Create a Mapper class

Implement the **map function**. Split words using spaces as delimiters and generate `<word, 1>` tuples for each word.

### (2) Create a Reducer class

Implement the **reduce function**. A list structure is used to store intermediate results. All `"1"` values in the list are summed to obtain the final word frequency.

### (3) Configure the main function

In the `main()` function: Create a **Job object** to store parameters such as:

* jar package path
* input and output file paths
* mapper class
* reducer class

# III. Experimental Results

## 3.1 Execution Process

The following screenshots show several key steps in the experimental process.

### (1) Upload files to HDFS

Create directories in **HDFS** and upload files.
Use the command:
```
hadoop fs -ls
```
to verify that the files have been successfully uploaded.

```
Found 2 items
-rw-r--r-- 1 hadoop supergroup 0 2022-05-09 13:54 /lab4/output/_SUCCESS
-rw-r--r-- 1 hadoop supergroup 32 2022-05-09 13:54 /lab4/output/part-r-00000
```
_Figure 3-1 Execution screenshot_

### (2) Run the MapReduce job

Execute the command:
```
hadoop jar
```
The execution results include:

* INFO logs
* File System Counters
* Job Counters

```
22/05/09 13:58:23 INFO mapreduce.Job: Job job_local524341653_0001 running ...
22/05/09 13:58:23 INFO mapreduce.Job: map 100% reduce 0%
22/05/09 13:58:53 INFO reduce.MergeManagerImpl: MergerManager: memoryLimit=...
22/05/09 13:58:53 INFO reduce.EventFetcher: attempt_local524341653_0001 ...
```
_(a)_

```
File System Counters
   FILE: Number of bytes read=51
   FILE: Number of bytes written=288301
   FILE: Number of read operations=0
   FILE: Number of large read operations=0
   FILE: Number of write operations=0
   HDFS: Number of bytes read=152
   HDFS: Number of bytes written=32
   HDFS: Number of read operations=9
   HDFS: Number of large read operations=0
   HDFS: Number of write operations=2

Job Counters
   Launched map tasks=2
   Launched reduce tasks=1
   Data-local map tasks=2
   Total time spent by all maps in occupied slots (ms)=4084
   Total time spent by all reduces in occupied slots (ms)=1832
   Total time spent by all map tasks (ms)=4084
   Total time spent by all reduce tasks (ms)=1832
   Total vcore-milliseconds taken by all map tasks=4084
   Total vcore-milliseconds taken by all reduce tasks=1832
   Total megabyte-milliseconds taken by all map tasks=3831728
   Total megabyte-milliseconds taken by all reduce tasks=2728643
```
_(b)_

```
Map-Reduce Framework
   Map input records=2
   Map output records=4
   Map output bytes=39
   Map output materialized bytes=62
   Input split bytes=204
   Combine input records=4
   Combine output records=4
   Reduce input groups=3
   Reduce shuffle bytes=62
   Reduce input records=4
   Reduce output records=3
   Spilled Records=8
   Shuffled Maps=2
   Failed Shuffles=0
   Merged Map outputs=2
   GC time elapsed (ms)=80
   CPU time spent (ms)=1990
   Physical memory (bytes) snapshot=729286434
   Virtual memory (bytes) snapshot=4288357953
   Total committed heap usage (bytes)=509213468
```
_(c)_

_Figure 3-2 Execution process screenshots_

## 3.2 Result Output

After executing the Hadoop program, the output result is shown in Figure 3-3.

```
WARNING: Use --illegal-access=warn to enable warnings of further illegal reflective access operations
WARNING: All illegal access operations will be denied in a future release
Hello  2
happy  1
world  1
```
_Figure 3-3 Output result screenshot_

The results confirm that the program correctly performs **word frequency counting**.

# IV. Experimental Summary

## 4.1 Problems Encountered and Solutions

1. During environment configuration, problems occurred such as **failure of network address mapping**.
   After changing the virtual machine network mode, the issue was resolved.

2. This experiment was the first attempt to implement a relatively complex parallel computing program.
   By following the experimental manual and consulting online resources, the experiment was successfully completed.

Through this process, our understanding of **Hadoop and distributed computing** was significantly improved, and we realized the diversity and flexibility of parallel algorithms.

# 4.2 Understanding of Different Parallel Computing Models

## 4.2.1 MPI Programming Model

MPI is a programming model based on **distributed memory systems**. Data sharing must be implemented through **message passing**, because MPI processes may run on different machines. Communication between processes on different hosts requires installing MPI software such as **MPICH**.

Advantages:

* Works regardless of shared memory architecture
* Suitable for large-scale problems

Disadvantages:

* Algorithms often require major modification when errors occur
* Performance can be affected by network communication overhead

## 4.2.2 MapReduce Programming Model

MapReduce is a programming model designed for **large-scale datasets (greater than 1TB)**. It can achieve good performance on:

* multi-core processors
* multiprocessor systems
* heterogeneous clusters

### Advantages over MPI

1. MapReduce jobs can launch a large number of instances simultaneously. If cluster resources are insufficient, only part of them will run first.

2. MapReduce instances do not require communication between each other.

3. If an instance fails, the scheduling system automatically retries the task without affecting other instances.

### Disadvantages

Intermediate results are stored in **files**, which may reduce efficiency.

# V. Course Summary

## 5.1 Positive Aspects of the Course

1. The instructor introduced parallel computing concepts based on their **research experience**, which increased student engagement.

2. Lecture videos uploaded to the **Zhihuishu platform** help students review difficult topics.

3. Teaching assistants actively helped solve problems during experiments, greatly improving learning efficiency.

## 5.2 Suggestions for Improvement

1. Uploaded videos could be **edited into shorter segments** based on topics, making review easier and reducing fatigue.

2. Online theoretical lectures could include **live coding demonstrations**, since parallel computing is highly practical.

Through online platforms such as **Tencent Meeting**, the demonstration interface can be shown more clearly to students.

# Appendix: Analysis of Experiments and Course Concepts

| No. | Experiment Content              | Theoretical Knowledge         | Analysis                                                                          |
| --- | ------------------------------- | ----------------------------- | --------------------------------------------------------------------------------- |
| 1   | Implement programs using Hadoop | MapReduce programming model   | Hadoop enables efficient parallel processing of large data                        |
| 2   | Implement MapReduce interfaces  | map function, reduce function | Map processes input and generates intermediate results; Reduce aggregates results |
| 3   | Use MapReduce nodes             | Storage management            | MapReduce uses multiple nodes where each node has its own storage                 |
| 4   | Distributed computing execution | Cluster architecture          | Cluster computing integrates multiple nodes for large-scale computation           |
