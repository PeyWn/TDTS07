//This file was generated from (Commercial) UPPAAL 4.0.14 (rev. 5615), May 2014

/*

*/
N.Idle --> N.Wait

/*

*/
N.Wait --> N.Green

/*

*/
E<> W.Green

/*

*/
E<> S.Green

/*

*/
E<> N.Green

/*

*/
A[] not deadlock

/*

*/
E<> N.Green and S.Green

/*

*/
E<> not (((N.Green or S.Green) and (E.Green or W.Green)))
