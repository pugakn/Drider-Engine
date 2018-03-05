void Start() {
	//Print("Start\n");
	Camera cam;
	//cam.getFarPlane();

	//cam.move(1.0,2.0,50.0,false);

	/*Vector3D vec(10.0,10.0,10.0);
	Vector3D vec2(1.0,2.0,5.0);
	Vector3D vec3(10.0,11.0,54.2);

	vec3 = vec;

	float a = vec.dot(vec2);
	vec3 = vec.crossRef(vec2);
	vec.length();
	vec.lengthSqr();
	vec.normalize();
	vec.distance(vec2);
	vec.distanceSqr(vec2);

	vec -= vec2;
	vec += vec2;
	vec *= vec2;
	vec *= 12.2;*/

	Object obj(2.0,30.3);
	Object obj2(1.0,1.0);

	Object obj3(0.0,0.0);
	obj3 = obj + obj2;



}

void Update() {
	//Print("Update\n");
	Camera cam;
	//cam.getNearPlane();
}	 
