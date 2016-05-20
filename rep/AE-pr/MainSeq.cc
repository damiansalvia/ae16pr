#include "prAE.hh"
#include <stdlib.h>
#include <string.h>

int main (int argc, char** argv){
	// Invocacion: ./MainSeq $(path_cgf) $(path_params) $(path_out)

	using skeleton prAE;

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
				<< "Iteration: " << solver.iteration_best_found()<< endl << endl;
		cout << ":( ---------------------- THE END --------------- :) " <<endl;

		// Copiar solucion al archivo de salida
		ofstream fexit(argv[3]);
		if(!fexit) show_message(13);
		fexit << solver.global_best_solution();

	}
	return(0);
}
