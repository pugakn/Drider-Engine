class MontiBehavior {

	void Start() {
		Print("Start base\n");
	}

	void Update() {
		Print("Update base\n");
	}

	void onKeyDown(KeyCode key) {
		Print("onKeyDown base\n");
	}

	private void SetTransform(Transform@ trans) {
		@transform = trans;
	}

	Transform@ transform;

}

class script1 : MontiBehavior {

	void Start() override {
		this.transform.m_position.x -= 10;
		Print("Start derived, x = " + this.transform.m_position.x);
	}

	void Update() override {
		Print("Update derived\n");
	}

	void onKeyDown(KeyCode key) override {
		if(key == kD) {
			Vector3D vec(1.0,0.0,0.0);
			this.transform.move(vec);
		}
	}
}


