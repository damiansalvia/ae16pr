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
Problem::Problem() : _dimension(0),_ciudades(NULL),_camino(NULL) {
}

ostream& operator<<(ostream& os, const Problem& pbm) {
	os << endl << endl << "Numero de variables " << pbm._dimension << endl;

	// TODO - Imprimo el arreglo con el costo de viaje entre ciudades
	os<<"Matriz costo ciudades: "<<endl;
	for (int i=0;i<pbm._dimension;i++){
		os<<i<<": ";
		int indice=0;
		while (pbm._ciudades[i][indice]!=-1){
			os<<pbm._ciudades[i][indice]<<" ";
			indice++;
		}
		os<<endl;
	}
	os<<endl;

	return os;
}

istream& operator>>(istream& is, Problem& pbm) {
	char buffer[MAX_BUFFER];
	int i;

	//pbm._dimension = 5; // TODO - Harcoded

	is.getline(buffer, MAX_BUFFER, '\n');
	sscanf(buffer, "%d", &pbm._dimension);

	// TODO - Inicializacion matriz cuidades
	pbm._ciudades = new int*[pbm._dimension];
	for (int i = 0; i < pbm._dimension; i++) {
		pbm._ciudades[i] = new int[pbm._dimension];
		for (int j = 0; j < pbm._dimension; j++) {
			pbm._ciudades[i][j] = -1;
		}
	}

	// TODO - Cargar datos desde archivo
	FILE* stream = fopen("ej1_matriz", "r");
	char line[1024];
	i = 0;
	while (fgets(line, 1024, stream) && i < 5) {
		for (int j = 0; j < 5; j++) { // TODO - j harcoded
			char* tmp = strdup(line);
			const char * costo_str = pbm.getfield(tmp, j+1);
			int costo = atoi(costo_str);
			pbm._ciudades[i][j] = costo;
			free(tmp);
		}
		i++;
	}
	cout << pbm;

	return is;
}

const char* Problem::getfield(char* line, int num) {
	// TODO - Lee csv - Queremos leer generador.py
	const char* tok;
	for (tok = strtok(line, " "); tok && *tok; tok = strtok(NULL, " \n")) {
		if (!--num)
			return tok;
	}
	return NULL;
}

int** Problem::ciudades() const{
	return _ciudades;
}

int* Problem::camino() const{
	return _camino;
}

bool Problem::operator==(const Problem& pbm) const {
	if (_dimension != pbm.dimension())
		return false;
	return true;
}

bool Problem::operator!=(const Problem& pbm) const {
	return !(*this == pbm);
}

Direction Problem::direction() const {
	// return maximize;
	return minimize;
}

int Problem::dimension() const {
	return _dimension;
}

Problem::~Problem() {
	// TODO - Libero la memoria pedida para almacenar los limites de los barrios
	for (int i=0;i<_dimension;i++)
		delete [] _ciudades[i];
	delete [] _ciudades;
	delete [] _camino;
}

// Solution --------------------------------------------------------------

Solution::Solution(const Problem& pbm) : _pbm(pbm), _var(pbm.dimension()) {
}

const Problem& Solution::pbm() const {
	return _pbm;
}

Solution::Solution(const Solution& sol) : _pbm(sol.pbm()) {
	*this = sol;
}

istream& operator>>(istream& is, Solution& sol) {
	for (int i = 0; i < sol.pbm().dimension(); i++)
		is >> sol._var[i];
	return is;
}

ostream& operator<<(ostream& os, const Solution& sol) {
	for (int i = 0; i < sol.pbm().dimension(); i++)
		os << " " << sol._var[i];
	return os;
}

NetStream& operator <<(NetStream& ns, const Solution& sol) {
	for (int i = 0; i < sol._var.size(); i++)
		ns << sol._var[i];
	return ns;
}

NetStream& operator >>(NetStream& ns, Solution& sol) {
	for (int i = 0; i < sol._var.size(); i++)
		ns >> sol._var[i];
	return ns;
}

Solution& Solution::operator=(const Solution &sol) {
	_var = sol._var;
	return *this;
}

bool Solution::operator==(const Solution& sol) const {
	if (sol.pbm() != _pbm)
		return false;
	for (int i = 0; i < _var.size(); i++)
		if (_var[i] != sol._var[i])
			return false;
	return true;
}

bool Solution::operator!=(const Solution& sol) const {
	return !(*this == sol);
}

void Solution::initialize() {
	for (int i = 0; i < _pbm.dimension(); i++)
		_var[i] = rand_int(0, 3);
}

double Solution::fitness() {
	double fitness = 0.0;

	// TODO - Función de fitness
	for (int i = 0; i < _var.size(); i++){
		int indice = 0;
		while (pbm().ciudades()[i][indice]!=-1){
			if (_var[i] == _var[pbm().ciudades()[i][indice]]){
				fitness++;
			}
			indice++;
		}
	}
	return fitness;
}

char *Solution::to_String() const {
	return (char *) _var.get_first();
}

void Solution::to_Solution(char *_string_) {
	int *ptr = (int *) _string_;
	for (int i = 0; i < _pbm.dimension(); i++) {
		_var[i] = *ptr;
		ptr++;
	}
}

unsigned int Solution::size() const {
	return (_pbm.dimension() * sizeof(int));
}

int& Solution::var(const int index) {
	return _var[index];
}

Rarray<int>& Solution::array_var() {
	return _var;
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
	// TODO - Implementar cruzamiento (2PX)
	int i = 0;
	Rarray<int> aux(sol1.pbm().dimension());
	aux = sol2.array_var();

	int limit = rand_int((sol1.pbm().dimension() / 2) + 1,
			sol1.pbm().dimension() - 1);
	int limit2 = rand_int(0, limit - 1);

	for (i = 0; i < limit2; i++)
		sol2.var(i) = sol1.var(i);
	for (i = 0; i < limit2; i++)
		sol1.var(i) = aux[i];
	for (i = limit; i < sol1.pbm().dimension(); i++)
		sol2.var(i) = sol1.var(i);
	for (i = limit; i < sol1.pbm().dimension(); i++)
		sol1.var(i) = aux[i];
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
	for (int i = 0; i < sol.pbm().dimension(); i++) {
		if (rand01() <= probability[1]) {
			//La mutacion modifica un gen aleatoriamente con probabilidad uniforme en {0,3}
			sol.var(i)=rand_int(0,3);
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
		for (int i=0;i<pbm.dimension();i++){
			fprintf (pFile, "%d,%d\n",i+1,solver.best_solution_trial().var(i));
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

