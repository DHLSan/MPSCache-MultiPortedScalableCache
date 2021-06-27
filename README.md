# MPSCache Multi-Ported Scalable Cache #

Multi-Ported Scalable Cache Design 

Team Number: xohw21-248

Eskisehir Technical University

## **_Participants_** ##
- Fatma ÖZÜDOĞRU
  - Contact: ozudogrufatma26@gmail.com
- Seray ÖZCAN
  - Contact: serayyozcann@gmail.com
## **_Supervisor_** ##
 - Assist. Prof. Dr. İsmail San
    - Contact: isan@eskisehir.edu.tr
## **_Platform and Tools_** ##
- Zedboard ZYNQ-7000
- Xilinx Vivado High-Level Synthesis 2017.4
- Xilinx Vivado Design Suit 2017.4
- Xilinx Software Development Kit 2017.4 
## **_Project Description_** ##
While approaching the end of Moore's Law regarding physical limits in silicon technology, parallel multi-core hardware system design and parallel-programming environments are concentrated solutions to get more performance. Today, varying computing platforms solve problems of different high-computational tasks with varying architectures of memory. In the end, heterogeneous computing platforms are presenting a better performance for application-specific solutions since they have customized logic units and the transactions between compute and memory units. Computing and communication specialization for a specific task can achieve high performance or high energy efficiency in computing with computing and communication specialization in a particular study. Reconfigurable computing platforms allow us to tailor our design to perform better in terms of performance and area. In this project, our main objective is to develop a cache design that supports many ports to get more acceleration in the hardware by keeping execution stages all busy. Even if we have accelerated computing with specialized accelerators, memory access, especially to off-chip memory access, is the bottleneck in many application fields, e.g., memory-intensive applications such as neural networks, matrix operations, and graph processing. Memory hierarchy with different cache levels is the solution to the memory access problem, and it will reduce the overall memory access latency of the application. Since reconfigurable platforms perform better performance, in this project, we propose high-level multi-ported cache designs and introduce a novel and efficient approach to improve the memory access in FPGA. Various methods have been used in multi-port memory studies to improve memory access. In this study, BRAM-based multi-port cache design is proposed based on the XOR method to increase read ports and write ports. So far we have observed 8x acceleration in the inline cache design. In addition, we achieved approximately 30 times acceleration in the multi-port memory design we designed.
## **_Report_** ##
Project phases and more detail about project is shared in this repository as MPSCache_report_xohw21-248.pdf
## **_Video Link_** ##
https://youtu.be/ASoMwttuucU

## **_Inline Cache source code link we used throughout the project_** ## 

- Inline cache proposed by Ma et al, and you can find at 
- [Github]()

## **_The resource used for the multi-ported memory designed in the project_** ##
- [Github](https://github.com/tomverbeure/multi_port_mem/tree/e9d456f019913c94d2aa2839e199fed50840d09b)

## **_Step-by-step Building and Testing Instructions_** ##

--Running the multiport memory  design
Creating IP with HLS:
  1. Open Vivado HLS 2017.4
  2. Create New Project
  3. Name of the top function should be "multiport" for seaching neural network IP.
  4. Click C synthesis.
  5. When synthesis is finished, click Export RTL.
Creating SoC Design with All Components:
  1. Open Vivado 2017.4
  2. Create project
  3. Choose ZedBoard Development Kit as destination board
  4. Create block design
  5. Add ip --> zynq processing system
  6. Add ip --> Axi_timer
  7. Add source --> Axi_smartconnect and Axi_interconnect
  8. Click Window --> Add ip catalog --> click right click on list --> select your the path of the multiport IP you exported --> click OK
  9. click right click on block design -->  ip catalog --> select ip files which you created with HLS
  10. Make neccessary connections as seen in VIVADO/Multi_port_memory/design1.pdf, make sure the connections are exactly the same with design1.pdf
  11. Click Generate Bitstream
  12. Right click on design file and click Create HDL Wrapper.
  13. Right click on design and click Generate Output Products.
  14. Click on Generate Bitstream.

When generate bitstream finished, export hardware (include bitstream should be marked) and launch SDK.

  1.Create new Hello World application project in SDK.
  2. Replace the contents of the Hello world project with SDK/Multi_port_memory/main.c
  3. Ready to run.
  

