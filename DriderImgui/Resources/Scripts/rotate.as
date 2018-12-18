class rotate : driderBehavior {

	void Start() {
		Print("\nScript initialized\n");
	}

	void Update() {
		Vector3D vectorUp(0.0,10.0 * getDelta(),0.0);

		
		transform.rotate(vectorUp);
			

	}

}