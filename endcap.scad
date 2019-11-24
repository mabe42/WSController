$fa = 1;
$fs = 0.5;

r = 11.3;
h = 3;

difference() {
    union() {
        cylinder(h,r,r);
        translate([0,0,h])cylinder(1,r+0.2,r);
        translate([0,0,h+1])cylinder(1,r+0.3,r);
        translate([0,0,h+2])cylinder(1,r+0.4,r);
        translate([0,0,h+3])cylinder(1,r+0.5,r);
    }
    translate([0,0,2.5])cylinder(2*h,r-1.5,r-1.3);
    translate([0,0,7.5])cube([8,30,10],center=true);
    translate([0,0,7.5])cube([30,8,10],center=true);
}
