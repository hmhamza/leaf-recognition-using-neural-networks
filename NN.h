#include<iostream>
using namespace std;

struct Neuron{
	double x;
	double *w;
	double e;
};

struct Layer{
	int numOfNeurons;
	Neuron *Neurons;
};

class NeuralNetwork{

	int numOfLayers;
	Layer *Layers;

	double LearningRate;
	double Threshold;

	double GenerateRandomNumber(double Low, double High)	{
		return ((double)rand() / RAND_MAX) * (High - Low) + Low;
	}

	void AssignRandomWeights(){

		for(int i=1;i<numOfLayers;i++){

			for(int j=0;j<Layers[i].numOfNeurons;j++)

				for(int k=0;k<Layers[i-1].numOfNeurons;k++)
					Layers[i].Neurons[j].w[k]=GenerateRandomNumber(-0.5, 0.5);					
		}
	}
	
	void AdjustWeights(double error){

		for(int i=1;i<numOfLayers;i++){

			for(int j=0;j<Layers[i].numOfNeurons;j++)

				for(int k=0;k<Layers[i-1].numOfNeurons;k++)
					Layers[i].Neurons[j].w[k]+=(LearningRate*Layers[i-1].Neurons[k].x*error);
		}
		
	}

	void SetInputSignal(double *input,double max){

		for(int i=0;i<Layers[0].numOfNeurons;i++)
			Layers[0].Neurons[i].x=input[i]/max;
	}

	void PropagateSignal(){

		//// Setting all X's to ZERO
		for(int i=1;i<numOfLayers;i++){
			
			for(int j=0;j<Layers[i].numOfNeurons;j++)
				Layers[i].Neurons[j].x=0;
		}


		double sum;
		for(int i=1;i<numOfLayers;i++){                //Starting from Second Layer

			for(int j=0;j<Layers[i].numOfNeurons;j++){         //For all neurons in the current Layer
				sum=0;
				for(int a=0;a<Layers[i-1].numOfNeurons;a++) {          //Getting X's multiplied with weights from the previous layer
					sum+=Layers[i-1].Neurons[a].x*Layers[i].Neurons[j].w[a];

				}
				
				if(i!=numOfLayers-1)
					Layers[i].Neurons[j].x = 1.0 / (1.0 + exp(-0.1 * sum));
				else
					Layers[i].Neurons[j].x = sum;				
			}
		}
	}

	void GetOutputSignal(double *output){

		for(int i=0;i<Layers[numOfLayers-1].numOfNeurons;i++)
			output[i]=Layers[numOfLayers-1].Neurons[i].x;
	}


public:
	NeuralNetwork(int nl,int nnl[],double a,double t){

		LearningRate=a;
		Threshold=t;

		numOfLayers=nl;
		Layers=new Layer[numOfLayers];

		for(int i=0;i<numOfLayers;i++){
			Layers[i].numOfNeurons=nnl[i];
			Layers[i].Neurons=new Neuron[nnl[i]];
		}

		for(int i=0;i<numOfLayers;i++){

			if(i!=0){
				for(int j=0;j<Layers[i].numOfNeurons;j++){
					Layers[i].Neurons[j].w=new double[Layers[i-1].numOfNeurons];
				}
			}

			else{
				for(int j=0;j<Layers[i].numOfNeurons;j++){
					Layers[i].Neurons[j].w=nullptr;
					
				}
			}
		}
	
	}

	~NeuralNetwork(){

		for(int i=0;i<numOfLayers;i++)
			delete Layers[i].Neurons;
		
		delete Layers;
	}

	void Train(double input[][3072],double *Expected_Output,int size,int maxIter){

		AssignRandomWeights();
		double *output=new double[1];
		for(int i=0;i<maxIter;i++){

			SetInputSignal(input[i%size],255.0);
			PropagateSignal();
			GetOutputSignal(output);

			if(output[0]<=Threshold)
				output[0]=0;
			else
				output[0]=1;

			if(output[0]!=Expected_Output[i%size])
				AdjustWeights(Expected_Output[i%size]-output[0]);		//Passing the error
		}
		
	}

	double* Test(double *input){

		SetInputSignal(input,255.0);
		PropagateSignal();
		double *output=new double[1];
		GetOutputSignal(output);
		return output;
	}
};