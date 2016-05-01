#ifndef INC_REQ_prAE
#define INC_REQ_prAE
#include "prAE.hh"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

skeleton prAE {

// Problem ---------------------------------------------------------------
// TODO - Despues de "Problem():" van todos los atributos inicializados
Problem::Problem() : _num_ciudades(0),_ciudades(NULL) {
}

ostream& operator<<(ostream& os, const Problem& pbm) {
	os << endl << endl;

	// TODO - Imprimo los datos de mis atributos problema
	os << "Cantidad ciudades: " << pbm._num_ciudades << endl;

	os << "Temporadas: ";
	for(int i = 0; i < 3; i++)
		os << ((i == 0)?"Baja(":(i == 1)?"Media(":"Alta(") << pbm._temporadas[i] << ") ";
	os << endl;


	os << "Matriz costo ciudades: " << endl;
	for (int i = 0 ; i < pbm._num_ciudades ; i++){
		os << i << ": ";
		for(int j = 0; j < pbm._num_ciudades; j++)
			os << pbm._ciudades[i][j] << " ";
		os << endl;
	}
	os << endl;

	return os;
}

istream& operator>>(istream& is, Problem& pbm) {
	char buffer[MAX_BUFFER];
	int i;

	// TODO - Leer archivo con parametros para generador
	is.getline(buffer,MAX_BUFFER, '\n');
	pbm._num_ciudades = atoi(buffer);
	is.getline(buffer,MAX_BUFFER, '\n');
	float tasa_ciudades_no_directo = (float) atof(buffer);
	is.getline(buffer,MAX_BUFFER, '\n');
	char* nombre = buffer;

	// TODO - Invocar al generador
	char command_buffer[MAX_BUFFER];
	sprintf(command_buffer,"python generador.py %d %f %s",pbm._num_ciudades,tasa_ciudades_no_directo, nombre);
	system(command_buffer);

	// TODO - Inicializacion matriz cuidades
	pbm._ciudades = new int*[pbm._num_ciudades];
	for (int i = 0; i < pbm._num_ciudades; i++) {
		pbm._ciudades[i] = new int[pbm._num_ciudades];
		for (int j = 0; j < pbm._num_ciudades; j++) {
			pbm._ciudades[i][j] = -1;
		}
	}

	// TODO - Cargar datos desde archivo
	FILE* stream = fopen("ej1_matriz", "r");
	char line[1024];
	i = 0;
	while (fgets(line, 1024, stream) && i < pbm._num_ciudades) {
		for (int j = 0; j < pbm._num_ciudades; j++) {
			char* tmp = strdup(line);
			const char* costo_str = pbm.getfield(tmp, j);
			pbm._ciudades[i][j] = atoi(costo_str);
			free(tmp);
		}
		i++;
	}
	fclose(stream);

	stream = fopen("ej1_temporadas", "r");
	i = 0;
	while (fgets(line, 1024, stream) && i < 3) {
		char* tmp = strdup(line);
		const char * tmp_str = strtok(tmp,",");
		pbm._temporadas[i] = atoi(tmp_str);
		free(tmp);
		i++;
	}
	fclose(stream);

	cout << pbm;

	return is;
}

const char* Problem::getfield(char* line, int pos) {
	// TODO - Leer celda en fila de matriz
	const char* tok;
	for (tok = strtok(line, " "); tok && *tok; tok = strtok(NULL, " \n")) {
		if (!pos--)
			return tok;
	}
	return NULL;
}

int** Problem::ciudades() const{
	return _ciudades;
}

const int* Problem::temporadas() const {
	return _temporadas;
}

int Problem::num_ciudades() const {
	return _num_ciudades;
}

bool Problem::operator==(const Problem& pbm) const {
	// FIXME - Comparar dos insancias de tipo Problem por medio de la matriz
	if (_num_ciudades != pbm.num_ciudades())
		return false;
	return true;
}

bool Problem::operator!=(const Problem& pbm) const {
	return !(*this == pbm);
}

Direction Problem::direction() const {
	//return maximize;
	return minimize;
}

Problem::~Problem() {
	// TODO - Liberar memoria de las estructuras
	for (int i=0;i<_num_ciudades;i++)
		delete [] _ciudades[i];
	delete [] _ciudades;
}

// Solution --------------------------------------------------------------

Solution::Solution(const Problem& pbm) : _pbm(pbm), _camino(pbm.num_ciudades() ) {
}

const Problem& Solution::pbm() const {
	return _pbm;
}

Solution::Solution(const Solution& sol) : _pbm(sol.pbm()) {
	*this = sol;
}

istream& operator>>(istream& is, Solution& sol) {
	for (int i = 0; i < sol.pbm().num_ciudades(); i++)
		is >> sol._camino[i];
	return is;
}

ostream& operator<<(ostream& os, const Solution& sol) {
	for (int i = 0; i < sol.pbm().num_ciudades(); i++)
		os << " " << sol._camino[i];
	return os;
}

NetStream& operator <<(NetStream& ns, const Solution& sol) {
	for (int i = 0; i < sol._camino.size(); i++)
		ns << sol._camino[i];
	return ns;
}

NetStream& operator >>(NetStream& ns, Solution& sol) {
	for (int i = 0; i < sol._camino.size(); i++)
		ns >> sol._camino[i];
	return ns;
}

Solution& Solution::operator=(const Solution &sol) {
	_camino = sol._camino;
	return *this;
}

bool Solution::operator==(const Solution& sol) const {
	if (sol.pbm() != _pbm)
		return false;
	for (int i = 0; i < _camino.size(); i++)
		if (_camino[i] != sol._camino[i])
			return false;
	return true;
}

bool Solution::operator!=(const Solution& sol) const {
	return !(*this == sol);
}

void Solution::initialize() {
	// TODO - Inicializar el camino aleatoriamente
	int max = _pbm.num_ciudades();
	for (int i = 0; i < max; i++)
		_camino[i] = i;

	for(int i = 0; i < max*5 ; i++){ // 5 es arbitrario para dispersar valor inicial
		int ind1 = rand_int(1,max-1); // La posicion 0 no se cambia, por eso entre 1..max-1
		int ind2 = rand_int(1,max-1);

		// Hace swap para generar aleatorio
		int aux = _camino[ind1];
		_camino[ind1] = _camino[ind2];
		_camino[ind2] = aux;
	}
}

double Solution::fitness() {
	// TODO - Inicializar fitness
	double fitness = 0.0;

	// Declarar constantes
	int temp_media = 1.1, temp_alta = 1.3;
	int ini_temp_media = _pbm.temporadas()[1], ini_temp_alta = _pbm.temporadas()[2];

	// TODO - Función de fitness
	int dia = 1;
	for (int i = 1; i < _camino.size(); i++){
		int valor = _pbm.ciudades()[i-1][i];
		if(valor==-1){
			// TODO - Pensar caso
		}else if (dia < ini_temp_media){ // Caso temprada baja
			fitness += valor;
		}else if (dia < ini_temp_alta){ // Caso temporada media
			fitness += valor*temp_media;
		}else{ // Caso temporada alta
			fitness += valor*temp_alta;
		}
		dia += 5;
	}
	return fitness;
}

char *Solution::to_String() const {
	return (char *) _camino.get_first();
}

void Solution::to_Solution(char *_string_) {
	int *ptr = (int *) _string_;
	for (int i = 0; i < _pbm.num_ciudades(); i++) {
		_camino[i] = *ptr;
		ptr++;
	}
}

unsigned int Solution::size() const {
	return (_pbm.num_ciudades() * sizeof(int));
}

int& Solution::pos(const int index) {
	return _camino[index];
}

Rarray<int>& Solution::camino() {
	return _camino;
}

Solution::~Solution() {
}

// UserStatistics -------------------------------------------------------

UserStatistics::UserStatistics() {
}

ostream& operator<<(ostream& os, const UserStatistics& userstat) {
	// TODO - Imprimir estadísiticas lindo
	os << "\n---------------------------------------------------------------"
			<< endl;
	os << "                   STATISTICS OF TRIALS                   	 "
			<< endl;
	os << "------------------------------------------------------------------"
			<< endl;

	for (int i = 0; i < userstat.result_trials.size(); i++) {
		os << endl << userstat.result_trials[i].trial << "\t"
				<< userstat.result_trials[i].best_cost_trial << "\t\t"
				<< userstat.result_trials[i].worst_cost_trial << "\t\t\t"
				<< userstat.result_trials[i].nb_evaluation_best_found_trial
				<< "\t\t\t"
				<< userstat.result_trials[i].nb_iteration_best_found_trial
				<< "\t\t\t" << userstat.result_trials[i].time_best_found_trial
				<< "\t\t" << userstat.result_trials[i].time_spent_trial;
	}
	os << endl
			<< "------------------------------------------------------------------"
			<< endl;
	return os;
}

UserStatistics& UserStatistics::operator=(const UserStatistics& userstats) {
	result_trials = userstats.result_trials;
	return (*this);
}

void UserStatistics::update(const Solver& solver) {
	if ((solver.pid() != 0) || (solver.end_trial() != true)
			|| ((solver.current_iteration()
					!= solver.setup().nb_evolution_steps())
					&& !terminateQ(solver.pbm(), solver, solver.setup())))
		return;

	struct user_stat *new_stat;

	if ((new_stat = (struct user_stat *) malloc(sizeof(struct user_stat)))
			== NULL)
		show_message(7);
	new_stat->trial = solver.current_trial();
	new_stat->nb_evaluation_best_found_trial =
			solver.evaluations_best_found_in_trial();
	new_stat->nb_iteration_best_found_trial =
			solver.iteration_best_found_in_trial();
	new_stat->worst_cost_trial = solver.worst_cost_trial();
	new_stat->best_cost_trial = solver.best_cost_trial();
	new_stat->time_best_found_trial = solver.time_best_found_trial();
	new_stat->time_spent_trial = solver.time_spent_trial();

	result_trials.append(*new_stat);
}

void UserStatistics::clear() {
	result_trials.remove();
}

UserStatistics::~UserStatistics() {
	result_trials.remove();
}

// Intra_operator  --------------------------------------------------------------

Intra_Operator::Intra_Operator(const unsigned int _number_op) :
		_number_operator(_number_op), probability(NULL) {
}

unsigned int Intra_Operator::number_operator() const {
	return _number_operator;
}

Intra_Operator *Intra_Operator::create(const unsigned int _number_op) {
	switch (_number_op) {
	case 0:
		return new Crossover;
		break;
	case 1:
		return new Mutation();
		break;
	}
	return NULL; // TODO - Sino da error
}

ostream& operator<<(ostream& os, const Intra_Operator& intra) {
	switch (intra.number_operator()) {
	case 0:
		os << (Crossover&) intra;
		break;
	case 1:
		os << (Mutation&) intra;
		break;
	}
	return os;
}

Intra_Operator::~Intra_Operator() {
}

//  Crossover:Intra_operator -------------------------------------------------------------

Crossover::Crossover() :
		Intra_Operator(0) {
	probability = new float[1];
}

void Crossover::cross(Solution& sol1, Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
		{
	// TODO -
	int i = 0;
	Rarray<int> aux(sol1.pbm().num_ciudades());
	aux = sol2.camino();

	int limit = rand_int((sol1.pbm().num_ciudades() / 2) + 1,
			sol1.pbm().num_ciudades() - 1);
	int limit2 = rand_int(0, limit - 1);

	for (i = 0; i < limit2; i++)
		sol2.pos(i) = sol1.pos(i);
	for (i = 0; i < limit2; i++)
		sol1.pos(i) = aux[i];
	for (i = limit; i < sol1.pbm().num_ciudades(); i++)
		sol2.pos(i) = sol1.pos(i);
	for (i = limit; i < sol1.pbm().num_ciudades(); i++)
		sol1.pos(i) = aux[i];
}

void Crossover::execute(Rarray<Solution*>& sols) const {
	for (int i = 0; i + 1 < sols.size(); i = i + 2)
		if (rand01() <= probability[0])
			cross(*sols[i], *sols[i + 1]);
}

ostream& operator<<(ostream& os, const Crossover& cross) {
	os << "Crossover." << " Probability: " << cross.probability[0] << endl;
	return os;
}

void Crossover::RefreshState(const StateCenter& _sc) const {
	_sc.set_contents_state_variable("_crossover_probability",
			(char *) probability, 1, sizeof(float));
}

void Crossover::UpdateFromState(const StateCenter& _sc) {
	unsigned long nbytes, length;
	_sc.get_contents_state_variable("_crossover_probability",
			(char *) probability, nbytes, length);
}

void Crossover::setup(char line[MAX_BUFFER]) {
	int op;
	sscanf(line, " %d %f ", &op, &probability[0]);
	assert(probability[0] >= 0);
}

Crossover::~Crossover() {
	delete[] probability;
}

//  Mutation: Sub_operator -------------------------------------------------------------

Mutation::Mutation() :
		Intra_Operator(1) {
	probability = new float[2];
}

void Mutation::mutate(Solution& sol) const {
	// TODO - Implementar mutación
	for (int i = 0; i < sol.pbm().num_ciudades(); i++) {
		if (rand01() <= probability[1]) {
			//La mutacion modifica un gen aleatoriamente con probabilidad uniforme en {0,3}
			sol.pos(i)=rand_int(0,3);
		}
	}
}

void Mutation::execute(Rarray<Solution*>& sols) const {
	for (int i = 0; i < sols.size(); i++)
		if (rand01() <= probability[0])
			mutate(*sols[i]);
}

ostream& operator<<(ostream& os, const Mutation& mutation) {
	os << "Mutation." << " Probability: " << mutation.probability[0]
			<< " Probability1: " << mutation.probability[1] << endl;
	return os;
}

void Mutation::setup(char line[MAX_BUFFER]) {
	int op;
	sscanf(line, " %d %f %f ", &op, &probability[0], &probability[1]);
	assert(probability[0] >= 0);
	assert(probability[1] >= 0);
}

void Mutation::RefreshState(const StateCenter& _sc) const {
	_sc.set_contents_state_variable("_mutation_probability",
			(char *) probability, 2, sizeof(probability));
}

void Mutation::UpdateFromState(const StateCenter& _sc) {
	unsigned long nbytes, length;
	_sc.get_contents_state_variable("_mutation_probability",
			(char *) probability, nbytes, length);
}

Mutation::~Mutation() {
	delete[] probability;
}

// StopCondition_1 -------------------------------------------------------------------------------------

StopCondition_1::StopCondition_1() : StopCondition() {
}

bool StopCondition_1::EvaluateCondition(const Problem& pbm,
		const Solver& solver, const SetUpParams& setup) {
	// TODO - Implementar condicion de parada. Si el fitness es 0 terminamos la ejecucion.
	bool fin=(int)solver.best_cost_trial() == 0;
	if (fin){
		// TODO - Escribir resultado en archivo de salida
		FILE * pFile;
		pFile = fopen ("res/sol.my.output","w");
		for (int i=0;i<pbm.num_ciudades();i++){
			fprintf (pFile, "%d,%d\n",i+1,solver.best_solution_trial().pos(i));
		}
		fclose (pFile);
	}

	return (fin);
}

StopCondition_1::~StopCondition_1() {
}

//------------------------------------------------------------------------
// Specific methods ------------------------------------------------------
//------------------------------------------------------------------------

bool terminateQ(const Problem& pbm, const Solver& solver,
		const SetUpParams& setup) {
	StopCondition_1 stop;
	return stop.EvaluateCondition(pbm, solver, setup);
}
}

#endif

