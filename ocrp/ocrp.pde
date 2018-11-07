int totalTrain = 0; 
int totalTest = 0;
int totalRight = 0;
float sucess = 0;
int testCard = 0;
int trainCard = 0;

cnetwork neuralnet;
cbutton trainB, testB;

void setup(){
    size(1000, 400);
    setup_sigmoid();
    load_data();
    neuralnet = new cnetwork(196, 49, 10);
    smooth();
    stroke(150);

    trainB = new cbutton(width*0.06, height*0.9, "Train");
    testB = new cbutton(width*0.11, height*0.9, "Test");
}

void draw(){
    background(250);
    neuralnet.display();

    fill(100);

    if(totalTest>0) sucess = float(totalRight)/float(totalTest);
    text("Success rate: " + nfc(sucess, 2), width*0.44, height*0.89);
    text("Card label: " + testing_set[testCard].output, width*0.44, height*0.93);

    trainB.display();
    testB.display();
}

void mousePressed(){
    if(trainB.hover()) {
        for (int i = 0; i < 500; i++) {
            trainCard = (int) floor(random(0, training_set.length));
            neuralnet.respond(training_set[trainCard]);
            neuralnet.train(training_set[trainCard].outputs);
            totalTrain++;
        }
    }
    else if(testB.hover()){
        testCard = (int) floor(random(0, testing_set.length));
        neuralnet.respond(testing_set[testCard]);
        neuralnet.display();
        if(neuralnet.best_index == testing_set[testCard].output) totalRight ++;
        totalTest ++;
    }
    redraw();
}
