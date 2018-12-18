class test : driderBehavior {

	void Start() {
		Print("\nScript initialized\n");
	}

	void Update() {
		Vector3D vecRight(100.0 * getDelta(),0.0,0.0);
		Vector3D vecFront(0,0.0,100.0 * getDelta());

		
		if(isKeyDown(kD)) {
			transform.move(vecRight);
		}

		if(isKeyDown(kA)) {
			transform.move(vecRight * -1.0f);
		}
			

	}

	void onKeyDown(KeyCode key) {
		if(isKeyDown(kD)) {
			Print("\nPress D\n");
			Print("\nPress asdasD\n");

			
			/*GameObject@ player = Object.findObject("Model");
			SoundComponent@ sound = cast<SoundComponent>(player.getComponent("SoundComponent"));
			if(@sound != null) {
				Print("\nPlay sound\n");
				sound.play("testSound1.mp3");
			}
			else {
				Print("\nSound null");
			}

				//transform.move(vecRight);
			}*/

	}

}