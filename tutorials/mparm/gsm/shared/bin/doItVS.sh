#!/bin/bash
 
count_i=5;
i=1;
count_j=5;
j=1;
while [ $i -lt $count_i ]; do
  j=1;
  while [ $j -lt $count_j ]; do
        mpsim.x --intc=s -w -F1,$i -F2,$j -c2;
        #echo $j
	sync;
	cp stats.txt set.var_f.fixed_icache.$i.$j;
	cp stats_light.txt set.light.var_f.fixed_icache.$i.$j;
        sync;
	j=`expr $j + 1`;
  done;
  i=`expr $i + 1`;
done;

