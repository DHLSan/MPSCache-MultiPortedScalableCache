# **_Multi-ported scalable memory design and implementation in VIVADO_** #

## **_Running the multiport memory SoC design with VIVADO:_** ##

The following detailed steps are required to build our project with VIVADO. 

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
  
![Figure 1](/VIVADO/Images/Multi-ported-SOC.png?raw=true "Title")

The SoC architecture set up for the experiment is shown in Figure 13. Since the Zynq-7000 architecture has an ARM processor and FPGA blocks, it is software and hardware programmable. Communication between these two structures is provided through the Advanced Extensible Interface (AXI). Therefore, All communications between the processor, memory and the reconfigurable custom IP containing the selection sort algorithm are made through AXI. Also, Since High Performance(HP)  ports are designed for full bandwidth access to off-chip memory, we used an HP port for accessing DDR.


One of the most important parts in this section is to determine the memory address of the custom IP created over hls.we have set the memory address to read data and the memory address to write data  for the m_axi_BUS_MEM and m_axi_BUS_MEM1 ports of multiport_0 custom IP. We also added a timer module to measure multiport memory performance.
