#include "prAE.hh"
#include <stdlib.h>
#include <string.h>

int main (int argc, char** argv)
{
	using skeleton prAE;

	system("clear");

	// Verificar que estén todos los argumentos
	// self cgf num tas nom out
	if(argc < 7) show_message(1);

	// Cargar *.cfg
	ifstream f1(argv[1]);
	if (!f1) show_message(11);

	// Crear el path y crear archivo con parametros
	char path[32]; // ins/<nomb>.in
	sprintf((char*)path,"ins/%s.in",argv[4]);
	FILE *fp = fopen(path,"w");
	fprintf(fp,"%s\n%s\n%s\n%s\n",argv[2],argv[3],argv[4],argv[5]);
	fclose(fp);

	// Cargar archivo con parametros
	ifstream f2(path);
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
		solver.show_state();
		cout << endl << endl << "Solution --------------------------------------------------" << endl << endl
				<< "Solution: " << solver.global_best_solution() << endl
				<< "Fitness: " << round(solver.global_best_solution().fitness()) << endl // FIXME
				<< "Iteration: " << solver.iteration_best_found()<< endl << endl;
		cout << "\n\n :( ---------------------- THE END --------------- :) " <<endl;

		// Copiar solucion al archivo de salida
		ofstream fexit(argv[6]);
		if(!fexit) show_message(13);
		fexit << solver.best_solution_trial();

	}
	return(0);
}
