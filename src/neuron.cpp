//#include <neuro-2/neuron.hpp>
#include "neuron.hpp"
#include <iostream>
#include <cstdlib>		//pour fonction rand()

using namespace std;

//Initialisation des constantes static
	const double Neuron::v_reset(10);         	   	 //[milliVolt]
    const double Neuron::potential_amplitude(0.1);   //[milliVolt]
	const int Neuron::refractory_period(2);          //[milliseconde]
	const int Neuron::firing_threshold(20);          //[milliVolt]
	const double Neuron::transmission_delay(1.5);    //[milliseconde]
	const int Neuron::membrane_time(20);             //[milliseconde]

    const double Neuron::connection_probability(0.1);
    const int Neuron::excitatory_connection(1000);       //nb of excitatory connections for each neuron
	const int Neuron::inhibatory_connection(250);        //nb of inhibitory connections for each neuron
    const int Neuron::ext_excitatory_connection(1000);   //nb of external excitatory connections for each neuron
	
	const int Neuron::excitatory_neurons(10000);    	//nb of excitatory neurons in the network
    const int Neuron::inhibatory_neurons(2500);   	//nb of inhibitory neurons in the network

//Methodes
	
Neuron::Neuron(int neuron_number_, double g_, bool excitatory_, double rapport_vext_over_vthr_)
    :numero_neuron(neuron_number_), //initialisation du numero du neuron
    compteur_spikes(0.0),           //initialisation du nb de spikes a 0
    potential(v_reset),             //initialisation du potentiel a la valeur de repos
    active_state(true),             //etat de départ est actif
    is_excitatory(excitatory_),
    g(g_)                           //initialisation de g

{
    //Calcul des valeurs pour Vthr et Vext
    v_thr = firing_threshold / (potential_amplitude * excitatory_connection * membrane_time);
    v_ext = rapport_vext_over_vthr_ * v_thr;
    
}

Neuron::~Neuron() {
    //vide si pas d'utilisation de pointeurs
}
  
                   
int Neuron::get_numero(){
    return numero_neuron;
}
	
double Neuron::get_compteur(){
    return compteur_spikes;
}
	
double Neuron::get_potential(){
    return potential;
}
	
	
void Neuron::reset(){
    //remet le potentiel du neurone a la valeur v_reset après avoir
    //envoyé un spike
    potential = v_reset;
    active_state = true;
}
	
void Neuron::refractory(){
    //rend le neurone incapable de produire un spike pendant un certain
    //temps (refractory_time)
   
   	active_state = false;
   		
    /*do {
		active_state = false;
		last_spike += Env::time_unit;
	} while (Env::time < last_spike + Neuron::refractory_period);
	* pb : pas de cohérence du temps (passe plus vite ici que dans env
	* et pas d'accès à Env::time dans ce fichier
	*/
	
}

void Neuron::random_connection() {
	  /* Pour un nombre random entre min et max :
      * a = min + rand() % (max - min + 1 );
      * exemple : nb entre 250 et 1249 : neurones excitateurs
      * a = 250 + rand() % 1000;
      */
     
    int number;
    
	//Connections avec les neurons inhibiteurs
	if (is_excitatory == false) {										//AVANT: for (unsigned int i(0); i < Neuron::inhibatory_connection; ++i)
		number = rand() % Neuron::inhibatory_neurons;
		this->Neuron::add_connection(numero_neuron, number);
	}
     
     //Connections avec les neurons excitateurs
	else if (is_excitatory == true) {									// AVANT: unsigned int borne_max(Neuron::excitatory_connection +  Neuron::inhibatory_connection);
																		//for (unsigned int i(Neuron::inhibatory_connection); i < borne_max; ++i) {
		number = Neuron::inhibatory_neurons + rand() % Neuron::excitatory_neurons;
		this->Neuron::add_connection(numero_neuron, number);
	}

	
}

void Neuron::add_connection(int indice, int number) {
	this->connections_[indice] = number;
}

void Neuron::get_spike(int number) {
	//number est le numéro du neurone qui ENVOIE le spike
	
	//sauf erreur l'indice dans le tableau = numero du neuron - 1
	//donc les neurones n°1 à 2500 sont inhibiteurs, et 2501-12500 excitateurs
	//pour le moment les neurones d'indice 12501 à 13500 sont excitateurs externes
	
	//(paranoia : controler que le numero est positif, sinon faire une erreur)
	
	//Recu d'un neurone inhibiteur
	if (number <= Neuron::inhibatory_neurons) {
		
		if (this->potential >= g*Neuron::potential_amplitude) {
			this->potential -= g*Neuron::potential_amplitude;
			compteur_spikes += 1; 
			
		} else if (this->potential > 0) {
			this->potential = 0;
			compteur_spikes += 1;
		}
		//dans les autres cas il ne se passe rien 
		//(en considérant que le potentiel ne peux pas etre negatif)
		//sinon mettre une limite minimale autre que 0 
	}
	
	//Recu d'un neurone excitateur (du network ou externe)
	int total_number(Neuron::inhibatory_neurons + Neuron::excitatory_neurons + Neuron::ext_excitatory_connection);
	if ((number > Neuron::inhibatory_neurons) and (number < total_number )) {
		this->potential += Neuron::potential_amplitude;
		compteur_spikes += 1;
	}
	
}

int Neuron::is_times_spikes_empty(){
	return times_spikes.size();		
}

void Neuron::times_spikes_add(int x){
	times_spikes.push_back(x);
}

int Neuron::get_time_last_spike(){
	int b(times_spikes.size());
	return times_spikes[b-1];
}

vector<int> Neuron::get_times_spikes(){
	return times_spikes;
}
