class player : driderBehavior {

	void Start() {
		Print("\nPlayer spawned\n");
	}

	void Update() {
		/*Vector3D vecRight(100.0 * getDelta(),0.0,0.0);
		Vector3D vecFront(0,0.0,100.0 * getDelta());

		if(isConnected) {
			if(isKeyDown(kD)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecRight);
			}
			if(isKeyDown(kA)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecRight * -1.0);
			}
			if(isKeyDown(kW)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecFront);
			}
			if(isKeyDown(kS)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecFront * -1.0);
			}
		}*/

	}

	void onKeyDown(KeyCode key) {
		Vector3D vecRight(100.0 * getDelta(),0.0,0.0);
		Vector3D vecFront(0,0.0,100.0 * getDelta());

		if(isConnected) {

			if(isKeyDown(kD)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecRight);
			}

			if(isKeyDown(kA)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecRight * -1.0);
			}

			if(isKeyDown(kW)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecFront);
			}

			if(isKeyDown(kS)) {
				GameObject@ player = Object.findObject("Player");
				NetworkManagerComponent@ net = cast<NetworkManagerComponent>(player.getComponent("NetworkManagerComponent"));
				net.move(vecFront * -1.0);
			}
		}
	}

}