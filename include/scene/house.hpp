camera = new ThinLenCamera(vec(0.55347, -11.174, 1.814), vec(-0.1, 1, 0), vec(0, 0, 1),
                           rad(40), 0, 2, resolution, resolution * 4 / 3);

// add(new Sphere(vec(2.7191, 3.5938, 4.4326), 3, light));
add(new Sphere(vec(-3.1372, -0.71179, 2.4333), 0.5, light));
// add(new Sphere(vec(-2, -1.3, 2.4333), 1, light));
add(new Sphere(vec(2.6494, -2.1917, 2.1047), 1, light));
// add(new Mesh("mesh/house.obj", diff_green, Matrix4f::identity(), false));
add(new Mesh("mesh/chair.obj", diff_green, Matrix4f::identity(), false));