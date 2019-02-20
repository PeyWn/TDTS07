//This file was generated from (Commercial) UPPAAL 4.0.14 (rev. 5615), May 2014

/*
Fischer's mutual exclusion protocol.
*/
//NO_QUERY

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

*/
//NO_QUERY
