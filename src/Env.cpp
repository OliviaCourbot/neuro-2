﻿#include "neuron.hpp"
#include "neuron.cpp"
#include "Env.hpp"
#include <iostream>
#include <vector>
#include <math.h>
using namespace std;

//Initialisation des constantes statiques
	const double Env::time_unit(0.01);		//[millisecondes]
	const double Env::time_simu(2000); 		//[ms] = 2s

Env::Env() 
	:time(0)
{
	
	double g;
	do {
		cout << "Valeur de g ? (relative strengh of excitatory and inhibitory neurons, > 0) " << endl;
		cin >> g;
	} while (g <= 0.0);
	
	double ratio;
	do {
		cout << "Valeur de Vext/Vthr ? ( ratio > 0) " << endl;
		cin >> ratio;
	} while (ratio <= 0.0);
	
	
	unsigned int number_of_neurons(13499);
	for(unsigned int i(0); i < number_of_neurons; ++i){
		if(i<=2499){										//les neurones d'indice 0-2499 sont inhibitory
			Neuron* A = new Neuron(i, g, false, ratio);
			neurons_.push_back(A);
		}
		else if(i>= 2500){									//les neurones d'indice  2500-13499 sont excitatory
			Neuron* A = new Neuron(i, g, true, ratio);		//(ceux d'indice 12500-13499 sont le background)
			neurons_.push_back(A);
		}
	}

	cout << neurons_.size() << " neurons created ! :) " << endl;
}


Env::~Env() {
	
	//destruction de la collection de neurones
    for (unsigned int i(0); i < neurons_.size(); ++i) {
        delete neurons_[i];
    }
	
	//réinitialise la taille de neurons_ à 0
	neurons_.clear();
}

void Env::random_connection() {
	//Boucle sur tous les neurones du network
	for (unsigned int i(0); i < neurons_.size(); ++i) {
		neurons_[i]->Neuron::random_connection();
	}

}

//le neurone externe envoie un spike selon loi de poisson aux neurones auquel il est connecté -> problème on a pas de neurones externes pour l'instant 
//dc pour l'instant on va dire que les neuronnes que l'on a créé recoivent des spikes selon loi de poisson
void Env::random_spike() {
	unsigned int number_of_neurons = neurons_.size();
	for (unsigned int i(0); i<= number_of_neurons; ++i){
	double p; // la proba
	double lambda(4.0); //valeur provisoire bien sur
	double t; // temps qu'il s'est passé depuis dernier spike, ca doit etre un attribut de chaque neurone
	p= ((pow(lambda,t))/(fact(t)))*exp(-lambda);
	
	if (p>= 0.15){
		neurons_[i]->get_spike(i);
	}
}
}

double Env::fact(double x){
	int s (1);
	for (int i(1); i<=x; ++i){
		s=s*i;
	}
	return s;
}
		
double Env::get_time(){
	return time;
}

/*
double Env::get_time_simu(){
	return time_simu;
} */

void Env::actualise_time(){
	time += time_unit;
}
