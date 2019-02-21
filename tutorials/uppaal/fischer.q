//This file was generated from UPPAAL 3.2.1, October 2001

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
