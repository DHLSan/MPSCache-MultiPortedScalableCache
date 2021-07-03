# **_Multi-ported scalable memory design and implementation in HLS_** #

![Flow Chart of Proposed System](/HLS/Images/Scheme.png?raw=true "Flow Chart of Proposed System")
## **_Running the multiport memory design Creating IP with HLS:_** ##

The following detailed steps are required to build and test our project with HLS. 

  1. Open Vivado HLS 2017.4
  2. Create New Project
  3. Name of the top function should be "multiport" for xor-based multi-port memory.
  4. Click C synthesis.
  5. When synthesis is finished, click Export RTL.



 ## **_Theoretical Background of Design_** ##
 
 
The XOR non-table-based write scheme design which works by XORing the components of a specific location to a particular bank of memory with the old contents of the same position from all other banks each time a read command is issued. The working principle is based on computing the XOR of values for that position across all banks that return the most recently written value. The design should have enough internal read ports to allow for writing. This necessitates a BRAM column one width less than the number of write ports. In addition, for each external read port, a BRAM column is required. In the design, each write port has its own bank of two BRAMs, and writes for each are copied to both BRAMs corresponding locations in all of the BRAMs in a bank always have the same value.Similar to the LVT approach, the XOR approach internally uses banking and replication.

 Total BRAM number can be calculated with 

  **_nr_block_rams = nr_write_ports * ((nr_write_ports-1) + nr_read_ports)_**
  
  

![Figure 1](/HLS/Images/XOR.png?raw=true "Figure 1")



To implement the XOR-based design, a scalable customized multi-ported RAM IP block is created on the HLS platform. This IP block is a design that allows two reads and two writes in itself and has a scalable feature with an array size that can change according to usage. The main reason why the sorting algorithm is preferred for testing is the need for 2R2W following the design and its widespread use in programming systems. For this reason, the Selection Sort algorithm, which was decided to be used as a benchmark because it contains two reads and two writes, was also placed in this IP block. Selection sort pseudocode is available below.  As seen in Figure in the pseudocode, the selection sort algorithm, which sorts from smallest to largest, makes two readings for X[ IndexOfMin ] and X[ i ] values in steps 1.3 and 1.4. Addition this, there are two write operations in X [ IndexOfMin ] and X[ i ] in steps 1.4 and 1.5.



![Figure 2](/HLS/Images/SelectionSort.png?raw=true "Figure 2")




