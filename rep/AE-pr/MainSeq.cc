#include "prAE.hh"
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main (int argc, char** argv){
	// Invocacion: ./MainSeq $(path_cgf) $(path_params) $(path_out)

	using skeleton prAE;
	int musec, sec, min, hr, _10e6_ = (int)10e5;

	system("clear");

	// Verificar que estén todos los argumentos
	if(argc < 4) show_message(1);

	// Cargar *.cfg
	ifstream f1(argv[1]);
	if (!f1) show_message(11);

	// Cargar archivo con parametros
	ifstream f2(argv[2]);
	if (!f2) show_message(12);

	// Cargar problema
	Problem pbm;
	f2 >> pbm;

	// Cargar parametros de configuracion
	Operator_Pool pool(pbm);
	SetUpParams cfg(pool);
	f1 >> cfg;
//	cout << cfg; // Imprimir configuración

	// Arrancar algoritmo
	Solver_Seq solver(pbm,cfg);
	solver.run();

	if (solver.pid()==0) {
		// Mostrar estadísticas finales
		//solver.show_state();
		cout << endl << endl
				<< ":( ------------------- THE SOLUTION ------------- :)" << endl << endl
				<< "Solution: " << solver.global_best_solution() << endl
				<< "Fitness: " << solver.global_best_solution().fitness() << endl // FIXME
				<< "Iteration: " << solver.iteration_best_found() << "/" << solver.current_iteration() << endl << endl;

		musec = (int) solver.time_best_found();
		sec   = (int) (musec / _10e6_) % 60;
		min   = (int) ((musec / (_10e6_*60)) % 60);
		hr    = (int) ((musec / (_10e6_*60*60)) % 24);
		printf("Time best found: %02i:%02i:%02i\n",hr,min,sec);

		musec = (int) solver.current_time_spent();
		sec   = (int) (musec / _10e6_) % 60;
		min   = (int) ((musec / (_10e6_*60)) % 60);
		hr    = (int) ((musec / (_10e6_*60*60)) % 24);
		printf("Total time spent: %02i:%02i:%02i\n\n",hr,min,sec);
		cout << ":( ---------------------- THE END --------------- :) " <<endl;

		// Copiar solucion al archivo de salida
		ofstream fexit(argv[3]);
		if(!fexit) show_message(13);
		fexit << solver.global_best_solution();

	}
	return(0);
}
