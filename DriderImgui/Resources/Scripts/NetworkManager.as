class NetworkManager : driderBehavior {

	void Start() {
		
	}

	void Update() {

	}

	void onKeyDown(KeyCode key) {
		if(isKeyDown(kI)) {
			Print("\nPress I\n");
			NetworkManagerComponent@ net = cast<NetworkManagerComponent>(gameObject.getComponent("NetworkManagerComponent"));
			if(@net != null) {
				Vector3D pos(50.0,0.0,0.0);
				Vector3D dir(0.0,0.0,0.0);
				net.instantiate(kPlayer, pos, dir);
			}
			else {
				Print("\nNetwork null");
			}
		}
	}

}