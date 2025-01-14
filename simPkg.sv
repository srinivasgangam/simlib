
`ifndef SIM_PKG_
`define SIM_PKG_

package simPkg;

  import "DPI-C" context function int    simRead_  (input string fileName,int seed);
  import "DPI-C" context function int    cDpi_getVal    (input string objName, input string propName, input int defVal);
  import "DPI-C" context function int    cDpi_getVal_   (input string objName, input string propName, input int idx, input int defVal);
  import "DPI-C" context function int    cDpi_getList   (input string objName, input string propName, output int list[256]);
  import "DPI-C" context function int    cDpi_getList_  (input string objName, input string propName, input int idx, output int list[256]);
  import "DPI-C" context function string cDpi_getString (input string objName, input string propName, input string defVal);
  import "DPI-C" context function string cDpi_getString_(input string objName, input string propName, input int idx, input string defVal);

  import "DPI-C" context function int    dpi_simRead_  (input string fileName,int seed);
  import "DPI-C" context function int    dpi_getVal    (input string objName, input string propName, input int defVal);
  import "DPI-C" context function int    dpi_getVal_   (input string objName, input string propName, input int idx, input int defVal);
  import "DPI-C" context function int    dpi_getList   (input string objName, input string propName, output int list[256]);
  import "DPI-C" context function int    dpi_getList_  (input string objName, input string propName, input int idx, output int list[256]);
  import "DPI-C" context function string dpi_getString (input string objName, input string propName, input string defVal);
  import "DPI-C" context function string dpi_getString_(input string objName, input string propName, input int idx, input string defVal);

endpackage

`endif

