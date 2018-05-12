class script1 : MontiBehavior {

	float timer;

	void Start() {
		Print("Start\n");
		
		GameObject@ a = Object.findObject("Floor");
		GameObject@ ch = Object.getChildByIndex(0);
	}

	void Update() {
		Vector3D vec(100.0 * getDelta(),0.0,0.0);

		if(isKeyDown(kD)) {
			this.transform.move(vec);
			//Print("move, x = " + this.transform.m_position.x + "\n");
		}

		if(isKeyDown(kA)) {
			this.transform.move(vec * -1.0);
			//Print("move, x = " + this.transform.m_position.x + "\n");
		}
	}

	/*void onKeyDown(KeyCode key) {
		if(key == kD) {
			Print("D key pressed!\n");
			Vector3D vec(1.0,0.0,0.0);
			this.transform.move(vec);
		}

		Print("onKeyDown\n");
	}

	void onKeyUp(KeyCode key) {
		if(key == kD) {
			Vector3D vec(10.0,0.0,0.0);
			this.transform.move(vec);
		}

		if(key == kA) {
			Vector3D vec(-10.0,0.0,0.0);
			this.transform.move(vec);
		}
	}*/
}


