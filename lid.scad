$fa = 1;
$fs = 0.5;

r = 11.3;
h = 3;
bpcb = 19;
tpcb = 1.8;
busb = 9;
tusb = 3;
bwindow = 10;
twindow = 2;
rk = 2.1;
versk = -1;
vers_x_pcb = 4;
vers_x_usb = vers_x_pcb;

difference() {
    cylinder(h,r,r);
// cutout for cable
    translate([versk,3,-0.5]) cylinder(h+1,rk,rk);
// cutout for pcb
    translate([vers_x_pcb,-(bpcb/2-(tpcb/2)),1.5]) cylinder(h,(tpcb/2),(tpcb/2));
    translate([vers_x_pcb,(bpcb/2-(tpcb/2)),1.5]) cylinder(h,(tpcb/2),(tpcb/2));
    translate([vers_x_pcb,0,1.5+h/2]) cube([tpcb,bpcb-tpcb,3],center=true);
// cutout for micro usb    
    translate([vers_x_usb+2,-(busb/2-(tusb/2)),-0.5]) cylinder(h+1,(tusb/2),(tusb/2));
    translate([vers_x_usb+2,(busb/2-(tusb/2)),-0.5]) cylinder(h+1,(tusb/2),(tusb/2));
    translate([vers_x_usb+2,0,h/2]) cube([tusb,busb-tusb,5],center=true);
    translate([vers_x_usb+1,0,1.5+h/2]) cube([tpcb,busb,3],center=true);
// pseudo window    
    *translate([6.5,-busb/2,0.4]) cylinder(h,(twindow/2),(twindow/2));
    *translate([7,(bwindow/2-(twindow/2)),0.4]) cylinder(h,(twindow/2),(twindow/2));
    *translate([7,0,2.4]) cube([twindow,bwindow-twindow,4],center=true);
}

*translate([vers_x_pcb+1.8,1+busb/2,h+1])cube([2,2,2],center=true);
*translate([vers_x_pcb+1.8,-1-busb/2,h+1])cube([2,2,2],center=true);
translate([vers_x_pcb-2,-4,h+1])cube([2,8,2],center=true);
