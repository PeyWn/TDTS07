INCDIR	     = -I. -I${SWARMDIR}/core -I${SWARMDIR}/../apps/support/simulator
OPT	     = -g -O3
CFLAGS       = $(INCDIR) $(OPT)

all:
	arm-rtems-as -mfpu=softfpa -o test.o ${SWARMDIR}/../apps/support/simulator/test.s
	arm-rtems-gcc $(CFLAGS) -c -o appsupport.o ${SWARMDIR}/../apps/support/simulator/appsupport.c
	arm-rtems-gcc $(CFLAGS) -c hello_world.c  -o hello_world.o
	arm-rtems-ld -T ${SWARMDIR}/../apps/support/simulator/test.ld -o linked.o test.o appsupport.o hello_world.o
	arm-rtems-objcopy -O binary linked.o TargetMem_1.mem
	cp TargetMem_1.mem TargetMem_2.mem
