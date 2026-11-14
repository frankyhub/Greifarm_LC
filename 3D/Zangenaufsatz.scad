/*
*******************************************
Programm Info: Greifarm_LC Greifzangen Aufsatz

*******************************************
Version: 14.11.2026 khf
*******************************************
*/

//***************   Auswahl   *************

//keine

//***************   Libraries  ************

//keine

//*****************************************

//***************  Parameter   *************
$fn=60;
//*****************************************

//**************   Programm  **************

//-----------------------------------

translate(v = [8, -43, 0]) 
linear_extrude(height = 3  )
import ("Zangenaufsatz.svg");


translate([60,20,-3])
color("red")
cylinder(3,2.5,2.5,$fn=60);

translate([60,30,-3])
color("green")
cylinder(3,2.5,2.5,$fn=60);

translate([40,42,-3])
color("blue")
cylinder(3,2.5,2.5,$fn=60);

translate([40,7.5,-3])
color("grey")
cylinder(3,2.5,2.5,$fn=60);