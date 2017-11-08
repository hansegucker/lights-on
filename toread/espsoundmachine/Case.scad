res = 32;

module button() {
    union() {
        hull() {
            for(x=[-1, 1]) {
                for(y=[-1, 1]) {
                    translate([4*x, 4*y, 0]) cylinder(d=3, h=8, $fn=res);
                }
            }
        }
        translate([0, 0, 1]) cube([13, 6, 2], center=true);
        translate([0, 0, 1]) cube([6, 13, 2], center=true);
        difference() {
            translate([0, 0, 10]) sphere(d=22, $fn=res);
            for(i=[-1, 1]) {
                translate([i*10, 0, 5]) cube([10, 20, 10], center=true);
                translate([0, i*10, 5]) cube([20, 10, 10], center=true);
            }
            translate([0, 0, 15]) cube([30, 30, 20], center=true);
        }
    }
}

module button_base() {
    difference() {
        union() {
            translate([0, 0, 1]) cube([80, 78, 2], center=true);
            for(i=[-1, 1]) {
                for(j=[-1, 1]) {
                    translate([i*31, j*35.5, 0]) cylinder(d=13, h=13.7, $fn=res);
                }
                translate([0, 0, 2+3]) rotate([0, 0, i*49]) cube([84, 6, 6], center=true);
                translate([i*31, 0, 2+3]) cube([6, 60, 6], center=true);
                translate([0, i*35.5, 2+3]) cube([51, 6, 6], center=true);
            }
        }
        for(i=[-1, 1]) {
            for(j=[-1, 1]) {
                translate([i*31, j*35.5, -1]) cylinder(d=3.5, h=17, $fn=res);
                translate([i*31, j*35.5, 11.2]) cylinder(d=6.7, h=5, $fn=6);
            }
        }
        translate([0, -42, 0]) cube([100, 5, 50], center=true);
    }
}

module case() {
    union() {
        difference() {
            union() {
                for(x=[-1, 1]) for(y=[-1, 1]) {
                   translate([x*(88-13)/2, y*(182-13)/2, 3]) {
                        difference() {
                            cylinder(d=10, h=34, $fn=res);  
                            translate([x*-5, 0, 40]) rotate([0, 45, 0]) cube([20, 20, 20], center=true);
                            translate([0, 0, 25]) cylinder(d=6.7, h=20, $fn=6);
                        }
                   }
                   translate([x*31, -48+y*35.5, 57-(3.35+3)]) cylinder(d=7, h=3.35, $fn=res);
               }
            }
            for(x=[-1, 1]) for(y=[-1, 1]) {
                translate([x*(88-13)/2, y*(182-13)/2, 1]) cylinder(d=3.5, h=55, $fn=res);
                translate([x*31, -48+y*35.5, 57-24]) cylinder(d=3.5, h=26, $fn=res);
                translate([x*31, -48+y*35.5, 57-3.3]) cylinder(d=5.8, h=5, $fn=res);
            }
        }
        difference() {
            translate([0, 0, 57/2]) render(convexity=2) hull() { for(x=[-1, 1]) for(y=[-1, 1]) for(z=[-1, 1]) translate([x*(88-10)/2, y*(182-10)/2, z*(57-10)/2]) sphere(d=10, $fn=res); }
            //translate([0, 0, 57/2]) cube([88, 182, 57], center=true);
            translate([0, 0, 57/2]) render(convexity=2) hull() { for(x=[-1, 1]) for(y=[-1, 1]) for(z=[-1, 1]) translate([x*(88-10)/2, y*(182-10)/2, z*(57-10)/2]) sphere(d=4, $fn=res); }
            //translate([0, 0, 57/2]) cube([88-6, 182-6, 57-6], center=true);
            for(x=[-3,3]) {
                for(y=[-3,3]) {
                    translate([x*10, y*10+46, 10]) cylinder(d=3.5, h=60, $fn=res);
                }
            }
            // Speaker holes
            translate([0, 46, 10]) {
                //cylinder(d=6, h=60, $fn=res);
                for(i=[0:2]) {
                    rotate([0, 0, i*120]) translate([0, 4, 0]) cylinder(d=6, h=60, $fn=res);
                }
                for(i=[0:8]) {
                    rotate([0, 0, i*40+20]) translate([0, 11, 0]) cylinder(d=6, h=60, $fn=res);
                }
                for(i=[0:14]) {
                    rotate([0, 0, i*24]) translate([0, 18, 0]) cylinder(d=6, h=60, $fn=res);
                }
                for(i=[0:19]) {
                    rotate([0, 0, i*18+9]) translate([0, 25, 0]) cylinder(d=6, h=60, $fn=res);
                }
                for(i=[0:23]) {
                    rotate([0, 0, i*15]) translate([0, 32, 0]) cylinder(d=6, h=60, $fn=res);
                }
            }
            
            translate([0, -100, 22]) rotate([-90, 0, 0]) cylinder(d=4, h=50, $fn=res);
            translate([-25, 0, 10]) {
                difference() {
                    cylinder(d=6.7+0.4, h=60, $fn=res);
                    for(i=[-1, 1]) {
                        translate([0, i*((5.9+0.4)/2+5), 48]) cube([10, 10, 10], center=true);
                    }
                }
            }
            translate([20, 0, 10]) cylinder(d=5.5, h=60, $fn=res);
            translate([28, -9.3/2, 10]) cube([4, 9.3, 60]);
            for(x=[0:3]) {
                for(y=[0:3]) {
                    hull() {
                        for(i=[-1, 1]) {
                            for(j=[-1, 1]) {
                                translate([-24+x*14.8+4*i, -73+y*14.8+4*j, 53]) cylinder(d=3.5, h=6, $fn=res);
                            }
                        }
                    }
                }
            }
            //translate([0, -45, 0]) cube([58.5, 69.5, 20], center=true);
            for(x=[-1, 1]) for(y=[-1, 1]) {
                translate([x*(88-13)/2, y*(182-13)/2, -1]) {
                    cylinder(d=3.5, h=44, $fn=res);
                    cylinder(d=5.8, h=4, $fn=res);
                }
                translate([x*31, -48+y*35.5, 57-3.3]) cylinder(d=5.8, h=5, $fn=res);
            }
        }
    }
}

module upper_half() {
    difference() {
        case();
        translate([0, 0, -5]) cube([85, 179, 57], center=true);
        translate([0, 0, -7]) cube([100, 200, 57], center=true);
        //translate([-100, -100, -100]) cube([100, 200, 200]);
    }
}

module lower_half() {
    difference() {
        case();
        difference() {
            translate([0, 0, 50]) cube([100, 200, 57], center=true);
            translate([0, 0, -5]) cube([84.5, 178.5, 56.8], center=true);
        }
        translate([30, 15, 50-28-2.8]) cube([24.2, 24.2, 5]);
        //translate([-100, -100, -100]) cube([100, 200, 200]);
    }
}

upper_half();
//lower_half();
/*for(x=[0:3]) {
    for(y=[0:3]) {
        translate([-24+x*14.8, -73+y*14.8, 52]) button();
    }
}*/
//translate([0, -48, 52-1.35]) rotate([0, 180, 0]) button_base();