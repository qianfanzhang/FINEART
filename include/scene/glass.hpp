camera = new ThinLenCamera(vec(0, 0.4, 7), vec(0, -0.17, -1), vec(0, 1, 0),
                           rad(40), 0, 4.8, resolution, resolution);

group->add(new Sphere(vec(0, 3.5, 0.75), 1.82, light));
group->add(new Sphere(vec(0, 3.5, 0), 1.82, light));

group->add(new Plane(vec(0, 0, 1), -2, diff_grey));
group->add(new Plane(vec(0, 0, 1), -7.1, diff_grey));
group->add(new Plane(vec(0, 1, 0), -2, diff_grey));
group->add(new Plane(vec(0, 1, 0), 2, diff_grey));
group->add(new Plane(vec(1, 0, 0), -2, diff_red));
group->add(new Plane(vec(1, 0, 0), 2, diff_blue));

add(new Mesh("mesh/chair.obj", diff_green, Matrix4f::rotateX(rad(-90)) * Matrix4f::identity(), false));
// add(new Mesh("mesh/glass.obj", refr_blue, Matrix4f::translation(1, -0.5, 1.4) * Matrix4f::rotateX(rad(-90)) * Matrix4f::uniformScaling(0.5), true));