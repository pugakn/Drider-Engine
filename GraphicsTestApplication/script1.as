class script1 : MontiBehavior {

	float timer;

	void Start() {
		Print("Start\n");
		
		//Compare two GameObject ref
		GameObject@ a = Object.findObject("Floor");
		GameObject@ b = Object.findObject("Floor");

		GameObject@ c = a.findObject("Test");

		if(a is b)
			Print("a is equal to b\n");
		else
			Print("a is not equal to b\n");

		if(@c == null)
			Print("c is null\n");

		//Change tag and print
		TString tag = a.getTag();
		Print("a tag is " + tag + "\n");
		a.setTag("NewTag");
		Print("a tag is " + a.getTag() + "\n");

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


