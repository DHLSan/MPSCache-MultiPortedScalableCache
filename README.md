# MPSCache Multi-Ported Scalable Cache #

Multi-Ported Scalable Cache Design 

Team Number: xohw21-248

Eskisehir Technical University

## **_Participants_** ##
- Fatma ÖZÜDOĞRU
  - Contact: ozudogrufatma26@gmail.com
- Seray ÖZCAN
  - Contact: serayyozcann@gmail.com
## **_Supervisor:_** ##
 - Assist. Prof. Dr. İsmail San
    - Contact: isan@eskisehir.edu.tr
## Platform and Tools ##
- Zedboard ZYNQ-7000
- Xilinx Vivado High-Level Synthesis 2017.4
- Xilinx Vivado Design Suit 2017.4
- Xilinx Software Development Kit 2017.4 
## Project Description ##
While approaching the end of Moore's Law regarding physical limits in silicon technology, parallel multi-core hardware system design and parallel-programming environments are concentrated solutions to get more performance. Today, varying computing platforms solve problems of different high-computational tasks with varying architectures of memory. In the end, heterogeneous computing platforms are presenting a better performance for application-specific solutions since they have customized logic units and the transactions between compute and memory units. Computing and communication specialization for a specific task can achieve high performance or high energy efficiency in computing with computing and communication specialization in a particular study. Reconfigurable computing platforms allow us to tailor our design to perform better in terms of performance and area. In this project, our main objective is to develop a cache design that supports many ports to get more acceleration in the hardware by keeping execution stages all busy. Even if we have accelerated computing with specialized accelerators, memory access, especially to off-chip memory access, is the bottleneck in many application fields, e.g., memory-intensive applications such as neural networks, matrix operations, and graph processing. Memory hierarchy with different cache levels is the solution to the memory access problem, and it will reduce the overall memory access latency of the application. Since reconfigurable platforms perform better performance, in this project, we propose high-level multi-ported cache designs and introduce a novel and efficient approach to improve the memory access in FPGA. Various methods have been used in multi-port memory studies to improve memory access. In this study, BRAM-based multi-port cache design is proposed based on the XOR method to increase read ports and write ports. So far we have observed 8x acceleration in the inline cache design. In addition, we achieved approximately 30 times acceleration in the multi-port memory design we designed.
## Report ##
Project phases and more detail about project is shared in this repository as MPSCache_report_xohw21-248.pdf
## Step-by-step Build and Test Instructions ##

