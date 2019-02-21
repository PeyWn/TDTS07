//This file was generated from (Commercial) UPPAAL 4.0.14 (rev. 5615), May 2014

/*

*/
A[] not deadlock

/*

*/
E<> EW.Green 

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
E<> not ((NS.Green and WE.Green) or (SN.Green and WE.Green) or (NS.Green and EW.Green) or (SN.Green and EW.Green))
