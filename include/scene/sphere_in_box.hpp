camera = new ThinLenCamera(vec(0, 0, 4), vec(0, 0, -1), vec(0, 1, 0),
                           rad(70), 0, 4.8, resolution, resolution);

group->add(new Sphere(vec(0, 3.5, 0.75), 1, light));

//group->add(new Plane(vec(0, 0, 1), -2, diff_grey));
// group->add(new Plane(vec(0, 0, 1), -7.1, diff_grey));
group->add(new Plane(vec(0, 1, 0), -2, diff_grey));
// group->add(new Plane(vec(0, 1, 0), 2, diff_grey));
// group->add(new Plane(vec(1, 0, 0), -2, diff_red));
// group->add(new Plane(vec(1, 0, 0), 2, diff_blue));

group->add(new Sphere(vec(0.25, 0, 1.5), 0.7, diff_white));

medium = new Medium({0, 0, 0}, {0.05, 0.05, 0.05}, 0);