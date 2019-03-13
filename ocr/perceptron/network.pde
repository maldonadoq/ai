class cnetwork{
    cneuron [] input_layer;
    cneuron [] hidden_layer;
    cneuron [] output_layer;
    int best_index = 0;

    cnetwork(int inputs, int hidden, int outputs) {

        input_layer = new cneuron [inputs];
        hidden_layer = new cneuron [hidden];
        output_layer = new cneuron [outputs];

        for(int i=0; i<input_layer.length; i++){
            input_layer[i] = new cneuron();
        }

        for(int j=0; j<hidden_layer.length; j++){
            hidden_layer[j] = new cneuron(input_layer);
        }

        for(int k=0; k<output_layer.length; k++){
            output_layer[k] = new cneuron(hidden_layer);
        }
    }

    void respond(ccard card){

        for (int i=0; i<input_layer.length; i++){
            input_layer[i].output = card.inputs[i];
        }
        
        // now feed forward through the hidden layer
        for(int j=0; j<hidden_layer.length; j++){
            hidden_layer[j].respond();
        }

        for(int k=0; k<output_layer.length; k++){
            output_layer[k].respond();
        }
    }

    void display(){
        // Draw the input layer
        for (int i = 0; i < input_layer.length; i++) {
            pushMatrix();
            translate(
                (i%14) * height / 20.0 + width * 0.05, 
                (i/14) * height / 20.0 + height * 0.13);
            input_layer[i].display();
            popMatrix();
        }        

        // Draw the output layer
        float [] resp = new float [output_layer.length];
        float respTotal = 0.0;
        for (int k = 0; k < output_layer.length; k++) {
            resp[k] = output_layer[k].output;
            respTotal += resp[k]+1;
        }

        for (int k = 0; k < output_layer.length; k++) {
            pushMatrix();
            translate(width * 0.7, (k%10) * height / 15.0 + height * 0.15);
            output_layer[k].display();
            fill(150);
            strokeWeight(sq(output_layer[k].output)/2);
            line(12, 0, 25, 0);
            text(k%10, 40, 5);
            text(nfc(((output_layer[k].output+1)/respTotal)*100, 2) + "%", 55, 5);
            popMatrix();
            strokeWeight(1);
        }

        float best = -1.0;
        for (int i =0; i < resp.length; i++) {
            if (resp[i]>best) {
                best = resp[i];
                best_index = i;
            }
        }

        stroke(255, 0, 0);
        noFill();
        ellipse(width * 0.7, (best_index%10) * height / 15.0 + height * 0.15,  25, 25);
    }

    void train(float [] outputs) {
        // adjust the output layer
        for (int k = 0; k < output_layer.length; k++) {
            output_layer[k].set_error(outputs[k]);
            output_layer[k].train();
        }

        float best = -1.0;
        for (int i = 0; i < output_layer.length; i++) {
            if (output_layer[i].output > best) best_index = i;
        }

        // propagate back to the hidden layer
        for (int j = 0; j < hidden_layer.length; j++) {
            hidden_layer[j].train();
        }
    }    
}