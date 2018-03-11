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
	float a = 10.0f;

	if(isKeyDown(k0))
		Print("Escape down\n");
}	 
