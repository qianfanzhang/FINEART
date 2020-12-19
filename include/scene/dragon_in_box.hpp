camera = new ThinLenCamera(vec(0, 0.4, 7), vec(0, -0.17, -1), vec(0, 1, 0),
                           rad(40), 0, 4.8, resolution, resolution);

group->add(new Sphere(vec(0, 3.5, 0.75), 1.82, light));

group->add(new Sphere(vec(0, 0, -3), 1.9, light_white0));
group->add(new Sphere(vec(3.5, 0, 3.75), 1.82, light_white));
group->add(new Sphere(vec(-3.5, 0, 3.75), 1.82, light_white));

group->add(new Plane(vec(0, 0, 1), -2, diff_grey));
group->add(new Plane(vec(0, 0, 1), -7.1, diff_grey));
group->add(new Plane(vec(0, 1, 0), -2, diff_grey));
group->add(new Plane(vec(0, 1, 0), 2, diff_grey));
group->add(new Plane(vec(1, 0, 0), -2, diff_red));
group->add(new Plane(vec(1, 0, 0), 2, diff_blue));

group->add(new Mesh("mesh/fixed.perfect.dragon.100K.obj", refr_green, Matrix4f::translation(0.5, -1, 1.1) * Matrix4f::uniformScaling(1.5)));
group->add(new Mesh("mesh/bunny.fine.obj", diff_red, Matrix4f::translation(-0.5, -2.35, -0.5) * Matrix4f::uniformScaling(10)));

// medium = new Medium({0.00011, 0.00024, 0.0014}, {0.03, 0.04, 0.04}, 0);