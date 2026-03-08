# Experiment 4: Implementation of Hadoop/Spark Algorithms

## I. Experimental Content

### 1.1 Experiment Topic

This experiment implements **WordCount**, which is one of the simplest distributed application examples.
Its main function is to **count the total number of occurrences of each word in the input directory**.

For example, if a text file contains the following content:

Hello world

The output result should be:

Hello 1
world 1

WordCount can be implemented in several ways. In this experiment, the requirement is to implement the **WordCount program using Hadoop or Spark**.

---

### 1.2 Experimental Environment Configuration

In this experiment, the Hadoop environment is configured on an **Ubuntu 20.04 virtual machine**.
The configuration steps for each node are described as follows.

---

### 1.2.1 SSH Password-Free Authentication Configuration

Figure 1-1 shows the command summary for configuring **OpenSSH**.

Figure 1-1 Configuration of SSH password-free login

This configuration process consists of four steps:

1. Install OpenSSH
2. Start the SSH service
3. Generate a key pair using `ssh-keygen`
4. Import the key into `authorized_keys` to enable password-free login

---

### 1.2.2 Java Environment Configuration

(1) First, download the **JDK** from the official website and extract it.
Then modify the `.bashrc` file and add the following configuration at the end of the file.

Figure 1-2

(2) Add Java environment variable configurations in the **Profile file**, including settings for variables such as:

JAVA_HOME
JAVA_BIN

as shown in Figure 1-3.

Figure 1-3 Java environment variable configuration

(3) Open the **environment file** and add the directory paths of JDK and related libraries.

Figure 1-4 Configuration of related directory paths

(4) After each modification, the command

source + file path

must be executed to make the configuration effective.

After completing these steps, the **Java environment configuration is finished**.

---

### 1.2.3 Hadoop Environment Configuration

The Hadoop version used in this experiment is **Hadoop 2.7.4**.

After downloading from the Hadoop official website, the installation is performed using the command:

sudo tar zxvf hadoop-2.7.4.tar.gz

Then Hadoop is installed into the specified directory.

The configuration steps are as follows.

#### (1) Modify the `.bashrc` file

Add Hadoop-related environment variables as shown in Figure 1-6.

Figure 1-6 Hadoop variable configuration

#### (2) Configure Hadoop environment files

Add the content shown in Figure 1-7 to:

hadoop-env.sh
yarn-env.sh

(`yarn-env.sh` only needs to define `JAVA_HOME`).

Figure 1-7 Filling `.sh` configuration files

#### (3) Modify Hadoop configuration files

Edit the following files:

core-site.xml
hdfs-site.xml
yarn-site.xml

Replace the code between `<configuration>` and `</configuration>`.

Figure 1-8 shows the terminal interface verifying that Hadoop has been successfully configured.

Figure 1-8 Hadoop configuration completion interface

---

### 1.2.4 Network and Node Configuration

#### (1) Network Configuration

Change the network mode of the Ubuntu virtual machine to **Bridge Mode**.

Modify the IP mapping of all nodes using the command:

sudo vim /etc/hosts

---

#### (2) Node Configuration

First modify the following files:

slaves
core-site.xml
hdfs-site.xml
mapred-site.xml
yarn-site.xml

Then delete temporary files and copy the Hadoop directory under the **Master node's local directory** to each slave node.

---

# II. Experimental Principle and Design Implementation

## 2.1 Experimental Principle

**MapReduce** is a parallel computing programming model that can be applied to **large-scale data processing**.

It mainly requires implementing two functions:

**map function**

Processes the input `(in_key, in_value)` pair and generates intermediate results.

**reduce function**

Aggregates all intermediate results with the same key and produces the final result.

The overall framework is shown in Figure 2-1.

Figure 2-1 MapReduce model framework

In this experiment, the **WordCount program is implemented in the Hadoop environment using the MapReduce model**.

---

# 2.2 Parallel Algorithm Design

Based on the principle described above, the map and reduce interfaces are designed as follows.

---

## 2.2.1 Map Function

The word information in the source file is used as the `(key, value)` pair in the map function.

Specifically:

input = (filename, file content)

The map function splits the words in the file content and generates intermediate results:

<word, 1>

### Pseudocode

Function Map(input_key, input_value) : BEGIN

input_key = file name
input_value = file content

for each word w in input_value do

EmitIntermediate(w, "1")

endfor

END

---

## 2.2.2 Reduce Function

After the map operation completes, the reduce function performs **result aggregation**.

All intermediate results corresponding to the same `output_key` are grouped together to generate the final output.

### Pseudocode

Function Reduce(output_key, mid_values) : BEGIN

output_key = word w
mid_values = intermediate result list

res = 0

for each value v in mid_values do

res += parseInt(v)

endfor

Emit(Turn_to_Str(res))

END

---

# 2.3 Implementation

Based on the pseudocode in section 2.2, the program is implemented in **Java**.

### (1) Create a Mapper class

Implement the **map function**.

Split words using spaces as delimiters and generate `<word, 1>` tuples for each word.

---

### (2) Create a Reducer class

Implement the **reduce function**.

A list structure is used to store intermediate results.
All `"1"` values in the list are summed to obtain the final word frequency.

---

### (3) Configure the main function

In the `main()` function:

Create a **Job object** to store parameters such as:

* jar package path
* input and output file paths
* mapper class
* reducer class

---

# III. Experimental Results

## 3.1 Execution Process

The following screenshots show several key steps in the experimental process.

### (1) Upload files to HDFS

Create directories in **HDFS** and upload files.

Use the command:

hadoop fs -ls

to verify that the files have been successfully uploaded.

Figure 3-1 Execution screenshot

---

### (2) Run the MapReduce job

Execute the command:

hadoop jar

The execution results include:

* INFO logs
* File System Counters
* Job Counters

Figure 3-2 Execution process screenshots

---

## 3.2 Result Output

After executing the Hadoop program, the output result is shown in Figure 3-3.

Figure 3-3 Output result screenshot

The results confirm that the program correctly performs **word frequency counting**.

---

# IV. Experimental Summary

## 4.1 Problems Encountered and Solutions

1. During environment configuration, problems occurred such as **failure of network address mapping**.
   After changing the virtual machine network mode, the issue was resolved.

2. This experiment was the first attempt to implement a relatively complex parallel computing program.
   By following the experimental manual and consulting online resources, the experiment was successfully completed.

Through this process, our understanding of **Hadoop and distributed computing** was significantly improved, and we realized the diversity and flexibility of parallel algorithms.

---

# 4.2 Understanding of Different Parallel Computing Models

## 4.2.1 MPI Programming Model

MPI is a programming model based on **distributed memory systems**.

Data sharing must be implemented through **message passing**, because MPI processes may run on different machines.

Communication between processes on different hosts requires installing MPI software such as **MPICH**.

Advantages:

* Works regardless of shared memory architecture
* Suitable for large-scale problems

Disadvantages:

* Algorithms often require major modification when errors occur
* Performance can be affected by network communication overhead

---

## 4.2.2 MapReduce Programming Model

MapReduce is a programming model designed for **large-scale datasets (greater than 1TB)**.

It can achieve good performance on:

* multi-core processors
* multiprocessor systems
* heterogeneous clusters

### Advantages over MPI

1. MapReduce jobs can launch a large number of instances simultaneously. If cluster resources are insufficient, only part of them will run first.

2. MapReduce instances do not require communication between each other.

3. If an instance fails, the scheduling system automatically retries the task without affecting other instances.

### Disadvantages

Intermediate results are stored in **files**, which may reduce efficiency.

---

# V. Course Summary

## 5.1 Positive Aspects of the Course

1. The instructor introduced parallel computing concepts based on their **research experience**, which increased student engagement.

2. Lecture videos uploaded to the **Zhihuishu platform** help students review difficult topics.

3. Teaching assistants actively helped solve problems during experiments, greatly improving learning efficiency.

---

## 5.2 Suggestions for Improvement

1. Uploaded videos could be **edited into shorter segments** based on topics, making review easier and reducing fatigue.

2. Online theoretical lectures could include **live coding demonstrations**, since parallel computing is highly practical.

Through online platforms such as **Tencent Meeting**, the demonstration interface can be shown more clearly to students.

---

# Appendix: Analysis of Experiments and Course Concepts

| No. | Experiment Content              | Theoretical Knowledge         | Analysis                                                                          |
| --- | ------------------------------- | ----------------------------- | --------------------------------------------------------------------------------- |
| 1   | Implement programs using Hadoop | MapReduce programming model   | Hadoop enables efficient parallel processing of large data                        |
| 2   | Implement MapReduce interfaces  | map function, reduce function | Map processes input and generates intermediate results; Reduce aggregates results |
| 3   | Use MapReduce nodes             | Storage management            | MapReduce uses multiple nodes where each node has its own storage                 |
| 4   | Distributed computing execution | Cluster architecture          | Cluster computing integrates multiple nodes for large-scale computation           |
