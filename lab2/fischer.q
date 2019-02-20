//This file was generated from (Commercial) UPPAAL 4.0.14 (rev. 5615), May 2014

/*
Mutex requirement.
*/
A[] not ( ( P1.cs and ( P2.cs or P3.cs or P4.cs ) ) or \
          ( P2.cs and ( P3.cs or P4.cs ) ) or \
          ( P3.cs and P4.cs ) \
        )

/*
The system is deadlock free.
*/
A[] not deadlock

/*
Whenever P1 requests access to the critical section it will eventually enter the wait state.
*/
P1.req --> P1.wait

/*
n = 12
*/
A[] not  (\
            ( P1.cs and ( P2.cs or P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs or P12.cs ) ) or\
            ( P2.cs and ( P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs or P12.cs ) ) or \
            ( P3.cs and ( P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs or P12.cs ) ) or \
            ( P4.cs and ( P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs or P12.cs ) ) or \
            ( P5.cs and ( P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs or P12.cs ) ) or \
            ( P6.cs and ( P7.cs or P8.cs or P9.cs or P10.cs or P11.cs or P12.cs ) ) or \
            ( P7.cs and ( P8.cs or P9.cs or P10.cs or P11.cs or P12.cs ) ) or\
            ( P8.cs and ( P9.cs or P10.cs or P11.cs or P12.cs ) ) or\
            ( P9.cs and ( P10.cs or P11.cs or P12.cs ) ) or\
            ( P10.cs and ( P11.cs or P12.cs ) ) or\
            ( P11.cs and P12.cs )\
            )

/*
n = 11
*/
A[] not  (\
            ( P1.cs and ( P2.cs or P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs ) ) or\
            ( P2.cs and ( P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs ) ) or \
            ( P3.cs and ( P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs ) ) or \
            ( P4.cs and ( P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs ) ) or \
            ( P5.cs and ( P6.cs or P7.cs or P8.cs or P9.cs or P10.cs or P11.cs ) ) or \
            ( P6.cs and ( P7.cs or P8.cs or P9.cs or P10.cs or P11.cs ) ) or \
            ( P7.cs and ( P8.cs or P9.cs or P10.cs or P11.cs ) ) or\
            ( P8.cs and ( P9.cs or P10.cs or P11.cs ) ) or\
            ( P9.cs and ( P10.cs or P11.cs ) ) or\
            (P10.cs and P11.cs)\
            )

/*
n = 10
*/
A[] not  (\
            ( P1.cs and ( P2.cs or P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs ) ) or\
            ( P2.cs and ( P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs ) ) or \
            ( P3.cs and ( P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs ) ) or \
            ( P4.cs and ( P5.cs or P6.cs or P7.cs or P8.cs or P9.cs or P10.cs ) ) or \
            ( P5.cs and ( P6.cs or P7.cs or P8.cs or P9.cs or P10.cs ) ) or \
            ( P6.cs and ( P7.cs or P8.cs or P9.cs or P10.cs ) ) or \
            ( P7.cs and ( P8.cs or P9.cs or P10.cs ) ) or\
            ( P8.cs and ( P9.cs or P10.cs ) ) or\
            ( P9.cs and P10.cs ) \
            )

/*
n = 9
*/
A[] not  (\
            ( P1.cs and ( P2.cs or P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs) ) or\
            ( P2.cs and ( P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs ) ) or \
            ( P3.cs and ( P4.cs or P5.cs or P6.cs or P7.cs or P8.cs or P9.cs ) ) or \
            ( P4.cs and ( P5.cs or P6.cs or P7.cs or P8.cs or P9.cs ) ) or \
            ( P5.cs and ( P6.cs or P7.cs or P8.cs or P9.cs ) ) or \
            ( P6.cs and ( P7.cs or P8.cs or P9.cs ) ) or \
            ( P7.cs and ( P8.cs or P9.cs ) ) or\
            ( P8.cs and P9.cs) \
            )

/*
n = 8
*/
A[] not  (\
            ( P1.cs and ( P2.cs or P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs ) ) or\
            ( P2.cs and ( P3.cs or P4.cs or P5.cs or P6.cs or P7.cs or P8.cs ) ) or \
            ( P3.cs and ( P4.cs or P5.cs or P6.cs or P7.cs or P8.cs ) ) or \
            ( P4.cs and ( P5.cs or P6.cs or P7.cs or P8.cs ) ) or \
            ( P5.cs and ( P6.cs or P7.cs or P8.cs ) ) or \
            ( P6.cs and ( P7.cs or P8.cs ) ) or \
            ( P7.cs and P8.cs )\
            )
