void Start() {
	Print("Start script1: ");
	Print(script1::globalVar + "\n");
}

void Update() {
	//Print("Update script1: \n");
	/*if(isKeyDown(k0)) {
		Print("Update script1: \n");
	}*/
}

void onKeyDown(KeyCode key) {
	if(key == k0)
		Print("onKeyDown\n");
}
