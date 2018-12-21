class rotate : driderBehavior {

	float velocity = 10.0f;
	
	void Start() {
		Print("\nScript initialized\n");
	}

	void Update() {
		Vector3D vectorUp(0.0,velocity * getDelta(),0.0);

		
		transform.rotate(vectorUp);
			

	}

}