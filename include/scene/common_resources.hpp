Vector3f white = {1, 1, 1};
Vector3f red = {0.75, 0.25, 0.25};
Vector3f green = {0.25, 0.75, 0.25};
Vector3f blue = {0.25, 0.25, 0.75};
Vector3f grey = {0.5, 0.5, 0.5};
Texture *earth = add(new Texture("texture/earth.png"));
Texture *wood = add(new Texture("texture/wood.png"));
Texture *marble = add(new Texture("texture/marble.png"));
Texture *girl = add(new Texture("texture/girl.png"));
Texture *morning = add(new Texture("texture/skylight-morn.png"));
Texture *morning_half = add(new Texture("texture/skylight-morn-half.png"));

Material *light = add(new Material({{new DiffuseBSDF(), 1}}, white, {15, 15, 15}));
Material *light_white = add(new Material({{new DiffuseBSDF(), 1}}, white, {4, 4, 4}));
Material *light_white0 = add(new Material({{new DiffuseBSDF(), 1}}, white, {0.5, 0.5, 0.5}));
Material *light_red = add(new Material({{new DiffuseBSDF(), 1}}, red, {4, 0, 0}));
Material *light_green = add(new Material({{new DiffuseBSDF(), 1}}, green, {0, 4, 0}));
Material *light_blue = add(new Material({{new DiffuseBSDF(), 1}}, blue, {0, 0, 4}));
Material *diff_white = add(new Material({{new DiffuseBSDF(), 1}}, white));
Material *diff_red = add(new Material({{new DiffuseBSDF(), 1}}, red));
Material *diff_green = add(new Material({{new DiffuseBSDF(), 1}}, green));
Material *diff_blue = add(new Material({{new DiffuseBSDF(), 1}}, blue));
Material *diff_grey = add(new Material({{new DiffuseBSDF(), 1}}, grey));
Material *diff_earth = add(new Material({{new DiffuseBSDF(), 1}}, earth));
Material *diff_girl = add(new Material({{new DiffuseBSDF(), 1}}, girl));
Material *spec = add(new Material({{new SpecularBSDF(), 1}}, white));
Material *refr = add(new Material({{new RefractiveBSDF(1.5), 1}}, white));
Material *refr_red = add(new Material({{new RefractiveBSDF(1.5), 1}}, red));
Material *refr_green = add(new Material({{new RefractiveBSDF(1.5), 1}}, green));
Material *refr_blue = add(new Material({{new RefractiveBSDF(1.5), 1}}, blue));
Material *refr_grey = add(new Material({{new RefractiveBSDF(1.5), 1}}, grey));
Material *refr_white0 = add(new Material({{new DiffuseBSDF(), 0.3}, {new RefractiveBSDF(1.5), 0.7}}, white));