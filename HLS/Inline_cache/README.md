# **_Inline cache implementation in HLS_** #


This study aims to show the performance of a multi-port cache design based on a proven cache design on FPGA rather than a cache design. Therefore, the cache design proposed by Ma et al. is used in this study. In the cache design they designed, the data is transferred from the global DRAM to the local on-chip SRAM buffers, and the cache parameters can be reconfigurable with minimal changes in the source code they have written. The port numbers will be a parameter in multi-ported cache design, and it will be scalable up to 32. In this project, our main objective is to develop a cache design that supports many ports to get more acceleration in the hardware by keeping execution stages all busy. Accelerators often require application-specific access and temporal and spatial-locality. General flow chart of the system given below and you can implement the IP block using given source code. For further information please follow [Github](https://github.com/HLSpolito/SDAccel_kernel_cache)



![Flow Chart of Proposed System](/HLS/Images/flowchart_inline.png?raw=true "Flow Chart of Proposed System")
