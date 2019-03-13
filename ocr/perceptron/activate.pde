float [] g_sigmoid = new float [200];

void setup_sigmoid() {  
	for (int i = 0; i < 200; i++) {
		float x = (i / 20.0) - 5.0;
		g_sigmoid[i] = 2.0 / (1.0 + exp(-2.0 * x)) - 1.0;
	}
}

float lookup_sigmoid(float x){
	return g_sigmoid[constrain((int)floor((x+5.0)*20.0), 0, 199)];	
}