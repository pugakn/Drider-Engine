void Start() {
	Vector3D vec1(1.0,1.0,1.0);
	Vector3D vec2(2.0,2.0,2.0);

	Vector3D r(0.0,0.0,0.0);
	r = vec1 + vec2;

	Vector3D test(3.0,3.0,3.0);

	if(r == test)
		Print("Vectors are equals");
}

Vector3D front(10.0,0.0,0.0);
Vector3D back(-10.0,0.0,0.0);
Vector3D right(0,0.0,10.0);
Vector3D left(0.0,0.0,-10.0);

void Update() {
	float a = 10.0f;

	if(isKeyDown(kW)) {
		//Print("W\n");
		transform.move(front);
	}
	if(isKeyDown(kS)) {
		//Print("S\n");
		transform.move(back);
	}
	if(isKeyDown(kA)) {
		//Print("S\n");
		transform.move(right);
	}
	if(isKeyDown(kD)) {
		//Print("S\n");
		transform.move(left);
	}
}	 
