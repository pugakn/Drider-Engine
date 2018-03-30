class MontiBehavior {

	private void SetTransform(Transform@ trans) {
		@transform = trans;
	}

	Transform@ transform;

}

class script1 : MontiBehavior {

	void Start() {
		Vector3D vec(10.0,0.0,0.0);
		this.transform.move(vec);
		Print("move, x = " + this.transform.m_position.x + "\n");
	}

	void Update() {
		//Print("Update derived\n");
	}

	/*void onKeyDown(KeyCode key) {
		if(key == kD) {
			Print("D key pressed!\n");
			Vector3D vec(1.0,0.0,0.0);
			this.transform.move(vec);
		}

		Print("onKeyDown\n");
	}*/

	void onKeyUp(KeyCode key) {
		if(key == kD) {
			Vector3D vec(10.0,0.0,0.0);
			this.transform.move(vec);
			Print("move, x = " + this.transform.m_position.x + "\n");
		}
	}
}


