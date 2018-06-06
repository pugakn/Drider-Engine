class script2 : driderBehavior {

	TString name = "TEST_NAME";

	void Start() {
		
	
		Print("\nThis is a test from the Print function");

		Log("This is a test from the Log function");
		Log("Testing from the log:");
		Log("int:");
		Log(5);
		Log("float:");
		Log(3.4);
		Log("double:");
		Log(6.666666);
		Log("bool (3==5?):");
		Log(3==5);

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