class script2 : driderBehavior {

	TString name = "TEST_NAME";

	void Start() {
		//Print("Script2 start\n");
		GameObject@ player = Object.findObject("Player");
		ScriptComponent@ sc = cast<ScriptComponent>(player.getComponent("ScriptComponent"));
		script1@ a = cast<script1>(sc.getScript());
		if(a == null) {
			Print("\nScript null");
		} else {
			a.Do();
		}

		TString name = "NEW_NAME";

	}

	void Update() {
		//Print("Update script2\n");

	}

	void doSomething() {
		Print(name);
	}

}