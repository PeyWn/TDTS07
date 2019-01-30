#!/bin/bash
 
count=10
i=3

while [ $i -lt $count ]; do
	let cache_size=$i+1;
	echo $cache_size;
        mpsim.x --intc=s -w --is=$cache_size -c2;
	sync;
	cp stats.txt set.fixed_f.var_icache.$i;
	cp stats_light.txt set.light.fixed_f.var_icache.$i;
        sync;
	i=`expr $i + 1`;
  done;

