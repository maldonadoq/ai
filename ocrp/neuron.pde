float learning_rate = 0.01;

class cneuron{
    cneuron [] inputs;  //previous layer
    float [] weights;
    float output;
    float error;

    cneuron(){  error = 0.0;    }

    cneuron(cneuron [] p_inputs){
        inputs = new cneuron [p_inputs.length];
        weights = new float [p_inputs.length];

        error = 0.0;
        for (int i = 0; i < inputs.length; i++) {
            inputs[i] = p_inputs[i];
            weights[i] = random(-1.0, 1.0);
        }
    }

    void respond(){
        float input = 0.0;
        for(int i=0; i<inputs.length; i++){
            input += inputs[i].output * weights[i];
        }

        output = lookup_sigmoid(input);
        error = 0.0;
    }

    void set_error(float desired) {
        error = desired - output;
    }

    void train() {

        float delta =(1.0 - output) * (1.0 + output) *
            error * learning_rate;
        for (int i = 0; i < inputs.length; i++) {
            inputs[i].error += weights[i] * error;
            weights[i] += inputs[i].output * delta;
        }
    }

    void display(){
        stroke(200);
        fill(128*(1-output));
        ellipse(0,0,16,16);
    }    
}