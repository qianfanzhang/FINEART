camera = new ThinLenCamera(vec(2.4, 2.4, 10), vec(0, 0, -1), vec(0, 1, 0),
                           rad(50), 0, 2, resolution, resolution);

add(new Plane(vec(0, 1, 0), 6, diff_grey));
add(new Plane(vec(0, -1, 0), 6, light_white0));
add(new Plane(vec(0, 0, 1), -10, diff_red));
add(new Mesh("mesh/steampunk_bike.obj", diff_green));
