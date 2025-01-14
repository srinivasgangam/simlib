program main; 
import simPkg::*;

int array[256];
string str;
int size;

initial 
begin 
 $display("strted");
 if(simPkg::simRead_("test.sim",$urandom)==0)
 //if(simRead("test.sim")==0)
    $display("Error while reading sim file");

 //size= cDpi_getList("OBJ1","list",array);
 //for(int i=0;i<size;i++) $display("list = %0d",array[i]);
 $display("OBJ1-value10 = %0d",cDpi_getVal("OBJ1","value10",456));
 $display("OBJ2-value1  = %0d",cDpi_getVal("OBJ2","value1",4561));
 $display("OBJ3-value1  = %0d",cDpi_getVal("OBJ3","value1",4561));
 $display("OBJ1-value1  = %0d",cDpi_getVal("OBJ1","value1",111));

 
 repeat(10)
 cDpi_getVal("OBJ1","value4",111);

 $write("OBJ1-value5 = ");
 repeat(15)
 $write(" %0d ",cDpi_getVal("OBJ1","value5",111));
 $display("");

 $write("OBJ1-value7 = ");
 repeat(20)
 $write(" %0d ",cDpi_getVal("OBJ1","value7",112));
 $display("");

 $write("OBJ1-value8 = ");
 repeat(20)
 $write(" %0d ",cDpi_getVal("OBJ1","value8",113));
 $display("");

 $write("OBJ1-valueSeq = ");
 repeat(40)
 $write(" %0d ",cDpi_getVal("OBJ1","valueSeq",113));
 $display("");
 
 $write("OBJ1-valueSeqU = ");
 repeat(14)
 $write(" %0d ",cDpi_getVal("OBJ1","valueSeqU",113));
 $display("");

 $write("OBJ1-string1 = -%s-", cDpi_getString("OBJ1","string1","nothing"));
 $display("");
 $write("OBJ1-string2 = -%s-", cDpi_getString("OBJ1","string2","nothing"));
 $display("");
 $write("OBJ1-string2 = -%s-", cDpi_getString("OBJ1","string2","nothing"));
 $display("");
 $write("OBJ1-value8 = -%s-", cDpi_getString("OBJ1","value8","nothing"));
 $display("");

 cDpi_getList("OBJ1","list",array);

 $write("OBJ1-wtVal = ");
 repeat(30)
 $write(" %0d ",cDpi_getVal("OBJ1","wtVal",113));
 $display("");
 $write("OBJ1-wtVal1 = ");
 repeat(30)
 $write(" %0d ",cDpi_getVal("OBJ1","wtVal1",113));
 $display("");
 $write("OBJ1-wtValSeq = ");
 repeat(30)
 $write(" %0d ",cDpi_getVal("OBJ1","wtValSeq",113));
 $display("");
 $write("OBJ1-wtVal1Seq = ");
 repeat(30)
 $write(" %0d ",cDpi_getVal("OBJ1","wtVal1Seq",113));
 $display("");
 $write("OBJ1-wtVal1SeqU = ");
 repeat(30)
 $write(" %0d ",cDpi_getVal("OBJ1","wtVal1SeqU",113));
 $display("");
 //str= "ffffffff";
 //$display("atoi.-10 %d",str.atohex());
 repeat(5)
 $write("OBJ2 new_val_arr[0] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",0,113));
 $display("");
 repeat(5)
 $write("OBJ2 new_val_arr[1] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",1,113));
 $display("");
 repeat(5)
 $write("OBJ2 new_val_arr[4] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",4,113));
 $display("");
 repeat(5)
 $write("OBJ2 new_val_arr[10] = %0d ",cDpi_getVal_("OBJ2","new_val_arr",'ha,113));
 $display("");
 $write("OBJ2 uniqVal =");
 repeat(50)
 $write("%x ",cDpi_getVal("OBJ2","uniq_val",113));
 $display("");
end 

endprogram 
