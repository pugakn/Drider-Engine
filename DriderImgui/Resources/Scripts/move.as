class move : driderBehavior {

	//int8 nel = 1;
	float velocity = 100.0f;
	float otherVar = 50.0f;
	bool bMove = true;
	

	void Start() {
		Print("\nScript initialized\n");
	}

	void Update() {
		Vector3D vecRight(velocity * getDelta(),0.0,0.0);
		Vector3D vecFront(0,0.0,velocity * getDelta());

		if(@gameObject == null) {
			Print("\nObject null\n");
		}

		if(bMove) {
			if(isKeyDown(kD)) {
				transform.move(vecRight);
			}

			if(isKeyDown(kA)) {
				transform.move(vecRight * -1.0f);
			}

			if(isKeyDown(kW)) {
				transform.move(vecFront);
			}

			if(isKeyDown(kS)) {
				transform.move(vecFront * -1.0f);
			}
		}	

	}

	void onKeyDown(KeyCode key) {
		if(isKeyDown(kD)) {
			Print("\nPress D\n");
			SoundComponent@ sound = cast<SoundComponent>(gameObject.getComponent("SoundComponent"));
			if(@sound != null) {
				Print("\nPlay sound\n");
				sound.play("testSound1.mp3");
			}
			else {
				Print("\nSound null");
			}
		}
	}

}