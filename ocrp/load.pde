ccard [] testing_set;
ccard [] training_set;

class ccard{
    float [] inputs;
    float [] outputs;
    int output;

    ccard(){
        inputs = new float [196];   // 14*14
        outputs = new float[10];    // 0-9
    }

    void image_load(byte [] images, int offset){
        for(int i=0; i<196; i++) {
            inputs[i] = int(images[i+offset])/128.0-1.0;    // scale
        }
    }

    void label_load(byte [] labels, int offset){
        output = int(labels[offset]);
        
        for(int i=0; i<10; i++){
            if(i == output){    outputs[i] = 1.0;   }
            else{  outputs[i] = -1.0;  }
        }
    }
    
}

void load_data(){    
    byte [] images = loadBytes("t10k-images-14x14.idx3-ubyte");
    byte [] labels = loadBytes("t10k-labels.idx1-ubyte");
    training_set = new ccard[8000];
    int tr_pos = 0;

    testing_set = new ccard[2000];
    int te_pos = 0;

    for(int i=0; i<10000; i++){
        if(i%5 != 0){ 
            training_set[tr_pos] = new ccard();
            training_set[tr_pos].image_load(images, 16 + i * 196);   // There is an offset of 16 bytes
            training_set[tr_pos].label_load(labels, 8 + i);          // There is an offset of 8 bytes
            tr_pos++;
        }
        else{
            testing_set[te_pos] = new ccard();
            testing_set[te_pos].image_load(images, 16 + i * 196);    // There is an offset of 16 bytes 
            testing_set[te_pos].label_load(labels, 8 + i);           // There is an offset of 8 bytes
            te_pos++;
        }
    }
}