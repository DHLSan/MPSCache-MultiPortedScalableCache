# **_Multi-ported scalable memory design and implementation in VIVADO_** #

## **_Running the multiport memory SoC designwith VIVADO:_** ##

The following detailed steps are required to build and test our project with VIVADO. 

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
  
![Figure 1](/VIVADO/Images/Multi-ported-SOC.png?raw=true "Title")
