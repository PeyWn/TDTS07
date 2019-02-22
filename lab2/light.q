//This file was generated from (Commercial) UPPAAL 4.0.14 (rev. 5615), May 2014

/*

*/
WE.Wait --> WE.Green

/*

*/
E<> WE.Green

/*

*/
E<> SN.Green

/*

*/
E<> NS.Green

/*

*/
A[] not deadlock

/*

*/
E<> NS.Green and SN.Green

/*

*/
E<> not (((NS.Green or SN.Green) and (EW.Green or WE.Green)))
