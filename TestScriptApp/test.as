void Start() {
	Print("Start !!!!!!!!!!!!!!!!!!!\n" + 10.0);
	Camera cam;
	cam.getFarPlane();

	//cam.move(1.0,2.0,50.0,false);

	Vector3D vec(10.0,10.0,10.0);
	Vector3D vec2(1.0,2.0,5.0);

	float a = vec.dot(vec2);
	//vec.cross(vec2);
	vec.length();
	vec.lengthSqr();
	vec.normalize();
	vec.distance(vec2);
	vec.distanceSqr(vec2);

}

void Update() {
	Print("Ojo, si no hay mas de ocho caracteres esto no sirve\nUpdate");
	Camera cam;
	cam.getNearPlane();
}	 