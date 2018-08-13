class script1 : driderBehavior {

	float timer;
	TString name = "TEST_NAME";

	bool m_isRegistered;
	GameObject@ player;

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

		//Change tag and print it
		TString tag = a.getTag();
		Print("a tag is " + tag + "\n");
		a.setTag("NewTag");
		Print("a tag is " + a.getTag() + "\n");

		//Get gameobject's transform
		Transform t = a.getTransform();
		Print("Position a: " + t.m_position.x + "," + t.m_position.y);

		//Get soundComponent
		player = Object.findObject("LocalPlayer");
		SoundComponent@ sound = cast<SoundComponent>(player.getComponent("SoundComponent"));
		Print("\nQue paso!\n");
		if(@sound != null) {
			//Print("\nPlay sound\n");
			sound.play("testSound1");
		}
		else {
			Print("\nSound null");
		}

		name = "EDIT_TEST";

		script2@ sc = cast<script2>(cast<ScriptComponent>(player.getComponent("ScriptComponent1")).getScript());
		sc.doSomething();

		m_isRegistered = false;

	}

	void Update() {
		Vector3D vecRight(100.0 * getDelta(),0.0,0.0);
		Vector3D vecFront(0,0.0,100.0 * getDelta());

		/*if(isKeyDown(kC)) {
			connect();
		}*/

		if(isKeyDown(kD)) {
			this.transform.move(vecRight);
			//Print("move, x = " + this.transform.m_position.x + "\n");
		}

		if(isKeyDown(kA)) {
			this.transform.move(vecRight * -1.0);
			//Print("move, x = " + this.transform.m_position.x + "\n");
		}

		if(isKeyDown(kW)) {
			this.transform.move(vecFront);
		}

		if(isKeyDown(kS)) {
			this.transform.move(vecFront * -1.0);
		}

		/*if(isConnected && isKeyDown(kP)) {
			//Print("Conectado a red" + "\n");
			//NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
			

			if(@net != null) {
				Print("net found");
				m_isRegistered = true;
				net.registerFloat("m_vel", 10.0);
				Vector3D pos(0.0,0.0,0.0);
				Vector3D dir(0.0,0.0,0.0);
				net.instantiate(kPlayer, pos, dir);
			} else {
				Print("net not found");
			}
		}*/
	}

	void Do() {
		Print("\n" + name + "\n");
	}

	void onKeyDown(KeyCode key) {
		if(key == kP && isConnected) {
			Print("instantiate" + "\n");
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


