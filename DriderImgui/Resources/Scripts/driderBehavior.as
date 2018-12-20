class driderBehavior : IScript {

	private void SetTransform(Transform@ trans) {
		@transform = trans;
	}

	private void SetGameObject(GameObject@ obj) {
		@gameObject = obj;
	}

	private void Start() {

	}
	private void Update() {
		
	}

	Transform@ transform;
	GameObject@ gameObject;

}