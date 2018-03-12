void Start() {
	Vector3D vec1(1.0,1.0,1.0);
	Vector3D vec2(2.0,2.0,2.0);

	Vector3D r(0.0,0.0,0.0);
	r = vec1 + vec2;

	Vector3D test(3.0,3.0,3.0);

	if(r == test)
		Print("Vectors are equals");
}

void Update() {
	
	float time = time + getDelta();
	float moveVel = 500.0 * getDelta();

	if(isKeyDown(kW)) {
		//Print("W\n");
		transform.move(Vector3D(moveVel,0.0,0.0));
	}
	if(isKeyDown(kS)) {
		//Print("S\n");
		transform.move(Vector3D(moveVel * -1.0,0.0,0.0));
	}
	if(isKeyDown(kA)) {
		//Print("S\n");
		transform.move(Vector3D(0,0.0,moveVel));
	}
	if(isKeyDown(kD)) {
		//Print("S\n");
		transform.move(Vector3D(0.0,0.0,moveVel * -1.0));
	}

	Print(time + "\n");

	if(isKeyDown(kR)) {
		Print("R\n");
		transform.rotate(Vector3D(0.0,0.0,3.1416));
	}
}	 
